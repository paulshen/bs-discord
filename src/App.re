[%raw "require('isomorphic-fetch')"];

open PayloadTypes;

let onOpen = e => {
  Js.log2("onOpen", e);
};
let onMessage = message => {
  Js.log2("onMessage", message);
  switch (message) {
  | Dispatch(MessageCreate(message)) =>
    if (Js.String.indexOf(message.content, "ping") != (-1)) {
      ChannelApi.createMessage(message.channelId, "pong") |> ignore;
    };
    if (Js.String.indexOf(message.content, "patch") != (-1)) {
      Js.Promise.(
        ChannelApi.updateChannel(message.channelId, ~name="Hello", ())
        |> then_(channel => Js.log2("patch", channel) |> resolve)
      )
      |> ignore;
    };
    if (Js.String.indexOf(message.content, "delete") != (-1)) {
      Js.Promise.(
        ChannelApi.deleteChannel(message.channelId)
        |> then_(channel => Js.log2("delete", channel) |> resolve)
      )
      |> ignore;
    };
    if (Js.String.indexOf(message.content, "get") != (-1)) {
      Js.Promise.(
        ChannelApi.getMessage(message.channelId, message.id)
        |> then_(message => Js.log2("get", message) |> resolve)
      )
      |> ignore;
    };
  | _ => ()
  };
};
let onError = e => {
  Js.log2("onError", e);
};
let onClose = e => {
  Js.log2("onClose", e);
};
let state =
  Gateway.createSocket(
    ~token=Constants.token,
    ~onOpen,
    ~onMessage,
    ~onError,
    ~onClose,
    (),
  );