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