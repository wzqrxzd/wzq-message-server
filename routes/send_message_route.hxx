#ifndef SEND_MESSAGE_ROUTE_HXX
#define SEND_MESSAGE_ROUTE_HXX

#include "repositories/chat_repository.hxx"
#include "repositories/message_repository.hxx"
#include "repositories/user_repository.hxx"
#include "route.hxx"
#include "types/Message.hxx"
#include <nlohmann/json.hpp>

class SendMessageRoute : public Route
{
  public:
    using Deps = std::tuple<UserRepository&, ChatRepository&, MessageRepository&>;

    SendMessageRoute(RouteContext context, UserRepository& userRepository, ChatRepository& chatRepository, MessageRepository& messageRepository) : 
      Route(RouteInfo("/chats/messages", http::Method::POST), context), 
      userRepository(userRepository), chatRepository(chatRepository), messageRepository(messageRepository) {}

    std::unique_ptr<http::Response> handleRequest(const http::Request& req) override;
  private:
    Message loadMessageData(const nlohmann::json& json);
    std::unique_ptr<http::Response> buildRouteResponse(const int messageId);

    const UserRepository& userRepository;
    const ChatRepository& chatRepository;
    const MessageRepository& messageRepository;
};

#endif
