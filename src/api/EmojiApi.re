open Types;

let getGuildEmojis = (guildId: snowflake) => {
  Js.Promise.(
    Api.requestGet({j|/guilds/$guildId/emojis|j}, ())
    |> then_(json =>
         json |> Json.Decode.array(PayloadParser.emoji) |> resolve
       )
  );
};

let getGuildEmoji = (guildId: snowflake, emojiId: snowflake) => {
  Js.Promise.(
    Api.requestGet({j|/guilds/$guildId/emojis/$emojiId|j}, ())
    |> then_(json => json |> PayloadParser.emoji |> resolve)
  );
};