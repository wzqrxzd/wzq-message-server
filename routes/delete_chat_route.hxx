#ifndef DELETE_CHAT_ROUTE_HXX
#define DELETE_CHAT_ROUTE_HXX

#include "repositories/chat_repository.hxx"
#include "repositories/user_repository.hxx"
#include "route.hxx"

#include <nlohmann/json.hpp>

class DeleteChatRoute : public Route
{
  public:
    using Deps = std::tuple<UserRepository&, ChatRepository&>;
    
    DeleteChatRoute(RouteContext context, UserRepository& userRepository, ChatRepository& chatRepository)
      : Route(RouteInfo("/chats", http::Method::DELETE), context), userRepository(userRepository), chatRepository(chatRepository) {}

    std::unique_ptr<http::Response> handleRequest(const http::Request& req) override;
  private:
    int loadChatId(const nlohmann::json& json);
    std::unique_ptr<http::Response> buildRouteResponse();

    const UserRepository& userRepository;
    const ChatRepository& chatRepository;
};

#endif
