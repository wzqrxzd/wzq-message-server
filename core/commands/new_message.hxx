#ifndef NEW_MESSAGE_COMMAND_HXX
#define NEW_MESSAGE_COMMAND_HXX

#include "commands/command.hxx"
#include "types/Message.hxx"
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

class NewMessageCommand : public Command
{
  public:
    NewMessageCommand(const Message& msg) : message(msg) {}

    void operator()(const std::unordered_map<UserId, std::shared_ptr<WsClient>>& clients) const override
    {
      nlohmann::json notify = {
            {"type", "new_message"},
            {"chat_id", message.chatId},
            {"message_id", message.messageId},
            {"sender_id", message.senderId},
            {"sender_name", message.username},
            {"content", message.content}
      };

      auto targets = filter(
        [chatId = message.chatId](const std::shared_ptr<WsClient>& client) {
          return client->getChatIds().count(chatId);
        },
        clients
      );

      for (const auto& [id, client] : targets)
        client->sendText(notify.dump());

      spdlog::info("cahtId {} message {}", message.chatId, message.content);
    }
  private:
    Message message;
};

#endif
