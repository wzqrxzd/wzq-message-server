#include "insert_member_route.hxx"
#include "error.hxx"

std::unique_ptr<http::Response> InsertMemberRoute::handleRequest(const http::Request& req)
{
  const std::string username = context.auth.authorize(req);

  auto [userId, chatId] = loadRequestData(nlohmann::json::parse(req.body()));

  chatRepository.insertUserToChat(userId, chatId);
  
  return buildRouteResponse();
}

std::unique_ptr<http::Response> InsertMemberRoute::buildRouteResponse()
{
  std::unique_ptr<http::CoreResponse> response = std::make_unique<http::CoreResponse>();
  response->setCode(200);
  response->setBody(
    R"({"status":"user added"})"
  );

  return std::move(response);
}

std::pair<int, int> InsertMemberRoute::loadRequestData(const nlohmann::json& json)
{
  if (!json.contains("chat_id") || !json.contains("user_id"))
    throw JsonException("Malformed Json");

  return std::pair<int, int>(json["chat_id"].get<int>(), json["user_id"].get<int>());
}
