// Generated by BUCKLESCRIPT VERSION 5.0.4, PLEASE EDIT WITH CARE
'use strict';

var $$Array = require("bs-platform/lib/js/array.js");
var Fetch = require("bs-fetch/src/Fetch.js");
var Belt_Option = require("bs-platform/lib/js/belt_Option.js");
var Caml_exceptions = require("bs-platform/lib/js/caml_exceptions.js");
var Constants$BsDiscord = require("../Constants.bs.js");

function urlWithQueryParams(url, params) {
  var encodeParam = function (param) {
    return encodeURIComponent(param[0]) + ("=" + encodeURIComponent(param[1]));
  };
  var params$1 = $$Array.map(encodeParam, params).join("&");
  if (params$1 === "") {
    return url;
  } else {
    return "" + (String(url) + ("?" + (String(params$1) + "")));
  }
}

var ApiError = Caml_exceptions.create("Api-BsDiscord.ApiError");

function requestGet(url, queryParams, param) {
  var endpoint = Constants$BsDiscord.apiBaseUrl + url;
  return fetch(queryParams !== undefined ? urlWithQueryParams(endpoint, queryParams) : endpoint, Fetch.RequestInit[/* make */0](/* Get */0, {
                        "Content-Type": "application/json",
                        Authorization: "Bot " + (String(Constants$BsDiscord.token) + "")
                      }, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined)(/* () */0)).then((function (response) {
                  var statusCode = response.status;
                  var switcher = statusCode - 200 | 0;
                  if (switcher > 4 || switcher < 0) {
                    if (switcher !== 104) {
                      return Promise.reject([
                                  ApiError,
                                  statusCode
                                ]);
                    } else {
                      return Promise.resolve(response);
                    }
                  } else if (switcher === 3 || switcher === 2) {
                    return Promise.reject([
                                ApiError,
                                statusCode
                              ]);
                  } else {
                    return Promise.resolve(response);
                  }
                })).then((function (prim) {
                return prim.json();
              }));
}

function requestPost(url, bodyJson, param) {
  return fetch(Constants$BsDiscord.apiBaseUrl + url, Fetch.RequestInit[/* make */0](/* Post */2, {
                        "Content-Type": "application/json",
                        Authorization: "Bot " + (String(Constants$BsDiscord.token) + "")
                      }, Belt_Option.map(bodyJson, (function (bodyJson) {
                              return JSON.stringify(bodyJson);
                            })), undefined, undefined, undefined, undefined, undefined, undefined, undefined, undefined)(/* () */0)).then((function (response) {
                  var statusCode = response.status;
                  var switcher = statusCode - 200 | 0;
                  if (switcher > 4 || switcher < 0) {
                    if (switcher !== 104) {
                      return Promise.reject([
                                  ApiError,
                                  statusCode
                                ]);
                    } else {
                      return Promise.resolve(response);
                    }
                  } else if (switcher === 3 || switcher === 2) {
                    return Promise.reject([
                                ApiError,
                                statusCode
                              ]);
                  } else {
                    return Promise.resolve(response);
                  }
                })).then((function (prim) {
                return prim.json();
              }));
}

exports.urlWithQueryParams = urlWithQueryParams;
exports.ApiError = ApiError;
exports.requestGet = requestGet;
exports.requestPost = requestPost;
/* No side effect */
