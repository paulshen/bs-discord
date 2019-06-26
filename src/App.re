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

let lastSequenceId: ref(option(int)) = ref(None);
let handleMessage = message => {
  switch (message) {
  | Hello(payload) =>
    Js.Global.setInterval(
      () => {
        Js.log("heartbeat");
        Websocket.send(
          ws,
          Js.Json.stringify(
            hackType({
              "op": opCodeToJs(Heartbeat),
              "d":
                switch (lastSequenceId^) {
                | Some(heartbeatSequence) =>
                  Js.Nullable.return(heartbeatSequence)
                | None => Js.Nullable.null
                },
            }),
          ),
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
    let json = Js.Json.parseExn(MessageEvent.data(ev));
    switch (json |> Json.Decode.(field("s", optional(int)))) {
    | Some(sequenceId) => lastSequenceId := Some(sequenceId)
    | None => ()
    };

    let message = PayloadParser.parseSocketData(json);
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