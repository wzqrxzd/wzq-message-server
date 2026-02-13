#include "login_route.hxx"
#include "error.hxx"
#include <fmt/format.h>
#include <nlohmann/json.hpp>

std::unique_ptr<http::Response> LoginRoute::handleRequest(const http::Request& req)
{
  UserFields user = loadUserData(req);

  userRepository.ensureUserExists(user.username);
  const std::string hashPassword = userRepository.getUserHashedPassword(user.username);

  context.auth.verifyPassword(hashPassword, user.password);

  const std::string token = context.auth.generateJWT(user.username);
  const int userId = userRepository.getUserId(user.username);

  return buildLoginRouteResponse(token, userId);
}

UserFields LoginRoute::loadUserData(const http::Request& req)
{
  auto bodyJson = nlohmann::json::parse(req.body());

  if (!bodyJson.contains("username") || !bodyJson.contains("password"))
    throw JsonException("Malformed Json");

  UserFields user;
  user.username = bodyJson["username"].get<std::string>();
  user.password = bodyJson["password"].get<std::string>();

  return user;
}

std::unique_ptr<http::Response> LoginRoute::buildLoginRouteResponse(const std::string& token, const int userId)
{
  std::unique_ptr<http::CoreResponse> response = std::make_unique<http::CoreResponse>();
  response->setCode(200);
  response->setBody(
       fmt::format(R"({{"token":"{}","user_id":"{}"}})",
          token,
          userId
      )
  );

  return std::move(response);
}
