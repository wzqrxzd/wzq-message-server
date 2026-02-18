#ifndef LOGIN_ROUTE_HXX
#define LOGIN_ROUTE_HXX

#include "repositories/user_repository.hxx"
#include "route.hxx"

class LoginRoute : public Route
{
  public:
    using Deps = std::tuple<UserRepository&>;

    LoginRoute(RouteContext context, UserRepository& userRepository) : Route(RouteInfo("/login", http::Method::POST), context), userRepository(userRepository) {}

    std::unique_ptr<http::Response> handleRequest(const http::Request& req) override;
  private:
    UserFields loadUserData(const http::Request& req);
    std::unique_ptr<http::Response> buildLoginRouteResponse(const std::string& token, const int userId);
    const UserRepository& userRepository;
};

#endif
