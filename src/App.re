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
      Js.log2(
        "patch",
        ChannelApi.updateChannel(message.channelId, ~name="Hello", ()),
      );
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