#ifndef CHAT_MESSAGES_ROUTE
#define CHAT_MESSAGES_ROUTE

#include "repositories/chat_repository.hxx"
#include "repositories/message_repository.hxx"
#include "repositories/user_repository.hxx"
#include "route.hxx"
#include <nlohmann/json.hpp>

class ChatMessagesRoute : public Route
{
  public:
    using Deps = std::tuple<UserRepository&, ChatRepository&, MessageRepository&>;

    ChatMessagesRoute(RouteContext context, UserRepository& userRepository, ChatRepository& chatRepository, MessageRepository& messageRepository)
      : Route(RouteInfo("/chats/messages/get", http::Method::POST), context),
        userRepository(userRepository), chatRepository(chatRepository), messageRepository(messageRepository)
    {}

    std::unique_ptr<http::Response> handleRequest(const http::Request& req);

  private:
    const int loadChatId(const nlohmann::json& json);
    std::unique_ptr<http::Response> buildRouteResponse(const std::vector<Message>& messages);

    const UserRepository& userRepository;
    const ChatRepository& chatRepository;
    const MessageRepository& messageRepository;
};

#endif
