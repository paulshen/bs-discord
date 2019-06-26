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
];

type readyPayload = {
  sessionId: string,
  user,
  guilds: array(unavailableGuild),
};

type dispatchMessage =
  | Ready(readyPayload)
  | GuildCreate(guild)
  | MessageCreate(message)
  | Unknown;

type socketMessage =
  | Hello(helloPayload)
  | Dispatch(dispatchMessage)
  | InvalidSession
  | Unknown;