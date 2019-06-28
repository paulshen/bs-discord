open WebsocketClient;
open PayloadTypes;
open State;

exception Unsupported;
external hackType: 'a => 'b = "%identity";

let identify = (ws, token) => {
  Websocket.send(
    ws,
    Js.Json.stringify(
      hackType({
        "op": opCodeToJs(Identify),
        "d": {
          "token": token,
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

let resume = (ws, {sessionId, lastSequenceId}, token) => {
  Websocket.send(
    ws,
    Js.Json.stringify(
      hackType({
        "op": opCodeToJs(Resume),
        "d": {
          "token": token,
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

let startHeartbeat =
    (ws, {lastSequenceId, heartbeatInterval}, payload: helloPayload) => {
  switch (heartbeatInterval^) {
  | Some(heartbeatInterval) => Js.Global.clearInterval(heartbeatInterval)
  | None => ()
  };
  heartbeatInterval :=
    Some(
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
      ),
    );
};

let handleMessage = (state, message) => {
  let {sessionId} = state.gateway;
  switch (message) {
  | Hello(payload) =>
    startHeartbeat(Belt.Option.getExn(state.ws^), state.gateway, payload)
  | Dispatch(Ready(readyPayload)) =>
    sessionId := Some(readyPayload.sessionId)
  | Dispatch(GuildCreate(guild)) =>
    switch (guild.presences) {
    | Some(presences) =>
      state.presences->PresenceStore.updatePresences(presences)
    | None => ()
    }
  | Dispatch(PresenceUpdate(presenceUpdate)) =>
    state.presences->PresenceStore.updatePresence(presenceUpdate)
  | _ => ()
  };
};

let connectSocket =
    (
      gatewayState: gatewayState,
      ~token,
      ~onOpen,
      ~onMessage,
      ~onError,
      ~onClose,
    ) => {
  let ws = Websocket.make("wss://gateway.discord.gg/?v=6&encoding=json");

  Websocket.onOpen(
    ws,
    e => {
      switch (onOpen) {
      | Some(onOpen) => onOpen(e)
      | None => ()
      };
      switch (gatewayState.sessionId^) {
      | Some(_) => resume(ws, gatewayState, token)
      | None => identify(ws, token)
      };
    },
  );

  Websocket.onMessage(
    ws,
    e => {
      Js.log(MessageEvent.data(e));
      let json = Js.Json.parseExn(MessageEvent.data(e));
      switch (json |> Json.Decode.(field("s", optional(int)))) {
      | Some(sequenceId) => gatewayState.lastSequenceId := Some(sequenceId)
      | None => ()
      };
      onMessage(json);
    },
  );

  Websocket.onError(
    ws,
    e => {
      switch (onError) {
      | Some(onError) => onError(e)
      | None => ()
      };
      Websocket.close(ws);
    },
  );

  Websocket.onClose(
    ws,
    e => {
      switch (gatewayState.heartbeatInterval^) {
      | Some(heartbeatInterval) =>
        Js.Global.clearInterval(heartbeatInterval);
        gatewayState.heartbeatInterval := None;
      | None => ()
      };
      onClose(e);
    },
  );

  ws;
};

let createConnection =
    (~token, ~onOpen=?, ~onMessage=?, ~onError=?, ~onClose=?, ()) => {
  let gatewayState = {
    sessionId: ref(None),
    lastSequenceId: ref(None),
    heartbeatInterval: ref(None),
  };
  let state = {
    ws: ref(None),
    gateway: gatewayState,
    presences: PresenceStore.getInitialState(),
  };

  let myOnMessage = json => {
    let message = PayloadParser.parseSocketData(json);
    handleMessage(state, message);
    switch (onMessage) {
    | Some(onMessage) => onMessage(message)
    | None => ()
    };
  };

  let rec connect = () =>
    state.ws :=
      Some(
        connectSocket(
          gatewayState,
          ~token,
          ~onOpen,
          ~onMessage=myOnMessage,
          ~onError,
          ~onClose=e => {
            switch (onClose) {
            | Some(onClose) => onClose(e)
            | None => ()
            };

            connect();
          },
        ),
      );

  connect();

  state;
};