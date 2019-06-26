[%raw "require('isomorphic-fetch')"];
open PayloadTypes;
open WebsocketClient;

exception Unsupported;
external hackType: 'a => 'b = "%identity";

let ws: Websocket.t(string) =
  Websocket.make("wss://gateway.discord.gg/?v=6&encoding=json");

let sessionId: ref(option(string)) = ref(None);
let lastSequenceId: ref(option(int)) = ref(None);

let identify = () => {
  Websocket.send(
    ws,
    Js.Json.stringify(
      hackType({
        "op": opCodeToJs(Identify),
        "d": {
          "token": Constants.token,
          "properties": {
            "$os": "darwin",
            "$browser": "bs-discord",
            "$device": "bs-discord",
          },
        },
      }),
    ),
  );
};
let resume = sessionId => {
  Websocket.send(
    ws,
    Js.Json.stringify(
      hackType({
        "op": opCodeToJs(Resume),
        "d": {
          "token": Constants.token,
          "session_id": sessionId,
          "seq":
            switch (lastSequenceId^) {
            | Some(heartbeatSequence) =>
              Js.Nullable.return(heartbeatSequence)
            | None => Js.Nullable.null
            },
        },
      }),
    ),
  );
};

Websocket.onOpen(
  ws,
  _ => {
    Js.log("onOpen");
    switch (sessionId^) {
    | Some(sessionId) => resume(sessionId)
    | None => identify()
    };
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
  | Dispatch(Ready(readyPayload)) =>
    sessionId := Some(readyPayload.sessionId)
  | Dispatch(GuildCreate(guild)) =>
    switch (guild.presences) {
    | Some(presences) => PresenceStore.updatePresences(presences)
    | None => ()
    }
  | Dispatch(PresenceUpdate(presenceUpdate)) =>
    PresenceStore.updatePresence(presenceUpdate);
    ChannelApi.createMessage("546759559973175321", "hello") |> ignore;
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