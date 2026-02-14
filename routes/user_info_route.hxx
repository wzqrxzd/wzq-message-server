#ifndef USER_UDPATE_INFO_ROUTE_HXX
#define USER_UPDATE_INFO_ROUTE_HXX

#include "repositories/user_repository.hxx"
#include "route.hxx"

class UserInfoRoute : public VariadicRoute<int>
{
  public:
    using Deps = std::tuple<UserRepository&>;

    UserInfoRoute(RouteContext context, UserRepository& userRepository) : VariadicRoute(RouteInfo("/user/<int>", http::Method::GET), context), userRepository(userRepository) {}

    std::unique_ptr<http::Response> handleTypedRequest(const http::Request& req, int userId) override;
  private:
    std::unique_ptr<http::Response> buildRouteResponse(const UserFields& user);

    const UserRepository& userRepository;
};

#endif
