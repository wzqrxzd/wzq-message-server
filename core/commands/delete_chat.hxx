#ifndef DELETE_CHAT_COMMAND_HXX
#define DELETE_CHAT_COMMAND_HXX

#include "commands/command.hxx"
#include "types/Chat.hxx"
#include <nlohmann/json.hpp>

class DeleteChatCommand : public Command
{
  public:
    DeleteChatCommand(const Chat& chat) : chat(chat) {}

    void operator()(const std::unordered_map<UserId, std::shared_ptr<WsClient>>& clients) const override {
      nlohmann::json notify = {
        {"type", "delete_chat"},
        {"chat_id", chat.chatId}
      };

      auto filterFunc = [&users = chat.usersId](const std::shared_ptr<WsClient>& client)
      {
        bool isFiltered = false;

        for (const auto& [id, member] : users)
        {
          if (client->getUserId() == id)
            isFiltered = true;
        }
        return isFiltered;
      };

      auto targets = filter(filterFunc, clients);

      for (const auto& [id, client] : clients)
      {
        client->eraseChatId(chat.chatId);
        client->sendText(notify.dump());
        // spdlog::info("[WS] Sent delete_chat notify to userId={} chatId={}", userId, chatId);
      }
    }
  private:
    Chat chat;
};

#endif
