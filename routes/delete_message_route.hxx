#ifndef DELETE_MESSAGE_ROUTE
#define DELETE_MESSAGE_ROUTE

#include "repositories/message_repository.hxx"
#include "repositories/user_repository.hxx"
#include "route.hxx"
#include <nlohmann/json.hpp>

class DeleteMessageRoute : public Route
{
  public:
    using Deps = std::tuple<UserRepository&, MessageRepository&>;

    DeleteMessageRoute(RouteContext context, UserRepository& userRepository, MessageRepository& messageRepository)
      : Route(RouteInfo("/chats/messages", http::Method::DELETE), context), userRepository(userRepository), messageRepository(messageRepository) {}

    std::unique_ptr<http::Response> handleRequest(const http::Request& req) override;
  private:
    int loadMessageId(const nlohmann::json& json);
    std::unique_ptr<http::Response> buildRouteResponse(const int messageId);

    const UserRepository& userRepository;
    const MessageRepository& messageRepository;
};

#endif
