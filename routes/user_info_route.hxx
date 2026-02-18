#ifndef USER_INFO_ROUTE
#define USER_INFO_ROUTE

#include "repositories/user_repository.hxx"
#include "route.hxx"
#include <nlohmann/json.hpp>

class UserInfoRoute : public Route
{
  public:
    using Deps = std::tuple<UserRepository&>;

    UserInfoRoute(RouteContext context, UserRepository& userRepository) : Route(RouteInfo("/user", http::Method::POST), context), userRepository(userRepository) {}

    std::unique_ptr<http::Response> handleRequest(const http::Request& req) override;
  private:
    const int loadRequestedId(const nlohmann::json& body);
    std::unique_ptr<http::Response> buildRouteResponse(const UserFields& user);
    const UserRepository& userRepository;
};

#endif
