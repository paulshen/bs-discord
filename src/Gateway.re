open WebsocketClient;
open PayloadTypes;
open State;

exception Unsupported;
external hackType: 'a => 'b = "%identity";

let identify = ({ws}, token) => {
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

let resume = ({ws, sessionId, lastSequenceId}, token) => {
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

let startHeartbeat = (ws, lastSequenceId, payload) => {
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
  |> ignore;
};

let handleMessage = (state, message) => {
  let {ws, lastSequenceId, sessionId} = state.gateway;
  switch (message) {
  | Hello(payload) => startHeartbeat(ws, lastSequenceId, payload)
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

let createSocket =
    (~token, ~onOpen=?, ~onMessage=?, ~onError=?, ~onClose=?, ()) => {
  let gatewayState = {
    ws: Websocket.make("wss://gateway.discord.gg/?v=6&encoding=json"),
    sessionId: ref(None),
    lastSequenceId: ref(None),
  };
  let state = {
    gateway: gatewayState,
    presences: PresenceStore.getInitialState(),
  };

  Websocket.onOpen(
    gatewayState.ws,
    e => {
      switch (onOpen) {
      | Some(onOpen) => onOpen(e)
      | None => ()
      };
      switch (gatewayState.sessionId^) {
      | Some(_) => resume(gatewayState, token)
      | None => identify(gatewayState, token)
      };
    },
  );

  Websocket.onMessage(
    gatewayState.ws,
    e => {
      Js.log(MessageEvent.data(e));
      let json = Js.Json.parseExn(MessageEvent.data(e));
      switch (json |> Json.Decode.(field("s", optional(int)))) {
      | Some(sequenceId) => gatewayState.lastSequenceId := Some(sequenceId)
      | None => ()
      };

      let message = PayloadParser.parseSocketData(json);
      handleMessage(state, message);
      switch (onMessage) {
      | Some(onMessage) => onMessage(message)
      | None => ()
      };
      ();
    },
  );

  Websocket.onError(
    gatewayState.ws,
    e => {
      switch (onError) {
      | Some(onError) => onError(e)
      | None => ()
      };
      Websocket.close(gatewayState.ws);
    },
  );

  Websocket.onClose(gatewayState.ws, e =>
    switch (onClose) {
    | Some(onClose) => onClose(e)
    | None => ()
    }
  );

  state;
};