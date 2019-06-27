open PayloadTypes;
open Types;

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

let userConnection = (json): userConnection => {
  Json.Decode.{
    id: json |> field("id", string),
    name: json |> field("name", string),
    type_: json |> field("type", string),
    revoked: json |> field("revoked", bool),
    verified: json |> field("revoked", bool),
    friendSync: json |> field("friend_sync", bool),
    showActivity: json |> field("show_activity", bool),
    visibility:
      json
      |> field("visibility", int)
      |> userConnectionVisibilityFromJs
      |> Belt.Option.getExn,
  };
};

let channel = (json): channel => {
  Json.Decode.{
    id: json |> field("id", string),
    type_:
      json |> field("type", int) |> channelTypeFromJs |> Belt.Option.getExn,
    guildId: json |> optional(field("guild_id", string)),
    position: json |> optional(field("position", int)),
    name: json |> optional(field("name", string)),
    topic: json |> optional(field("topic", string)),
    nsfw: json |> optional(field("nsfw", bool)),
    lastMessageId:
      json |> optional(field("lastMessageId", optional(string))),
    bitrate: json |> optional(field("bitrate", int)),
    userLimit: json |> optional(field("userLimit", int)),
    rateLimitPerUser: json |> optional(field("rateLimitPerUser", int)),
    recipients: json |> optional(field("recipients", array(user))),
    icon: json |> optional(field("icon", optional(string))),
    ownerId: json |> optional(field("ownerId", string)),
    applicationId: json |> optional(field("applicationId", string)),
    parentId: json |> optional(field("parentId", optional(string))),
    lastPinTimestamp: json |> optional(field("lastPinTimestamp", date)),
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
let partialGuildMember = (json): partialGuildMember => {
  Json.Decode.{
    user: json |> field("user", user),
    nick: json |> optional(field("nick", string)),
    roles: json |> optional(field("roles", array(string))),
    joinedAt: json |> optional(field("joined_at", date)),
    premiumSince: json |> optional(field("premium_since", optional(date))),
    deaf: json |> optional(field("deaf", bool)),
    mute: json |> optional(field("mute", bool)),
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
    status:
      (json |> optional(field("status", string)))
      ->Belt.Option.map(statusString =>
          Belt.Option.getExn(statusFromJs(statusString))
        ),
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

let emoji = (json): emoji => {
  Json.Decode.{
    id: json |> field("id", optional(string)),
    name: json |> field("name", string),
    user: json |> optional(field("user", user)),
    requireColons: json |> optional(field("require_colons", bool)),
    managed: json |> optional(field("managed", bool)),
    animated: json |> optional(field("animated", bool)),
  };
};

let messageReaction = (json): messageReaction => {
  Json.Decode.{
    count: json |> field("count", int),
    me: json |> field("me", bool),
    emoji: json |> field("emoji", emoji),
  };
};

module Embed = {
  let footer = (json): Embed.footer => {
    Json.Decode.{
      text: json |> field("text", string),
      iconUrl: json |> optional(field("icon_url", string)),
      proxyIconUrl: json |> optional(field("proxy_icon_url", string)),
    };
  };

  let image = (json): Embed.image => {
    Json.Decode.{
      url: json |> optional(field("url", string)),
      proxyUrl: json |> optional(field("proxy_url", string)),
      height: json |> optional(field("height", int)),
      width: json |> optional(field("width", int)),
    };
  };

  let thumbnail = (json): Embed.thumbnail => {
    Json.Decode.{
      url: json |> optional(field("url", string)),
      proxyUrl: json |> optional(field("proxy_url", string)),
      height: json |> optional(field("height", int)),
      width: json |> optional(field("width", int)),
    };
  };

  let video = (json): Embed.video => {
    Json.Decode.{
      url: json |> optional(field("url", string)),
      height: json |> optional(field("height", int)),
      width: json |> optional(field("width", int)),
    };
  };

  let provider = (json): Embed.provider => {
    Json.Decode.{
      name: json |> optional(field("name", string)),
      url: json |> optional(field("url", string)),
    };
  };

  let author = (json): Embed.author => {
    Json.Decode.{
      name: json |> optional(field("name", string)),
      url: json |> optional(field("url", string)),
      iconUrl: json |> optional(field("icon_url", string)),
      proxyIconUrl: json |> optional(field("proxy_icon_url", string)),
    };
  };

  let field = (json): Embed.field => {
    Json.Decode.{
      name: json |> field("name", string),
      value: json |> field("value", string),
      inline: json |> optional(field("inline", bool)),
    };
  };
};

let embed = (json): Types.Embed.t => {
  Json.Decode.{
    title: json |> optional(field("title", string)),
    type_: json |> optional(field("type", string)),
    description: json |> optional(field("description", string)),
    url: json |> optional(field("url", string)),
    timestamp: json |> optional(field("timestamp", date)),
    color: json |> optional(field("color", int)),
    footer: json |> optional(field("footer", Embed.footer)),
    image: json |> optional(field("image", Embed.image)),
    thumbnail: json |> optional(field("thumbnail", Embed.thumbnail)),
    video: json |> optional(field("video", Embed.video)),
    provider: json |> optional(field("provider", Embed.provider)),
    author: json |> optional(field("author", Embed.author)),
    fields: json |> optional(field("fields", array(Embed.field))),
  };
};

let messageAttachment = (json): messageAttachment => {
  Json.Decode.{
    id: json |> field("id", string),
    filename: json |> field("filename", string),
    size: json |> field("size", int),
    url: json |> field("url", string),
    proxyUrl: json |> field("proxy_url", string),
    height: json |> field("height", optional(int)),
    width: json |> field("width", optional(int)),
  };
};

let messageActivity = (json): messageActivity => {
  Json.Decode.{
    type_:
      json
      |> field("type", int)
      |> messageActivityTypeFromJs
      |> Belt.Option.getExn,
    partyId: json |> optional(field("party_id", string)),
  };
};

let messageApplication = (json): messageApplication => {
  Json.Decode.{
    id: json |> field("id", string),
    coverImage: json |> optional(field("cover_image", string)),
    description: json |> field("description", string),
    icon: json |> optional(field("icon", string)),
    name: json |> field("name", string),
  };
};

let message = (json): message => {
  Json.Decode.{
    id: json |> field("id", string),
    channelId: json |> field("channel_id", string),
    guildId: json |> optional(field("guild_id", string)),
    author: json |> field("author", user), /* TODO: handle webhook */
    member: json |> optional(field("member", partialGuildMember)),
    content: json |> field("content", string),
    timestamp: json |> field("timestamp", date),
    editedTimestamp: json |> field("edited_timestamp", optional(date)),
    tts: json |> field("tts", bool),
    mentionEveryone: json |> field("mention_everyone", bool),
    mentionRoles: json |> field("mention_roles", array(string)),
    embeds: json |> field("embeds", array(embed)),
    attachments: json |> field("attachments", array(messageAttachment)),
    reactions: json |> optional(field("reactions", array(messageReaction))),
    nonce: json |> optional(field("nonce", optional(string))),
    pinned: json |> field("pinned", bool),
    webhookId: json |> optional(field("webhookId", string)),
    type_:
      json |> field("type", int) |> messageTypeFromJs |> Belt.Option.getExn,
    activity: json |> field("activity", optional(messageActivity)),
    application: json |> field("application", optional(messageApplication)),
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

let messageReactionAddPayload = (json): messageReactionAddPayload => {
  Json.Decode.{
    userId: json |> field("user_id", string),
    channelId: json |> field("channel_id", string),
    messageId: json |> field("message_id", string),
    guildId: json |> optional(field("guild_id", string)),
    emoji: json |> field("emoji", emoji),
  };
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
        | Some(`MessageReactionAdd) =>
          MessageReactionAdd(field("d", messageReactionAddPayload, json))
        | Some(`PresenceUpdate) =>
          PresenceUpdate(field("d", presenceUpdate, json))
        | Some(`Resume) => Resume(field("d", resumePayload, json))
        | None => Unknown
        },
      )
    | InvalidSession => InvalidSession
    | _ => Unknown
    }
  );
};