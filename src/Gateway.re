open WebsocketClient;
open PayloadTypes;

exception Unsupported;
external hackType: 'a => 'b = "%identity";

type state = {
  ws: Websocket.t(string),
  sessionId: ref(option(string)),
  lastSequenceId: ref(option(int)),
};

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

let handleMessage = ({ws, lastSequenceId, sessionId}, message) => {
  switch (message) {
  | Hello(payload) => startHeartbeat(ws, lastSequenceId, payload)
  | Dispatch(Ready(readyPayload)) =>
    sessionId := Some(readyPayload.sessionId)
  | Dispatch(GuildCreate(guild)) =>
    switch (guild.presences) {
    | Some(presences) => PresenceStore.updatePresences(presences)
    | None => ()
    }
  | Dispatch(MessageCreate(message)) =>
    if (Js.String.indexOf(message.content, "ping") != (-1)) {
      ChannelApi.createMessage(message.channelId, "pong") |> ignore;
    }
  | Dispatch(PresenceUpdate(presenceUpdate)) =>
    PresenceStore.updatePresence(presenceUpdate)
  | _ => ()
  };
};

let createSocket =
    (~token, ~onOpen=?, ~onMessage=?, ~onError=?, ~onClose=?, ()) => {
  let state = {
    ws: Websocket.make("wss://gateway.discord.gg/?v=6&encoding=json"),
    sessionId: ref(None),
    lastSequenceId: ref(None),
  };

  Websocket.onOpen(
    state.ws,
    e => {
      switch (onOpen) {
      | Some(onOpen) => onOpen(e)
      | None => ()
      };
      switch (state.sessionId^) {
      | Some(_) => resume(state, token)
      | None => identify(state, token)
      };
    },
  );

  Websocket.onMessage(
    state.ws,
    e => {
      Js.log(MessageEvent.data(e));
      let json = Js.Json.parseExn(MessageEvent.data(e));
      switch (json |> Json.Decode.(field("s", optional(int)))) {
      | Some(sequenceId) => state.lastSequenceId := Some(sequenceId)
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
    state.ws,
    e => {
      switch (onError) {
      | Some(onError) => onError(e)
      | None => ()
      };
      Websocket.close(state.ws);
    },
  );

  Websocket.onClose(state.ws, e =>
    switch (onClose) {
    | Some(onClose) => onClose(e)
    | None => ()
    }
  );
};