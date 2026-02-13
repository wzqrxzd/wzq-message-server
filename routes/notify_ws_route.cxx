#include "notify_ws_route.hxx"
#include "error.hxx"

void NotifyWebsocketRoute::onOpen(std::shared_ptr<WsClient> client) {}

void NotifyWebsocketRoute::onMessage(std::shared_ptr<WsClient> client, const std::string_view& message)
{
  try {
      nlohmann::json data = nlohmann::json::parse(message);
      spdlog::debug("message: {}", message);

      if (data.contains("token"))
      {
        std::string token = data["token"];
        spdlog::debug("token: {}", token);

        std::string username = context.auth.getUsernameFromToken(token);
        spdlog::debug("username: {}", username);

        int userId = userRepository.getUserId(username);
        spdlog::debug("[Ws] username = {}, id = {}", username, userId);

        std::unordered_set<int> userChats = userRepository.getUserChats(userId);

        for (auto& id : userChats)
          client->insertChatId(id);

        context.ws.addClient(client);
        client->sendText(R"({"status":"ws_auth_ok"})");
      }
  } catch(const UserNotExistsException& e) {
    client->sendText(R"({"error":"user_not_exists"})");
  } catch (...) {
    client->sendText(R"({"error":"ws_auth_failed"})");
  }
}

void NotifyWebsocketRoute::onClose(std::shared_ptr<WsClient> client, const std::string_view& reason)
{
  context.ws.deleteClient(client);
}
