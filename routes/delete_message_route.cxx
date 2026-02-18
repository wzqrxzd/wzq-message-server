#include "delete_message_route.hxx"
#include "error.hxx"

std::unique_ptr<http::Response> DeleteMessageRoute::handleRequest(const http::Request& req)
{
  const std::string username = context.auth.authorize(req);

  int userId = userRepository.getUserId(username);
  int messageId = loadMessageId(nlohmann::json::parse(req.body()));

  int senderId = messageRepository.getSenderId(messageId);

  if (userId != senderId)
    throw AuthException(AuthError::PermissionDenied);

  messageRepository.deleteMessage(messageId);
  return buildRouteResponse(messageId);
}

std::unique_ptr<http::Response> DeleteMessageRoute::buildRouteResponse(const int messageId)
{
  std::unique_ptr<http::CoreResponse> response = std::make_unique<http::CoreResponse>();
  response->setCode(200);
  response->setBody(
    fmt::format(R"({{"status":"deleted","message_id":"{}"}})",std::to_string(messageId))
  );

  return std::move(response);
}

int DeleteMessageRoute::loadMessageId(const nlohmann::json& json)
{
  if (!json.contains("message_id"))
    throw JsonException("Malformed Json");

  return json["message_id"].get<int>();
}
