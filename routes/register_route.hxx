#ifndef REGISTER_ROUTE_HXX
#define REGISTER_ROUTE_HXX

#include "repositories/user_repository.hxx"
#include "route.hxx"

class RegisterRoute : public Route
{
  public:
    using Deps = std::tuple<UserRepository&>;

    RegisterRoute(RouteContext context, UserRepository& userRepository) : Route(RouteInfo("/register", http::Method::POST), context), userRepository(userRepository) {}

    std::unique_ptr<http::Response> handleRequest(const http::Request& req) override;
  private:
    UserFields loadUserData(const http::Request& req);
    std::unique_ptr<http::Response> buildRegisterRouteResponse(const std::string& token);
    const UserRepository& userRepository;
};

#endif
