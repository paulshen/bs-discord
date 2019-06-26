open PayloadTypes;
open Types;

let channel = (json): channel => {
  Json.Decode.{
    id: json |> field("id", string),
    type_: json |> field("type", int),
    guildId: json |> optional(field("guild_id", string)),
    name: json |> optional(field("name", string)),
  };
};

let user = (json): user => {
  Json.Decode.{
    id: json |> field("id", string),
    username: json |> field("username", string),
    discriminator: json |> field("discriminator", string),
    avatar: json |> field("avatar", optional(string)),
    bot: json |> optional(field("bot", bool)),
    mfaEnabled: json |> optional(field("mfa_enabled", bool)),
    locale: json |> optional(field("locale", string)),
    verified: json |> optional(field("verified", bool)),
    email: json |> optional(field("email", string)),
    flags: json |> optional(field("flags", int)),
    premiumType: json |> optional(field("premium_type", int)),
  };
};

let guildMember = (json): guildMember => {
  Json.Decode.{
    user: json |> field("user", user),
    nick: json |> optional(field("nick", string)),
    roles: json |> field("roles", array(string)),
    joinedAt: json |> field("joined_at", date),
    premiumSince: json |> field("premium_since", optional(date)),
    deaf: json |> field("deaf", bool),
    mute: json |> field("mute", bool),
  };
};

let presenceUser = (json): presenceUser => {
  Json.Decode.{id: json |> field("id", string)};
};
let unwrapOption =
  fun
  | Some(Some(x)) => Some(x)
  | _ => None;
let activity = (json): activity => {
  Json.Decode.{
    name: json |> field("name", string),
    type_:
      json |> field("type", int) |> activityTypeFromJs |> Belt.Option.getExn,
    url: json |> optional(field("url", optional(string))) |> unwrapOption,
    applicationId: json |> optional(field("application_id", string)),
    flags: json |> optional(field("flags", int)),
  };
};
let clientStatus = (json): clientStatus => {
  Json.Decode.{
    desktop:
      (json |> optional(field("desktop", string)))
      ->Belt.Option.map(statusString =>
          Belt.Option.getExn(clientStatusTypeFromJs(statusString))
        ),
    mobile:
      (json |> optional(field("mobile", string)))
      ->Belt.Option.map(statusString =>
          Belt.Option.getExn(clientStatusTypeFromJs(statusString))
        ),
    web:
      (json |> optional(field("web", string)))
      ->Belt.Option.map(statusString =>
          Belt.Option.getExn(clientStatusTypeFromJs(statusString))
        ),
  };
};
let presenceUpdate = (json): presenceUpdate => {
  Json.Decode.{
    user: json |> field("user", presenceUser),
    roles: json |> optional(field("roles", array(string))),
    game: json |> optional(field("game", optional(activity))),
    guildId: json |> optional(field("guild_id", string)),
    status: json |> optional(field("status", string)),
    activities: json |> optional(field("activities", array(activity))),
    clientStatus: json |> optional(field("client_status", clientStatus)),
  };
};

let guild = (json): guild => {
  Json.Decode.{
    id: json |> field("id", string),
    name: json |> field("name", string),
    icon: json |> field("icon", optional(string)),
    splash: json |> field("splash", optional(string)),
    owner: json |> optional(field("owner", bool)),
    ownerId: json |> field("owner_id", string),
    members: json |> optional(field("members", array(guildMember))),
    channels: json |> optional(field("channels", array(channel))),
    presences: json |> optional(field("presences", array(presenceUpdate))),
  };
};

let unavailableGuild = (json): unavailableGuild => {
  Json.Decode.{
    id: json |> field("id", string),
    unavailable: json |> field("unavailable", bool),
  };
};

let message = (json): message => {
  Json.Decode.{
    id: json |> field("id", string),
    channelId: json |> field("channel_id", string),
    guildId: json |> field("guild_id", string),
    author: json |> field("author", user), /* TODO: handle webhook */
    content: json |> field("content", string),
    timestamp: json |> field("timestamp", date),
    editedTimestamp: json |> field("edited_timestamp", optional(date)),
    tts: json |> field("tts", bool),
    mentionEveryone: json |> field("mention_everyone", bool),
    type_: json |> field("type", int),
  };
};

let readyPayload = (json): readyPayload => {
  Json.Decode.{
    sessionId: json |> field("session_id", string),
    user: json |> field("user", user),
    guilds: json |> field("guilds", array(unavailableGuild)),
  };
};

let helloPayload = (json): helloPayload => {
  Json.Decode.{heartbeatInterval: field("heartbeat_interval", int, json)};
};

let resumePayload = (json): resumedPayload => {
  Json.Decode.{trace: field("_trace", array(string), json)};
};

let parseSocketData = json => {
  Json.Decode.(
    switch (Belt.Option.getExn(opCodeFromJs(field("op", int, json)))) {
    | Hello => Hello(json |> field("d", helloPayload))
    | Dispatch =>
      Dispatch(
        switch (dispatchMessageTypeFromJs(field("t", string, json))) {
        | Some(`Ready) => Ready(field("d", readyPayload, json))
        | Some(`GuildCreate) => GuildCreate(field("d", guild, json))
        | Some(`MessageCreate) => MessageCreate(field("d", message, json))
        | Some(`Resume) => Resume(field("d", resumePayload, json))
        | None => Unknown
        },
      )
    | InvalidSession => InvalidSession
    | _ => Unknown
    }
  );
};