// Generated by BUCKLESCRIPT VERSION 5.0.4, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Belt_Option = require("bs-platform/lib/js/belt_Option.js");
var Js_mapperRt = require("bs-platform/lib/js/js_mapperRt.js");
var Json_decode = require("@glennsl/bs-json/src/Json_decode.bs.js");
var Caml_exceptions = require("bs-platform/lib/js/caml_exceptions.js");
var WebsocketClient$BsDiscord = require("./WebsocketClient.bs.js");

var jsMapperConstantArray = /* array */[
  0,
  1,
  2,
  3,
  4,
  6,
  7,
  8,
  9,
  10,
  11
];

function opCodeToJs(param) {
  return Js_mapperRt.toInt(param, jsMapperConstantArray);
}

function opCodeFromJs(param) {
  return Js_mapperRt.fromInt(11, jsMapperConstantArray, param);
}

var jsMapperConstantArray$1 = /* array */[
  /* tuple */[
    -102231997,
    "READY"
  ],
  /* tuple */[
    889912559,
    "GUILD_CREATE"
  ],
  /* tuple */[
    975859715,
    "MESSAGE_CREATE"
  ]
];

function dispatchMessageTypeToJs(param) {
  return Js_mapperRt.binarySearch(3, param, jsMapperConstantArray$1);
}

function dispatchMessageTypeFromJs(param) {
  return Js_mapperRt.revSearch(3, jsMapperConstantArray$1, param);
}

var Unsupported = Caml_exceptions.create("Demo-BsDiscord.Unsupported");

function parseMessage(messageData) {
  var match = Belt_Option.getExn(opCodeFromJs(messageData.op));
  switch (match) {
    case 0 : 
        var match$1 = dispatchMessageTypeFromJs(Belt_Option.getExn(messageData.t));
        var tmp;
        if (match$1 !== undefined) {
          var dispatchMessageType = match$1;
          tmp = dispatchMessageType !== 889912559 ? (
              dispatchMessageType >= 975859715 ? /* MessageCreate */Block.__(2, [messageData.d]) : /* Ready */Block.__(0, [messageData.d])
            ) : /* GuildCreate */Block.__(1, [messageData.d]);
        } else {
          tmp = /* Unknown */0;
        }
        return /* Dispatch */Block.__(1, [tmp]);
    case 8 : 
        return /* InvalidSession */0;
    case 9 : 
        var json = messageData.d;
        return /* Hello */Block.__(0, [/* record */[/* heartbeatInterval */Json_decode.field("heartbeat_interval", Json_decode.$$int, json)]]);
    case 1 : 
    case 2 : 
    case 3 : 
    case 4 : 
    case 5 : 
    case 6 : 
    case 7 : 
    case 10 : 
        return /* Unknown */1;
    
  }
}

var ws = WebsocketClient$BsDiscord.Websocket[/* make */2](undefined, "wss://gateway.discord.gg/?v=6&encoding=json");

WebsocketClient$BsDiscord.Websocket[/* onOpen */4](ws, (function (param) {
        console.log("onOpen");
        return WebsocketClient$BsDiscord.Websocket[/* send */8](ws, JSON.stringify({
                        op: opCodeToJs(/* Identify */2),
                        d: {
                          token: "Mzk4OTE3OTQzNTc0MTM0Nzk1.XRKUnA.KNRkoqpdhZVMEvD3ti0abVECf-k",
                          properties: {
                            $os: "darwin",
                            $browser: "bs-discord",
                            $device: "bs-discord"
                          }
                        }
                      }));
      }));

function handleMessage(message) {
  if (typeof message === "number" || message.tag) {
    return /* () */0;
  } else {
    setInterval((function (param) {
            return WebsocketClient$BsDiscord.Websocket[/* send */8](ws, JSON.stringify({
                            op: opCodeToJs(/* Heartbeat */1)
                          }));
          }), message[0][/* heartbeatInterval */0]);
    return /* () */0;
  }
}

WebsocketClient$BsDiscord.Websocket[/* onMessage */7](ws, (function (ev) {
        console.log(ev.data);
        var message = parseMessage(JSON.parse(ev.data));
        console.log("onMessage", message);
        handleMessage(message);
        return /* () */0;
      }));

WebsocketClient$BsDiscord.Websocket[/* onError */5](ws, (function (ev) {
        console.log("onError: " + (String(ev) + ""));
        return WebsocketClient$BsDiscord.Websocket[/* close */3](undefined, undefined, ws);
      }));

WebsocketClient$BsDiscord.Websocket[/* onClose */6](ws, (function (ev) {
        console.log("onClose: " + (String(ev) + ""));
        return /* () */0;
      }));

exports.opCodeToJs = opCodeToJs;
exports.opCodeFromJs = opCodeFromJs;
exports.dispatchMessageTypeToJs = dispatchMessageTypeToJs;
exports.dispatchMessageTypeFromJs = dispatchMessageTypeFromJs;
exports.Unsupported = Unsupported;
exports.parseMessage = parseMessage;
exports.ws = ws;
exports.handleMessage = handleMessage;
/* ws Not a pure module */
