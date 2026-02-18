#ifndef CHATS_ROUTE_HXX
#define CHATS_ROUTE_HXX

#include "repositories/chat_repository.hxx"
#include "repositories/user_repository.hxx"
#include "route.hxx"

class ChatsRoute : public Route
{
  public:
    using Deps = std::tuple<UserRepository&, ChatRepository&>;

    ChatsRoute(RouteContext context, UserRepository& userRepository, ChatRepository& chatRepository) : Route(RouteInfo("/chats", http::Method::GET), context), userRepository(userRepository), chatRepository(chatRepository) {}

    std::unique_ptr<http::Response> handleRequest(const http::Request& req);
  private:
    std::unique_ptr<http::Response> buildRouteResponse(const std::vector<Chat>& chats);

    UserRepository& userRepository;
    ChatRepository& chatRepository;
};

#endif
