open Types;

let getChannel = (channelId: snowflake) => {
  Api.requestGet({j|/channels/$channelId|j}, ())
  |> Js.Promise.(then_(json => json |> PayloadParser.channel |> resolve));
};

external hackType: 'a => 'b = "%identity";
[@bs.deriving abstract]
type updateChannelParams = {
  [@bs.optional]
  name: string,
  [@bs.optional]
  position: int,
  [@bs.optional]
  topic: string,
  [@bs.optional]
  nsfw: bool,
  [@bs.optional] [@bs.as "rate_limit_per_user"]
  rateLimitPerUser: int,
  [@bs.optional]
  bitrate: int,
  [@bs.optional] [@bs.as "user_limit"]
  userLimit: int,
  [@bs.optional] [@bs.as "parent_id"]
  parentId: snowflake,
};
let updateChannel =
    (
      channelId: snowflake,
      ~name=?,
      ~position=?,
      ~topic=?,
      ~nsfw=?,
      ~rateLimitPerUser=?,
      ~bitrate=?,
      ~userLimit=?,
      ~parentId=?,
      (),
    ) => {
  let bodyJson =
    hackType(
      updateChannelParams(
        ~name?,
        ~position?,
        ~topic?,
        ~nsfw?,
        ~rateLimitPerUser?,
        ~bitrate?,
        ~userLimit?,
        ~parentId?,
        (),
      ),
    );
  Js.Promise.(
    Api.requestPatch({j|/channels/$channelId|j}, ~bodyJson, ())
    |> then_(json => {
         let channel = PayloadParser.channel(json);
         Js.log2("patchMessage", channel);
         resolve(channel);
       })
  );
};

let deleteChannel = (channelId: snowflake) => {
  Js.Promise.(
    Api.requestDelete({j|/channels/$channelId|j})
    |> then_(json => {
         let channel = PayloadParser.channel(json);
         Js.log2("deleteMessage", channel);
         resolve(channel);
       })
  );
};

[@bs.deriving abstract]
type createMessageParams = {
  [@bs.optional]
  content: string,
  [@bs.optional]
  nonce: snowflake,
  [@bs.optional]
  tts: bool,
  /* todo: file, embed */
  [@bs.optional] [@bs.as "payload_json"]
  payloadJson: string,
};
let createMessage =
    (channelId: snowflake, ~content=?, ~nonce=?, ~tts=?, ~payloadJson=?, ()) => {
  let bodyJson =
    hackType(
      createMessageParams(~content?, ~nonce?, ~tts?, ~payloadJson?, ()),
    );
  Api.requestPost({j|/channels/$channelId/messages|j}, ~bodyJson, ())
  |> Js.Promise.(
       then_(json => {
         let message = PayloadParser.message(json);
         Js.log2("createMessage", message);
         resolve(message);
       })
     );
};

let getMessages =
    (
      channelId: snowflake,
      ~around: option(snowflake)=?,
      ~before: option(snowflake)=?,
      ~after: option(snowflake)=?,
      ~limit: option(int)=?,
      (),
    ) => {
  Js.Promise.(
    Api.requestGet(
      {j|/channels/$channelId/messages|j},
      ~queryParams=
        Api.createParams([
          ("around", around),
          ("before", before),
          ("after", after),
          ("limit", Belt.Option.map(limit, string_of_int)),
        ]),
      (),
    )
    |> then_(json =>
         json |> Json.Decode.array(PayloadParser.message) |> resolve
       )
  );
};

let getMessage = (channelId: snowflake, messageId: snowflake) => {
  Js.Promise.(
    Api.requestGet({j|/channels/$channelId/messages/$messageId|j}, ())
    |> then_(json => json |> PayloadParser.message |> resolve)
  );
};

let addReaction =
    (
      channelId: snowflake,
      messageId: snowflake,
      ~emojiName: string,
      ~emojiId: snowflake,
    ) => {
  let emoji = {j|$emojiName:$emojiId|j};
  Js.Promise.(
    Api.requestPut(
      {j|/channels/$channelId/messages/$messageId/reactions/$emoji/@me|j},
      (),
    )
    |> then_(_json => resolve())
  );
};