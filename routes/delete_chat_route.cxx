#include "delete_chat_route.hxx"
#include "error.hxx"

std::unique_ptr<http::Response> DeleteChatRoute::handleRequest(const http::Request& req)
{
  const std::string username = context.auth.authorize(req);
  const int userId = userRepository.getUserId(username);

  const int chatId = loadChatId(nlohmann::json::parse(req.body()));

  chatRepository.ensureUserInChat(userId, chatId);

  chatRepository.deleteChat(chatId);
  return buildRouteResponse();
}

int DeleteChatRoute::loadChatId(const nlohmann::json& json)
{
  if (!json.contains("id"))
    throw JsonException("Malformed Json");

  return json["id"].get<int>();
}

std::unique_ptr<http::Response> DeleteChatRoute::buildRouteResponse()
{
  std::unique_ptr<http::CoreResponse> response = std::make_unique<http::CoreResponse>();
  response->setCode(200);
  response->setBody(R"({"status":"chat_deleted"})");

  return std::move(response);
}

