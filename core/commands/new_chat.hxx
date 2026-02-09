#ifndef NEW_CHAT_COMMAND_HXX
#define NEW_CHAT_COMMAND_HXX

#include "commands/command.hxx"
#include "types/Chat.hxx"
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

class NewChatCommand : public Command
{
  public:
    NewChatCommand(const Chat& chat) : chat(chat) {}

    void operator()(const std::unordered_map<UserId, std::shared_ptr<WsClient>>& clients) const override {
      nlohmann::json notify = {
        {"type", "new_chat"},
        {"chat_id", chat.chatId},
        {"chat_name", chat.chatName}
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

      for (const auto& [id, client] : targets)
      {
        client->insertChatId(chat.chatId);
        client->sendText(notify.dump());
        // spdlog::info("[WS] Sent new_chat notify to userId={} chatId={}", id, chat.chatId);
      }
    }
  private:
    Chat chat;
};

#endif
