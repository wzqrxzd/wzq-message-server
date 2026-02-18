#include "user_update_info_route.hxx"
#include "error.hxx"
#include <nlohmann/json.hpp>

std::unique_ptr<http::Response> UserUpdateInfoRoute::handleRequest(const http::Request& req)
{
  const std::string username = context.auth.authorize(req);
  const int userId = userRepository.getUserId(username);

  const UserFields updatedUserFields = loadUserData(req);

  userRepository.changeUserData(userId, updatedUserFields);

  return buildRouteResponse();
}

UserFields UserUpdateInfoRoute::loadUserData(const http::Request& req)
{
  auto bodyJson = nlohmann::json::parse(req.body());

  if (!bodyJson.contains("name") || !bodyJson.contains("username") || !bodyJson.contains("password"))
    throw JsonException("Malformed Json");

  UserFields user;
  user.name = bodyJson["name"];
  user.username = bodyJson["username"];
  user.password = bodyJson["password"];

  return user;
}

std::unique_ptr<http::Response> UserUpdateInfoRoute::buildRouteResponse()
{
  std::unique_ptr<http::CoreResponse> response = std::make_unique<http::CoreResponse>();
  response->setCode(204);

  return std::move(response);
}
