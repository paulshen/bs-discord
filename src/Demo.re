open WebsocketClient;

let ws: Websocket.t(string) =
  Websocket.make("wss://gateway.discord.gg/?v=6&encoding=json");

Websocket.onOpen(ws, _ => Js.log("onOpen"));
Websocket.onMessage(ws, ev => Js.log2("onMessage", ev));
Websocket.onError(
  ws,
  ev => {
    Js.log({j|onError: $ev|j});
    Websocket.close(ws);
  },
);
Websocket.onClose(ws, ev => Js.log({j|onClose: $ev|j}));