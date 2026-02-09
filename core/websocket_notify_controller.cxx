#include "websocket_notify_controller.hxx"
#include <future>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

WebsocketNotifyController::WebsocketNotifyController() {};

void WebsocketNotifyController::addClient(std::shared_ptr<WsClient> client)
{
  std::lock_guard<std::mutex> guard(mtx);
  clients[client->getUserId()] = client;
}

void WebsocketNotifyController::deleteClient(std::shared_ptr<WsClient> client)
{
  std::lock_guard<std::mutex> guard(mtx);
  clients.erase(client->getUserId());
}

void WebsocketNotifyController::dispatch(std::unique_ptr<Command> command)
{
  std::unordered_map<UserId, std::shared_ptr<WsClient>> snapshot;

  {
    std::lock_guard<std::mutex> guard(mtx);
    snapshot = clients;
  }

  auto fut = std::async(std::launch::async, [cmd = std::move(command), snapshot]() mutable {
      (*cmd)(snapshot);
  });
}
//
// void WebsocketNotifyController::notifyNewMessage(Message& message)
// {
//   nlohmann::json notify = {
//       {"type", "new_message"},
//       {"chat_id", message.chatId},
//       {"message_id", message.messageId},
//       {"sender_id", message.senderId},
//       {"sender_name", message.username},
//       {"content", message.content}
//   };
//   std::lock_guard<std::mutex> lock(mtx);
//   spdlog::info("cahtId {} message {}", message.chatId, message.content);
//
//   for (auto& [_, client] : clients) {
//     if (client->getChatIds().count(message.chatId)) {
//       client->sendText(notify.dump());
//     }
//   }
// }
//
// void WebsocketNotifyController::notifyNewChat(const int& chatId, const int& userId, const std::string& chatName)
// {
//   nlohmann::json notify = {
//     {"type", "new_chat"},
//     {"chat_id", chatId},
//     {"chat_name", chatName}
//   };
//
//   std::lock_guard<std::mutex> lock(mtx);
//   for (auto& [id, client] : clients) {
//     if (id == userId) {
//       client->insertChatId(chatId);
//       client->sendText(notify.dump());
//       spdlog::info("[WS] Sent new_chat notify to userId={} chatId={}", userId, chatId);
//     }
//   }
//
// }
//
// void WebsocketNotifyController::notifyDeleteChat(const int& chatId, const int& userId)
// {
//   nlohmann::json notify = {
//     {"type", "delete_chat"},
//     {"chat_id", chatId}
//   };
//
//   {
//     std::lock_guard<std::mutex> lock(mtx);
//     for (auto& [id, client] : clients) {
//       if (id == userId) {
//         client->eraseChatId(chatId);
//         client->sendText(notify.dump());
//         spdlog::info("[WS] Sent delete_chat notify to userId={} chatId={}", userId, chatId);
//       }
//     }
//   }
//
// }
