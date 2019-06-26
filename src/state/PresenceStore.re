open State;
open Types;

let getInitialState = () => ref(Belt.Map.String.empty);

let getOptionalUpdate = (value: option('a), default: option('a)) =>
  if (Belt.Option.isSome(value)) {
    value;
  } else {
    default;
  };

let updatePresence = (state: State.presenceState, presenceUpdate) => {
  let userId = presenceUpdate.user.id;
  open Belt.Map.String;
  let newPresence =
    switch ((state^)->get(userId)) {
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
  state := (state^)->Belt.Map.String.set(userId, newPresence);
};

let updatePresences = (state, presenceUpdates) => {
  Array.iter(
    presenceUpdate => state->updatePresence(presenceUpdate),
    presenceUpdates,
  );
};