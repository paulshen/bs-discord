[@bs.val] external encodeURIComponent: string => string = "";

let urlWithQueryParams = (url, params) => {
  let encodeParam = ((key, value)) =>
    encodeURIComponent(key) ++ "=" ++ encodeURIComponent(value);

  let params = params |> Array.map(encodeParam) |> Js.Array.joinWith("&");

  switch (params) {
  | "" => url
  | _ => {j|$url?$params|j}
  };
};

exception ApiError(int);
let requestGet = (url, ~queryParams=?, ()) => {
  let endpoint = Constants.apiBaseUrl ++ url;
  let token = Constants.token;
  Js.Promise.(
    Fetch.fetchWithInit(
      switch (queryParams) {
      | Some(queryParams) => urlWithQueryParams(endpoint, queryParams)
      | None => endpoint
      },
      Fetch.RequestInit.make(
        ~method_=Get,
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
  );
};

let requestPost = (url, ~bodyJson=?, ()) => {
  let token = Constants.token;
  Js.Promise.(
    Fetch.fetchWithInit(
      Constants.apiBaseUrl ++ url,
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=?{
          Belt.Option.map(bodyJson, bodyJson =>
            Fetch.BodyInit.make(Js.Json.stringify(bodyJson))
          );
        },
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
  );
};

let requestPatch = (url, ~bodyJson=?, ()) => {
  let token = Constants.token;
  Js.Promise.(
    Fetch.fetchWithInit(
      Constants.apiBaseUrl ++ url,
      Fetch.RequestInit.make(
        ~method_=Patch,
        ~body=?{
          Belt.Option.map(bodyJson, bodyJson =>
            Fetch.BodyInit.make(Js.Json.stringify(bodyJson))
          );
        },
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
  );
};

let requestPut = (url, ~bodyJson=?, ()) => {
  let token = Constants.token;
  Js.Promise.(
    Fetch.fetchWithInit(
      Constants.apiBaseUrl ++ url,
      Fetch.RequestInit.make(
        ~method_=Put,
        ~body=?{
          Belt.Option.map(bodyJson, bodyJson =>
            Fetch.BodyInit.make(Js.Json.stringify(bodyJson))
          );
        },
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
  );
};

let requestDelete = url => {
  let token = Constants.token;
  Js.Promise.(
    Fetch.fetchWithInit(
      Constants.apiBaseUrl ++ url,
      Fetch.RequestInit.make(
        ~method_=Delete,
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
  );
};