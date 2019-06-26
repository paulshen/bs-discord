open Types;

let createMessage = (channelId: snowflake, content: string) => {
  let body = Js.Dict.empty();
  Js.Dict.set(body, "content", Js.Json.string(content));
  Api.requestPost({j|/channels/$channelId/messages|j}, ~body, ())
  |> Js.Promise.(
       then_(json => {
         let message = PayloadParser.message(json);
         Js.log2("createMessage", message);
         resolve(message);
       })
     );
};

let createParams = (params: list((string, option(string)))) => {
  let filteredParams = [||];
  List.iter(
    ((key, value)) =>
      switch (value) {
      | Some(value) => Js.Array.push((key, value), filteredParams) |> ignore
      | None => ()
      },
    params,
  );
  filteredParams;
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
        createParams([
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