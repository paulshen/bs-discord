open Types;

let presences: ref(Belt.Map.String.t(presenceUpdate)) =
  ref(Belt.Map.String.empty);

let getOptionalUpdate = (value: option('a), default: option('a)) =>
  if (Belt.Option.isSome(value)) {
    value;
  } else {
    default;
  };
let updatePresence = presenceUpdate => {
  let userId = presenceUpdate.user.id;
  open Belt.Map.String;
  let newPresence =
    switch ((presences^)->get(userId)) {
    | Some(presence) => {
        user: presenceUpdate.user,
        roles: getOptionalUpdate(presenceUpdate.roles, presence.roles),
        game: getOptionalUpdate(presenceUpdate.game, presence.game),
        guildId: getOptionalUpdate(presenceUpdate.guildId, presence.guildId),
        status: getOptionalUpdate(presenceUpdate.status, presence.status),
        activities:
          getOptionalUpdate(presenceUpdate.activities, presence.activities),
        clientStatus:
          getOptionalUpdate(
            presenceUpdate.clientStatus,
            presence.clientStatus,
          ),
      }
    | None => presenceUpdate
    };
  presences := (presences^)->Belt.Map.String.set(userId, newPresence);
  Js.log2("updatePresence", presences^);
};

let updatePresences = presenceUpdates => {
  Array.iter(updatePresence, presenceUpdates);
  Js.log2("updatePresences", presences^);
};