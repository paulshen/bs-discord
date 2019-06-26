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

type presenceUser = {id: snowflake};
type presence = {
  user: presenceUser,
  roles: array(snowflake),
  game: option(activity),
  guildId: snowflake,
  status: string,
  activities: array(activity),
  clientStatus,
};
type presenceUpdate = {
  user: presenceUser,
  roles: option(array(snowflake)),
  game: option(activity),
  guildId: option(snowflake),
  status: option(string),
  activities: option(array(activity)),
  clientStatus: option(clientStatus),
};

type channel = {
  id: snowflake,
  type_: int,
  guildId: option(snowflake),
  name: option(string),
  /* todo */
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
  guildId: snowflake,
  author: user,
  content: string,
  timestamp: Js.Date.t,
  editedTimestamp: option(Js.Date.t),
  tts: bool,
  mentionEveryone: bool,
  type_: int,
  /* todo */
};