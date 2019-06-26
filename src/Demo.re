open Types;
open WebsocketClient;

[@bs.deriving jsConverter]
type opCode =
  | [@bs.as 0] Dispatch
  | [@bs.as 1] Heartbeat
  | [@bs.as 2] Identify
  | [@bs.as 3] StatusUpdate
  | [@bs.as 4] VoiceStateUpdate
  | [@bs.as 6] Resume
  | [@bs.as 7] Reconnect
  | [@bs.as 8] RequestGuildMembers
  | [@bs.as 9] InvalidSession
  | [@bs.as 10] Hello
  | [@bs.as 11] HeartbeatAck;

[@bs.deriving abstract]
type helloMessageData = {
  [@bs.as "heartbeat_interval"]
  heartbeatInterval: int,
};

[@bs.deriving jsConverter]
type dispatchMessageType = [
  | [@bs.as "READY"] `Ready
  | [@bs.as "GUILD_CREATE"] `GuildCreate
  | [@bs.as "MESSAGE_CREATE"] `MessageCreate
];

[@bs.deriving abstract]
type readyPayload = {
  [@bs.as "session_id"]
  sessionId: string,
  user,
  guilds: array(unavailableGuild),
};

type dispatchMessage =
  | Ready(readyPayload)
  | GuildCreate(guild)
  | MessageCreate(message)
  | Unknown;

[@bs.deriving abstract]
type messageData = {
  t: option(string),
  op: int,
  d: Js.Json.t,
};

type message =
  | Hello(helloMessageData)
  | Dispatch(dispatchMessage)
  | InvalidSession
  | Unknown;

exception Unsupported;
external hackType: 'a => 'b = "%identity";
let parseMessage = messageData => {
  switch (Belt.Option.getExn(opCodeFromJs(opGet(messageData)))) {
  | Hello => Hello(hackType(dGet(messageData)))
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
      heartbeatIntervalGet(payload),
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