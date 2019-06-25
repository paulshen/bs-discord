type snowflake = string;

[@bs.deriving abstract]
type user = {
  id: snowflake,
  username: string,
  discriminator: string,
  avatar: option(string),
  [@bs.optional]
  bot: bool,
  [@bs.optional] [@bs.as "mfa_enabled"]
  mfaEnabled: bool,
  [@bs.optional]
  locale: string,
  [@bs.optional]
  verified: bool,
  [@bs.optional]
  email: string,
  [@bs.optional]
  flags: int,
  [@bs.optional] [@bs.as "premium_type"]
  premiumType: int,
};

[@bs.deriving abstract]
type guildMember = {
  user,
  [@bs.optional]
  nick: string,
  roles: array(snowflake),
};

[@bs.deriving abstract]
type channel = {
  id: snowflake,
  [@bs.as "type"]
  type_: int,
  [@bs.optional] [@bs.as "guild_id"]
  guildId: snowflake,
  [@bs.optional]
  name: string,
};

[@bs.deriving abstract]
type guild = {
  id: snowflake,
  name: string,
  icon: option(string),
  splash: option(string),
  [@bs.optional]
  owner: bool,
  owner_id: string,
  [@bs.optional]
  members: array(guildMember),
  [@bs.optional]
  channels: array(channel),
};

[@bs.deriving abstract]
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

[@bs.deriving abstract]
type message = {
  id: snowflake,
  [@bs.as "channel_id"]
  channelId: snowflake,
  [@bs.as "guild_id"]
  guildId: snowflake,
  author: user, /* TODO: handle webhook */
  content: string,
  timestamp: string,
  edited_timestamp: option(string),
  tts: bool,
  mention_everyone: bool,
  [@bs.as "type"]
  type_: int,
};