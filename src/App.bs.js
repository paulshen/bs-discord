// Generated by BUCKLESCRIPT VERSION 5.0.4, PLEASE EDIT WITH CARE
'use strict';

var Gateway$BsDiscord = require("./Gateway.bs.js");
var EmojiApi$BsDiscord = require("./api/EmojiApi.bs.js");
var Constants$BsDiscord = require("./Constants.bs.js");
var ChannelApi$BsDiscord = require("./api/ChannelApi.bs.js");

((require('isomorphic-fetch')));

function onOpen(e) {
  console.log("onOpen", e);
  return /* () */0;
}

function onMessage(message) {
  console.log("onMessage", message);
  if (typeof message === "number" || message.tag !== 1) {
    return /* () */0;
  } else {
    var match = message[0];
    if (typeof match === "number" || match.tag !== 2) {
      return /* () */0;
    } else {
      var message$1 = match[0];
      if ("ping".indexOf(message$1[/* content */5]) !== -1) {
        ChannelApi$BsDiscord.createMessage(message$1[/* channelId */1], "pong");
      }
      if ("patch".indexOf(message$1[/* content */5]) !== -1) {
        ChannelApi$BsDiscord.updateChannel(message$1[/* channelId */1], "Hello", undefined, undefined, undefined, undefined, undefined, undefined, undefined, /* () */0).then((function (channel) {
                return Promise.resolve((console.log("patch", channel), /* () */0));
              }));
      }
      if ("delete".indexOf(message$1[/* content */5]) !== -1) {
        ChannelApi$BsDiscord.deleteChannel(message$1[/* channelId */1]).then((function (channel) {
                return Promise.resolve((console.log("delete", channel), /* () */0));
              }));
      }
      if ("get".indexOf(message$1[/* content */5]) !== -1) {
        ChannelApi$BsDiscord.getMessage(message$1[/* channelId */1], message$1[/* id */0]).then((function (message) {
                return Promise.resolve((console.log("get", message), /* () */0));
              }));
      }
      if ("react".indexOf(message$1[/* content */5]) !== -1) {
        ChannelApi$BsDiscord.addReaction(message$1[/* channelId */1], message$1[/* id */0], "tester", "593616291546267658").then((function (param) {
                return Promise.resolve((function (param) {
                              console.log("react", param);
                              return /* () */0;
                            }));
              }));
      }
      if ("getGuildEmojis".indexOf(message$1[/* content */5]) !== -1) {
        EmojiApi$BsDiscord.getGuildEmojis("467125609773006859").then((function (emojis) {
                return Promise.resolve((console.log("getGuildEmojis", emojis), /* () */0));
              }));
        return /* () */0;
      } else {
        return 0;
      }
    }
  }
}

function onError(e) {
  console.log("onError", e);
  return /* () */0;
}

function onClose(e) {
  console.log("onClose", e);
  return /* () */0;
}

var state = Gateway$BsDiscord.createSocket(Constants$BsDiscord.token, onOpen, onMessage, onError, onClose, /* () */0);

exports.onOpen = onOpen;
exports.onMessage = onMessage;
exports.onError = onError;
exports.onClose = onClose;
exports.state = state;
/*  Not a pure module */
