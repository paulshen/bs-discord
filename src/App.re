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
      ChannelApi.createMessage(message.channelId, ~content="pong", ())
      |> ignore;
    };
    if (Js.String.indexOf(message.content, "patchChannel") != (-1)) {
      Js.Promise.(
        ChannelApi.updateChannel(message.channelId, ~name="hello", ())
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
    if (Js.String.indexOf(message.content, "react") != (-1)) {
      Js.Promise.(
        ChannelApi.addReaction(
          message.channelId,
          message.id,
          ~emojiName="tester",
          ~emojiId="593616291546267658",
        )
        |> then_(() => Js.log2("react") |> resolve)
      )
      |> ignore;
    };
    if (Js.String.indexOf(message.content, "getGuildEmojis") != (-1)) {
      Js.Promise.(
        EmojiApi.getGuildEmojis("467125609773006859")
        |> then_(emojis => Js.log2("getGuildEmojis", emojis) |> resolve)
      )
      |> ignore;
    };
    if (Js.String.indexOf(message.content, "me") != (-1)) {
      Js.Promise.(
        UserApi.getCurrentUser()
        |> then_(user => Js.log2("getCurrentUser", user) |> resolve)
      )
      |> ignore;
    };
    if (Js.String.indexOf(message.content, "patchMe") != (-1)) {
      Js.Promise.(
        UserApi.updateCurrentUser(~username="rename-bot", ())
        |> then_(user => Js.log2("patchMe", user) |> resolve)
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