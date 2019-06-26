// Generated by BUCKLESCRIPT VERSION 5.0.4, PLEASE EDIT WITH CARE
'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var Json_decode = require("@glennsl/bs-json/src/Json_decode.bs.js");
var Caml_exceptions = require("bs-platform/lib/js/caml_exceptions.js");
var ChannelApi$BsDiscord = require("./api/ChannelApi.bs.js");
var PayloadTypes$BsDiscord = require("./PayloadTypes.bs.js");
var PayloadParser$BsDiscord = require("./PayloadParser.bs.js");
var PresenceStore$BsDiscord = require("./state/PresenceStore.bs.js");
var WebsocketClient$BsDiscord = require("./WebsocketClient.bs.js");

var Unsupported = Caml_exceptions.create("Gateway-BsDiscord.Unsupported");

function identify(param, token) {
  return WebsocketClient$BsDiscord.Websocket[/* send */8](param[/* ws */0], JSON.stringify({
                  op: PayloadTypes$BsDiscord.opCodeToJs(/* Identify */2),
                  d: {
                    token: token,
                    properties: {
                      $os: "darwin",
                      $browser: "bs-discord",
                      $device: "bs-discord"
                    }
                  }
                }));
}

function resume(param, token) {
  var match = param[/* lastSequenceId */2][0];
  return WebsocketClient$BsDiscord.Websocket[/* send */8](param[/* ws */0], JSON.stringify({
                  op: PayloadTypes$BsDiscord.opCodeToJs(/* Resume */5),
                  d: {
                    token: token,
                    session_id: param[/* sessionId */1],
                    seq: match !== undefined ? match : null
                  }
                }));
}

function startHeartbeat(ws, lastSequenceId, payload) {
  setInterval((function (param) {
          console.log("heartbeat");
          var match = lastSequenceId[0];
          return WebsocketClient$BsDiscord.Websocket[/* send */8](ws, JSON.stringify({
                          op: PayloadTypes$BsDiscord.opCodeToJs(/* Heartbeat */1),
                          d: match !== undefined ? Caml_option.valFromOption(match) : null
                        }));
        }), payload[/* heartbeatInterval */0]);
  return /* () */0;
}

function handleMessage(state, message) {
  var match = state[/* gateway */0];
  if (typeof message === "number") {
    return /* () */0;
  } else if (message.tag) {
    var match$1 = message[0];
    if (typeof match$1 === "number") {
      return /* () */0;
    } else {
      switch (match$1.tag | 0) {
        case 0 : 
            match[/* sessionId */1][0] = match$1[0][/* sessionId */0];
            return /* () */0;
        case 1 : 
            var match$2 = match$1[0][/* presences */8];
            if (match$2 !== undefined) {
              return PresenceStore$BsDiscord.updatePresences(state[/* presences */1], match$2);
            } else {
              return /* () */0;
            }
        case 2 : 
            var message$1 = match$1[0];
            if ("ping".indexOf(message$1[/* content */4]) !== -1) {
              ChannelApi$BsDiscord.createMessage(message$1[/* channelId */1], "pong");
              return /* () */0;
            } else {
              return 0;
            }
        case 3 : 
            return PresenceStore$BsDiscord.updatePresence(state[/* presences */1], match$1[0]);
        case 4 : 
            return /* () */0;
        
      }
    }
  } else {
    return startHeartbeat(match[/* ws */0], match[/* lastSequenceId */2], message[0]);
  }
}

function createSocket(token, onOpen, onMessage, onError, onClose, param) {
  var gatewayState_000 = /* ws */WebsocketClient$BsDiscord.Websocket[/* make */2](undefined, "wss://gateway.discord.gg/?v=6&encoding=json");
  var gatewayState_001 = /* sessionId : record */[/* contents */undefined];
  var gatewayState_002 = /* lastSequenceId : record */[/* contents */undefined];
  var gatewayState = /* record */[
    gatewayState_000,
    gatewayState_001,
    gatewayState_002
  ];
  var state_001 = /* presences */PresenceStore$BsDiscord.getInitialState(/* () */0);
  var state = /* record */[
    /* gateway */gatewayState,
    state_001
  ];
  WebsocketClient$BsDiscord.Websocket[/* onOpen */4](gatewayState_000, (function (e) {
          if (onOpen !== undefined) {
            Curry._1(onOpen, e);
          }
          var match = gatewayState_001[0];
          if (match !== undefined) {
            return resume(gatewayState, token);
          } else {
            return identify(gatewayState, token);
          }
        }));
  WebsocketClient$BsDiscord.Websocket[/* onMessage */7](gatewayState_000, (function (e) {
          console.log(e.data);
          var json = JSON.parse(e.data);
          var match = Json_decode.field("s", (function (param) {
                  return Json_decode.optional(Json_decode.$$int, param);
                }), json);
          if (match !== undefined) {
            gatewayState_002[0] = match;
          }
          var message = PayloadParser$BsDiscord.parseSocketData(json);
          handleMessage(state, message);
          if (onMessage !== undefined) {
            Curry._1(onMessage, message);
          }
          return /* () */0;
        }));
  WebsocketClient$BsDiscord.Websocket[/* onError */5](gatewayState_000, (function (e) {
          if (onError !== undefined) {
            Curry._1(onError, e);
          }
          return WebsocketClient$BsDiscord.Websocket[/* close */3](undefined, undefined, gatewayState_000);
        }));
  WebsocketClient$BsDiscord.Websocket[/* onClose */6](gatewayState_000, (function (e) {
          if (onClose !== undefined) {
            return Curry._1(onClose, e);
          } else {
            return /* () */0;
          }
        }));
  return state;
}

exports.Unsupported = Unsupported;
exports.identify = identify;
exports.resume = resume;
exports.startHeartbeat = startHeartbeat;
exports.handleMessage = handleMessage;
exports.createSocket = createSocket;
/* WebsocketClient-BsDiscord Not a pure module */
