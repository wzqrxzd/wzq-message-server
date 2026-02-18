#ifndef CREATE_CHAT_ROUTE
#define CREATE_CHAT_ROUTE

#include "repositories/chat_repository.hxx"
#include "repositories/user_repository.hxx"
#include "route.hxx"
#include <nlohmann/json.hpp>

class CreateChatRoute : public Route
{
  public:
    using Deps = std::tuple<UserRepository&, ChatRepository&>;

    CreateChatRoute(RouteContext context, UserRepository& userRepository, ChatRepository& chatRepository) : Route(RouteInfo("/chats", http::Method::POST), context), userRepository(userRepository), chatRepository(chatRepository) {}
    std::unique_ptr<http::Response> handleRequest(const http::Request& req);

  private:
    void addUsersToChat(const Chat& chat);
    Chat loadChatData(const nlohmann::json& json);

    std::unique_ptr<http::Response> buildRouteResponse(const int chatId);
    const ChatRepository& chatRepository;
    const UserRepository& userRepository;
};

#endif
