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
  reactions: option(array(messageReaction)),
  nonce: option(option(snowflake)),
  pinned: bool,
  webhookId: option(snowflake),
  type_: messageType,
  /* todo: mentions, attachments, embeds, activity, application */
};