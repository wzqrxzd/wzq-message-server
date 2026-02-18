#include "chat_messages_route.hxx"
#include "error.hxx"

std::unique_ptr<http::Response> ChatMessagesRoute::handleRequest(const http::Request& req)
{
  const std::string username = context.auth.authorize(req);

  const int userId = userRepository.getUserId(username);

  const int chatId = loadChatId(nlohmann::json::parse(req.body()));

  chatRepository.ensureUserInChat(userId, chatId);

  std::vector<Message> messages = messageRepository.getChatHistory(chatId);

  return buildRouteResponse(messages);
}

std::unique_ptr<http::Response> ChatMessagesRoute::buildRouteResponse(const std::vector<Message>& messages)
{
  nlohmann::json resMessages = nlohmann::json::array();

  for (const auto& message : messages)
  {
    resMessages.push_back({
        {"message_id", message.messageId},
        {"sender_id", message.senderId},
        {"sender_name", message.username},
        {"content", message.content}
    });
  }

  std::unique_ptr<http::CoreResponse> response = std::make_unique<http::CoreResponse>();
  response->setCode(200);
  response->setBody(resMessages.dump());
  return std::move(response);
}

const int ChatMessagesRoute::loadChatId(const nlohmann::json& json)
{
  if (!json.contains("id"))
    throw JsonException("Malformerd json");

  return json["id"];
}
