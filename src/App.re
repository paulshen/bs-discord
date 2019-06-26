open PayloadTypes;
open WebsocketClient;

exception Unsupported;
external hackType: 'a => 'b = "%identity";
let parseMessage = messageData => {
  switch (Belt.Option.getExn(opCodeFromJs(opGet(messageData)))) {
  | Hello =>
    Hello(
      {
        let json = dGet(messageData);
        Json.Decode.{
          heartbeatInterval: field("heartbeat_interval", int, json),
        };
      },
    )
  | InvalidSession => InvalidSession
  | Dispatch =>
    Dispatch(
      switch (
        dispatchMessageTypeFromJs(Belt.Option.getExn(tGet(messageData)))
      ) {
      | Some(dispatchMessageType) =>
        switch (dispatchMessageType) {
        | `Ready => Ready(hackType(dGet(messageData)))
        | `GuildCreate => GuildCreate(hackType(dGet(messageData)))
        | `MessageCreate => MessageCreate(hackType(dGet(messageData)))
        }
      | None => Unknown
      },
    )
  | _ => Unknown
  };
};

let ws: Websocket.t(string) =
  Websocket.make("wss://gateway.discord.gg/?v=6&encoding=json");

Websocket.onOpen(
  ws,
  _ => {
    Js.log("onOpen");
    Websocket.send(
      ws,
      Js.Json.stringify(
        hackType({
          "op": opCodeToJs(Identify),
          "d": {
            "token": "Mzk4OTE3OTQzNTc0MTM0Nzk1.XRKUnA.KNRkoqpdhZVMEvD3ti0abVECf-k",
            "properties": {
              "$os": "darwin",
              "$browser": "bs-discord",
              "$device": "bs-discord",
            },
          },
        }),
      ),
    );
  },
);

let handleMessage = message => {
  switch (message) {
  | Hello(payload) =>
    Js.Global.setInterval(
      () =>
        Websocket.send(
          ws,
          Js.Json.stringify(hackType({"op": opCodeToJs(Heartbeat)})),
        ),
      payload.heartbeatInterval,
    )
    |> ignore
  | _ => ()
  };
};

[@bs.scope "JSON"] [@bs.val]
external parseIntoMessageData: string => messageData = "parse";
Websocket.onMessage(
  ws,
  ev => {
    Js.log(MessageEvent.data(ev));
    let message = ev->MessageEvent.data->parseIntoMessageData->parseMessage;
    Js.log2("onMessage", message);
    handleMessage(message);
    ();
  },
);

Websocket.onError(
  ws,
  ev => {
    Js.log({j|onError: $ev|j});
    Websocket.close(ws);
  },
);
Websocket.onClose(ws, ev => Js.log({j|onClose: $ev|j}));