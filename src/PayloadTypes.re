open Types;

[@bs.deriving jsConverter]
type opCode =
  | [@bs.as 0] Dispatch
  | [@bs.as 1] Heartbeat
  | [@bs.as 2] Identify
  | [@bs.as 3] StatusUpdate
  | [@bs.as 4] VoiceStateUpdate
  | [@bs.as 6] Resume
  | [@bs.as 7] Reconnect
  | [@bs.as 8] RequestGuildMembers
  | [@bs.as 9] InvalidSession
  | [@bs.as 10] Hello
  | [@bs.as 11] HeartbeatAck;

type helloPayload = {heartbeatInterval: int};

[@bs.deriving jsConverter]
type dispatchMessageType = [
  | [@bs.as "READY"] `Ready
  | [@bs.as "GUILD_CREATE"] `GuildCreate
  | [@bs.as "MESSAGE_CREATE"] `MessageCreate
  | [@bs.as "PRESENCE_UPDATE"] `PresenceUpdate
  | [@bs.as "RESUMED"] `Resume
];

type readyPayload = {
  sessionId: string,
  user,
  guilds: array(unavailableGuild),
  /* todo */
};

type resumedPayload = {trace: array(string)};

type dispatchMessage =
  | Ready(readyPayload)
  | GuildCreate(guild)
  | MessageCreate(message)
  | PresenceUpdate(presenceUpdate)
  | Resume(resumedPayload)
  | Unknown;

type socketMessage =
  | Hello(helloPayload)
  | Dispatch(dispatchMessage)
  | InvalidSession
  | Unknown;