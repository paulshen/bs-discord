type snowflake = string;

type user = {
  id: snowflake,
  username: string,
  discriminator: string,
  avatar: option(string),
  bot: option(bool),
  mfaEnabled: option(bool),
  locale: option(string),
  verified: option(bool),
  email: option(string),
  flags: option(int),
  premiumType: option(int),
};

[@bs.deriving jsConverter]
type userConnectionVisibility =
  | [@bs.as 0] None
  | [@bs.as 1] Everyone;
type userConnection = {
  id: snowflake,
  name: string,
  type_: string,
  revoked: bool,
  /* todo: integrations */
  verified: bool,
  friendSync: bool,
  showActivity: bool,
  visibility: userConnectionVisibility,
};

type guildMember = {
  user,
  nick: option(string),
  roles: array(snowflake),
  joinedAt: Js.Date.t,
  premiumSince: option(Js.Date.t),
  deaf: bool,
  mute: bool,
};
type partialGuildMember = {
  user,
  nick: option(string),
  roles: option(array(snowflake)),
  joinedAt: option(Js.Date.t),
  premiumSince: option(option(Js.Date.t)),
  deaf: option(bool),
  mute: option(bool),
};

[@bs.deriving jsConverter]
type activityType =
  | [@bs.as 0] Game
  | [@bs.as 1] Streaming
  | [@bs.as 2] Listening;
type activity = {
  name: string,
  type_: activityType,
  url: option(string),
  applicationId: option(snowflake),
  flags: option(int),
  /* todo */
};

[@bs.deriving jsConverter]
type clientStatusType = [
  | [@bs.as "online"] `Online
  | [@bs.as "idle"] `Idle
  | [@bs.as "dnd"] `Dnd
];
type clientStatus = {
  desktop: option(clientStatusType),
  mobile: option(clientStatusType),
  web: option(clientStatusType),
};

[@bs.deriving jsConverter]
type status = [
  | [@bs.as "online"] `Online
  | [@bs.as "idle"] `Idle
  | [@bs.as "dnd"] `Dnd
  | [@bs.as "offline"] `Offline
];
type presenceUser = {id: snowflake};
type presence = {
  user: presenceUser,
  roles: array(snowflake),
  game: option(activity),
  guildId: snowflake,
  status,
  activities: array(activity),
  clientStatus,
};
type presenceUpdate = {
  user: presenceUser,
  roles: option(array(snowflake)),
  game: option(option(activity)),
  guildId: option(snowflake),
  status: option(status),
  activities: option(array(activity)),
  clientStatus: option(clientStatus),
};

[@bs.deriving jsConverter]
type channelType =
  | [@bs.as 0] GuildText
  | [@bs.as 1] DM
  | [@bs.as 2] GuildVoice
  | [@bs.as 3] GroupDM
  | [@bs.as 4] GuildCategory
  | [@bs.as 5] GuildNews
  | [@bs.as 6] GuildStore;
type channel = {
  id: snowflake,
  type_: channelType,
  guildId: option(snowflake),
  position: option(int),
  name: option(string),
  topic: option(string),
  nsfw: option(bool),
  lastMessageId: option(option(snowflake)),
  bitrate: option(int),
  userLimit: option(int),
  rateLimitPerUser: option(int),
  recipients: option(array(user)),
  icon: option(option(string)),
  ownerId: option(snowflake),
  applicationId: option(snowflake),
  parentId: option(option(snowflake)),
  lastPinTimestamp: option(Js.Date.t),
  /* todo: permission_overwrites */
};

type guild = {
  id: snowflake,
  name: string,
  icon: option(string),
  splash: option(string),
  owner: option(bool),
  ownerId: snowflake,
  members: option(array(guildMember)),
  channels: option(array(channel)),
  presences: option(array(presenceUpdate)),
  /* todo */
};

type unavailableGuild = {
  id: snowflake,
  unavailable: bool,
};

type emoji = {
  id: option(snowflake),
  name: string,
  user: option(user),
  requireColons: option(bool),
  managed: option(bool),
  animated: option(bool),
  /* todo: roles */
};

type messageReaction = {
  count: int,
  me: bool,
  emoji,
};

module Embed = {
  // https://discordapp.com/developers/docs/resources/channel#embed-object-embed-footer-structure
  type footer = {
    text: string,
    iconUrl: option(string),
    proxyIconUrl: option(string),
  };
  // https://discordapp.com/developers/docs/resources/channel#embed-object-embed-field-structure
  type field = {
    name: string,
    value: string,
    inline: option(bool),
  };
  // https://discordapp.com/developers/docs/resources/channel#embed-object-embed-image-structure
  type image = {
    url: option(string),
    proxyUrl: option(string),
    height: option(int),
    width: option(int),
  };
  // https://discordapp.com/developers/docs/resources/channel#embed-object-embed-thumbnail-structure
  type thumbnail = {
    url: option(string),
    proxyUrl: option(string),
    height: option(int),
    width: option(int),
  };
  // https://discordapp.com/developers/docs/resources/channel#embed-object-embed-video-structure
  type video = {
    url: option(string),
    height: option(int),
    width: option(int),
  };
  // https://discordapp.com/developers/docs/resources/channel#embed-object-embed-provider-structure
  type provider = {
    name: option(string),
    url: option(string),
  };
  // https://discordapp.com/developers/docs/resources/channel#embed-object-embed-author-structure
  type author = {
    name: option(string),
    url: option(string),
    iconUrl: option(string),
    proxyIconUrl: option(string),
  };

  // https://discordapp.com/developers/docs/resources/channel#embed-object
  type t = {
    title: option(string),
    type_: option(string),
    description: option(string),
    url: option(string),
    timestamp: option(Js.Date.t),
    color: option(int),
    footer: option(footer),
    image: option(image),
    thumbnail: option(thumbnail),
    video: option(video),
    provider: option(provider),
    author: option(author),
    fields: option(array(field)),
  };
};

[@bs.deriving jsConverter]
type messageType =
  | [@bs.as 0] Default
  | [@bs.as 1] RecipientAdd
  | [@bs.as 2] RecipientRemove
  | [@bs.as 3] Call
  | [@bs.as 4] ChannelNameChange
  | [@bs.as 5] ChannelIconChange
  | [@bs.as 6] ChannelPinnedMessage
  | [@bs.as 7] GuildMemberJoin
  | [@bs.as 8] UserPremiumGuildSubscription
  | [@bs.as 9] UserPremiumGuildSubscriptionTier1
  | [@bs.as 10] UserPremiumGuildSubscriptionTier2
  | [@bs.as 11] UserPremiumGuildSubscriptionTier3;
// https://discordapp.com/developers/docs/resources/channel#message-object-message-structure
type message = {
  id: snowflake,
  channelId: snowflake,
  guildId: option(snowflake),
  author: user, /* todo: handle webhook */
  member: option(partialGuildMember),
  content: string,
  timestamp: Js.Date.t,
  editedTimestamp: option(Js.Date.t),
  tts: bool,
  mentionEveryone: bool,
  mentionRoles: array(snowflake),
  embeds: array(Embed.t),
  reactions: option(array(messageReaction)),
  nonce: option(option(snowflake)),
  pinned: bool,
  webhookId: option(snowflake),
  type_: messageType,
  /* todo: mentions, attachments, embeds, activity, application */
};