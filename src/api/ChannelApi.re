open Types;

let createMessage = (channelId: snowflake, content: string) => {
  let body = Js.Dict.empty();
  let token = Constants.token;
  Js.Dict.set(body, "content", Js.Json.string(content));
  Js.Promise.(
    Fetch.fetchWithInit(
      Constants.apiBaseUrl ++ {j|/channels/$channelId/messages|j},
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=Fetch.BodyInit.make(Js.Json.stringify(Js.Json.object_(body))),
        ~headers=
          Fetch.HeadersInit.make({
            "Content-Type": "application/json",
            "Authorization": {j|Bot $token|j},
          }),
        (),
      ),
    )
    |> then_(response =>
         switch (Fetch.Response.status(response)) {
         | 200
         | 201
         | 204
         | 304 => resolve(response)
         | statusCode => reject(Api.ApiError(statusCode))
         }
       )
    |> then_(Fetch.Response.json)
    |> then_(json => {
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