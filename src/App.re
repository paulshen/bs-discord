open PayloadTypes;
open WebsocketClient;

exception Unsupported;
external hackType: 'a => 'b = "%identity";

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
      () => {
        Js.log("heartbeat");
        Websocket.send(
          ws,
          Js.Json.stringify(hackType({"op": opCodeToJs(Heartbeat)})),
        );
      },
      payload.heartbeatInterval,
    )
    |> ignore
  | _ => ()
  };
};

Websocket.onMessage(
  ws,
  ev => {
    Js.log(MessageEvent.data(ev));
    let message =
      PayloadParser.parseSocketData(
        Js.Json.parseExn(MessageEvent.data(ev)),
      );
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
Websocket.onClose(ws, ev => Js.log2("onClose", ev));