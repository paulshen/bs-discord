open WebsocketClient;
open Types;

type gatewayState = {
  ws: Websocket.t(string),
  sessionId: ref(option(string)),
  lastSequenceId: ref(option(int)),
};

type presenceState = ref(Belt.Map.String.t(presenceUpdate));

type t = {
  gateway: gatewayState,
  presences: presenceState,
};