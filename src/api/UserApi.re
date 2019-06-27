open Types;

let getCurrentUser = () => {
  Js.Promise.(
    Api.requestGet("/users/@me", ())
    |> then_(json => json |> PayloadParser.user |> resolve)
  );
};

let getUser = (userId: snowflake) => {
  Js.Promise.(
    Api.requestGet({j|/users/$userId|j}, ())
    |> then_(json => json |> PayloadParser.user |> resolve)
  );
};

external hackType: 'a => 'b = "%identity";
[@bs.deriving abstract]
type updateUserParams = {
  [@bs.optional]
  username: string,
};
/* todo: avatar */
let updateCurrentUser = (~username=?, ()) => {
  let bodyJson = hackType(updateUserParams(~username?, ()));
  Js.Promise.(
    Api.requestPatch("/users/@me", ~bodyJson, ())
    |> then_(json => json |> PayloadParser.user |> resolve)
  );
};

let getCurrentUserGuilds =
    (
      ~before: option(snowflake)=?,
      ~after: option(snowflake)=?,
      ~limit: option(int)=?,
      (),
    ) => {
  Js.Promise.(
    Api.requestGet(
      "/users/@me/guilds",
      ~queryParams=
        Api.createParams([
          ("before", before),
          ("after", after),
          ("limit", Belt.Option.map(limit, string_of_int)),
        ]),
      (),
    )
    |> then_(json =>
         json |> Json.Decode.array(PayloadParser.guild) |> resolve
       )
  );
};

let leaveGuild = (guildId: snowflake) => {
  Js.Promise.(
    Api.requestDelete({j|/users/@me/guilds/$guildId|j}, ())
    |> then_(_ => resolve())
  );
};

let getUserDMs = () => {
  Js.Promise.(
    Api.requestGet("/users/@me/channels", ())
    |> then_(json =>
         json |> Json.Decode.array(PayloadParser.channel) |> resolve
       )
  );
};

let createDM = (~recipientId) => {
  let body = Js.Dict.empty();
  Js.Dict.set(body, "recipient_id", Js.Json.string(recipientId));
  Js.Promise.(
    Api.requestPost(
      "/users/@me/channels",
      ~bodyJson=Json.Encode.dict(body),
      (),
    )
    |> then_(json =>
         json |> Json.Decode.array(PayloadParser.channel) |> resolve
       )
  );
};

let getUserConnections = () => {
  Js.Promise.(
    Api.requestGet("/users/@me/connections", ())
    |> then_(json =>
         json |> Json.Decode.array(PayloadParser.userConnection) |> resolve
       )
  );
};