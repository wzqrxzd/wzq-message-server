#ifndef USER_UDPATE_INFO_ROUTE_HXX
#define USER_UPDATE_INFO_ROUTE_HXX

#include "repositories/user_repository.hxx"
#include "route.hxx"

class UserUpdateInfoRoute : public Route
{
  public:
    using Deps = std::tuple<UserRepository&>;

    UserUpdateInfoRoute(RouteContext context, UserRepository& userRepository) : Route(RouteInfo("/user", http::Method::PATCH), context), userRepository(userRepository) {}

    std::unique_ptr<http::Response> handleRequest(const http::Request& req) override;
  private:
    UserFields loadUserData(const http::Request& req);
    std::unique_ptr<http::Response> buildRouteResponse();

    const UserRepository& userRepository;
};

#endif
