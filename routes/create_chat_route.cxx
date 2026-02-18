#include "create_chat_route.hxx"
#include "commands/new_chat.hxx"
#include "error.hxx"

std::unique_ptr<http::Response> CreateChatRoute::handleRequest(const http::Request& req)
{
  const std::string username = context.auth.authorize(req);
  const int userId = userRepository.getUserId(username);
  UserFields user{
    .username = username,
    .id = userId,
  };
  
  Chat chat = loadChatData(nlohmann::json::parse(req.body()));
  chat.usersId[userId] = user; 

  addUsersToChat(chat);

  for (auto [id, user] : chat.usersId)
    spdlog::info("user: {}", id);
  context.ws.dispatch(std::make_unique<NewChatCommand>(chat));

  return buildRouteResponse(chat.chatId);
}

std::unique_ptr<http::Response> CreateChatRoute::buildRouteResponse(const int chatId)
{
  std::unique_ptr<http::CoreResponse> res = std::make_unique<http::CoreResponse>();
  res->setCode(200);
  res->setBody(
    fmt::format(R"({{"chat_id":"{}"}})", std::to_string(chatId))
  );

  return std::move(res);
}

Chat CreateChatRoute::loadChatData(const nlohmann::json& json)
{
  if (!json.contains("name"))
    throw JsonException("Malformed Json");

  const std::string chatName = json["name"];
  const std::string username = json["name"];

  const int userId = userRepository.getUserId(username);

  UserFields secondUser {
    .username = username,
    .id = userId
  };
  secondUser.username = username;
  secondUser.id = userId;

  Chat chat;
  chat.chatName = chatName;
  chat.usersId[userId] = secondUser;
  chat.chatId = chatRepository.createChat(chat);

  return chat;
}

void CreateChatRoute::addUsersToChat(const Chat& chat)
{
  for (const auto& [id, user] : chat.usersId)
    chatRepository.insertUserToChat(id, chat.chatId);
}
