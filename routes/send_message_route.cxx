#include "send_message_route.hxx"
#include "commands/new_message.hxx"
#include "error.hxx"

std::unique_ptr<http::Response> SendMessageRoute::handleRequest(const http::Request& req)
{
  const std::string username = context.auth.authorize(req);
  const int userId = userRepository.getUserId(username);

  Message message = loadMessageData(nlohmann::json::parse(req.body()));

  chatRepository.ensureUserInChat(userId, message.chatId);

  message.username = username;
  message.senderId = userId;
  message.messageId = messageRepository.sendMessage(message);

  context.ws.dispatch(std::make_unique<NewMessageCommand>(message));

  return buildRouteResponse(message.messageId);
}


Message SendMessageRoute::loadMessageData(const nlohmann::json& json)
{
  if (!json.contains("content") || !json.contains("chat_id"))
    throw JsonException("Malformed json");

  Message message {
    .content = json["content"],
    .chatId = json["chat_id"]
  };

  return message;
}

std::unique_ptr<http::Response> SendMessageRoute::buildRouteResponse(const int messageId)
{
  std::unique_ptr<http::CoreResponse> res = std::make_unique<http::CoreResponse>();
  res->setCode(200);
  res->setBody(
      fmt::format(R"({{"status":"message_sent","message_id":"{}"}})", std::to_string(messageId))
  );
  return std::move(res);
}

