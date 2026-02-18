#include "register_route.hxx"
#include "error.hxx"
#include <nlohmann/json.hpp>

std::unique_ptr<http::Response> RegisterRoute::handleRequest(const http::Request& req)
{
  UserFields user = loadUserData(req);
  user.password = context.auth.hashPassword(user.password);

  userRepository.ensureUserNotExists(user.username);

  userRepository.createUser(user);

  const std::string token = context.auth.generateJWT(user.username);

  return buildRegisterRouteResponse(token);
}

UserFields RegisterRoute::loadUserData(const http::Request& req)
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

std::unique_ptr<http::Response> RegisterRoute::buildRegisterRouteResponse(const std::string& token)
{
  std::unique_ptr<http::CoreResponse> response = std::make_unique<http::CoreResponse>();
  response->setCode(200);
  response->setBody(
    fmt::format(R"({{"status":"registered", "token":"{}"}})", token)
  );

  return std::move(response);
}
