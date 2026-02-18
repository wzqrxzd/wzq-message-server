#include "user_info_route.hxx"
#include "error.hxx"
#include <fmt/format.h>

std::unique_ptr<http::Response> UserInfoRoute::handleRequest(const http::Request& req)
{
  context.auth.authorize(req);

  const int userId = loadRequestedId(nlohmann::json::parse(req.body()));

  const UserFields user = userRepository.getUserData(userId);

  return buildRouteResponse(user);
}

std::unique_ptr<http::Response> UserInfoRoute::buildRouteResponse(const UserFields& user)
{
  std::unique_ptr<http::CoreResponse> response = std::make_unique<http::CoreResponse>();
  response->setCode(200);
  response->setBody(
      fmt::format(R"({{"name":"{}","username":"{}","description":"{}","user_id":"{}"}})",
        user.name,
        user.username,
        user.description,
        user.id
      )
  );

  return std::move(response);
}

const int UserInfoRoute::loadRequestedId(const nlohmann::json& json)
{
  if (!json.contains("id"))
    throw JsonException("Malformed Json");

  return json["id"].get<int>();
}
