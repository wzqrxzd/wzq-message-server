#include "user_info_route.hxx"
#include "error.hxx"
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

std::unique_ptr<http::Response> UserInfoRoute::handleTypedRequest(const http::Request& req, int userId)
{
  const std::string username = context.auth.authorize(req);

  spdlog::debug("userId: {}", userId);
  const UserFields user = userRepository.getUserData(userId);

  return buildRouteResponse(user);
}


std::unique_ptr<http::Response> UserInfoRoute::buildRouteResponse(const UserFields& user)
{
  std::unique_ptr<http::CoreResponse> response = std::make_unique<http::CoreResponse>();
  response->setCode(200);
  response->setBody(
      fmt::format(R"({{"name":"{}","username":"{}","description":"{}","user_id":"{}"}})",
        user.username,
        user.name,
        user.description,
        user.id
      )
  );

  return std::move(response);
}
