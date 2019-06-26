open Types;

exception ApiError(int);

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
         | statusCode => reject(ApiError(statusCode))
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