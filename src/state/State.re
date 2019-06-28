open WebsocketClient;
open Types;

type gatewayState = {
  sessionId: ref(option(string)),
  lastSequenceId: ref(option(int)),
  heartbeatInterval: ref(option(Js.Global.intervalId)),
};

type presenceState = ref(Belt.Map.String.t(presenceUpdate));

type t = {
  ws: ref(option(Websocket.t(string))),
  gateway: gatewayState,
  presences: presenceState,
};