#ifndef INSERT_MEMBER_ROUTE_HXX
#define INSERT_MEMBER_ROUTE_HXX

#include "repositories/chat_repository.hxx"
#include "repositories/user_repository.hxx"
#include "route.hxx"

#include <nlohmann/json.hpp>

class InsertMemberRoute : public Route
{
  public:
    using Deps = std::tuple<UserRepository&, ChatRepository&>;

    InsertMemberRoute(RouteContext context, UserRepository& userRepository, ChatRepository& chatRepository)
      : Route(RouteInfo("/chats/users", http::Method::POST), context), userRepository(userRepository), chatRepository(chatRepository) {}

    std::unique_ptr<http::Response> handleRequest(const http::Request& req) override;
  private:
    std::pair<int, int> loadRequestData(const nlohmann::json& json);
    std::unique_ptr<http::Response> buildRouteResponse();

    const UserRepository& userRepository;
    const ChatRepository& chatRepository;
};

#endif
