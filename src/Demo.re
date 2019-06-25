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

[@bs.deriving abstract]
type messageData = {
  op: int,
  d: Js.Json.t,
};

type message =
  | Hello(helloMessageData);

exception Unsupported;
external hackType: 'a => 'b = "%identity";
let parseMessage = messageData => {
  switch (Belt.Option.getExn(opCodeFromJs(opGet(messageData)))) {
  | Hello => Hello(hackType(dGet(messageData)))
  | _ => raise(Unsupported)
  };
};

let ws: Websocket.t(string) =
  Websocket.make("wss://gateway.discord.gg/?v=6&encoding=json");

Websocket.onOpen(ws, _ => Js.log("onOpen"));

[@bs.scope "JSON"] [@bs.val]
external parseIntoMessageData: string => messageData = "parse";
Websocket.onMessage(
  ws,
  ev => {
    let message = ev->MessageEvent.data->parseIntoMessageData->parseMessage;
    Js.log2("onMessage", message);
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