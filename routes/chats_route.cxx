#include "chats_route.hxx"
#include <nlohmann/json.hpp>

std::unique_ptr<http::Response> ChatsRoute::handleRequest(const http::Request& req)
{
  const std::string username = context.auth.authorize(req);
  const int userId = userRepository.getUserId(username);

  const std::vector<Chat> userChats = chatRepository.getUserChats(userId);

  return buildRouteResponse(userChats);
}


std::unique_ptr<http::Response> ChatsRoute::buildRouteResponse(const std::vector<Chat>& chats)
{
  nlohmann::json responseJson = nlohmann::json::array();

  for (const auto& chat : chats)
  {
    responseJson.push_back({
        {"chat_id", chat.chatId},
        {"chat_name", chat.chatName}
    });
  }

  std::unique_ptr<http::CoreResponse> response = std::make_unique<http::CoreResponse>();
  response->setCode(200);
  response->setBody(responseJson.dump());

  return std::move(response);
}
