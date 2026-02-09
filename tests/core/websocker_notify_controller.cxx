#include <future>
#include <gtest/gtest.h>
#include "websocket_notify_controller.hxx"
#include "commands/new_message.hxx"
#include "commands/new_chat.hxx"
#include "commands/delete_chat.hxx"

class FakeClient : public WsClient
{
  private:
    int userId;
    std::unordered_set<int> chatIds;

  public:
    FakeClient() {};
    int getUserId() override { return userId; }
    void sendText(const std::string_view& sv) override {
      lastMessage = std::string(sv);
    }
    std::unordered_set<int> getChatIds() override { return chatIds; };
    void insertChatId(const int& id) override { chatIds.insert(id); }
    void eraseChatId(const int& id) override { chatIds.erase(id); }

    void setId(const int& id) { userId = id; }

    std::string lastMessage;
};

class TestCommand : public Command
{
  public:
    TestCommand(std::promise<void>& promise) : promise(promise) {};

    void operator()(const std::unordered_map<UserId, std::shared_ptr<WsClient>>& clients) const override
    {
      promise.set_value();
    }
  private:
    std::promise<void>& promise;
};

TEST(NotifyController, SetupClients)
{
  WebsocketNotifyController controller;

  std::array<std::shared_ptr<FakeClient>, 4> clients;

  for (int i{0}; i<clients.size(); i++)
  {
    clients[i] = std::make_shared<FakeClient>();
    clients[i]->setId(i);
  }

  for (auto& client : clients)
    controller.addClient(client);
}

TEST(NotifyController, DeleteClient)
{
  WebsocketNotifyController controller;
  constexpr int id = 1;

  std::shared_ptr<FakeClient> client = std::make_shared<FakeClient>();
  client->setId(id);

  controller.addClient(client);
  controller.deleteClient(client);
}

TEST(NotifyController, DispatchInvokesCommand)
{
  WebsocketNotifyController controller;

  std::promise<void> done;
  auto fut = done.get_future();

  controller.dispatch(std::make_unique<TestCommand>(done));

  auto status = fut.wait_for(std::chrono::seconds(1));

  EXPECT_EQ(status, std::future_status::ready);
}

TEST(NewMessageCommand, SendNewMessageNotification)
{
  constexpr int id = 1;
  constexpr int chatId = 1;

  std::shared_ptr<FakeClient> client = std::make_shared<FakeClient>();
  client->setId(id);
  client->insertChatId(chatId);

  Message msg;
  msg.username = "John Smith";
  msg.content = "test";
  msg.senderId = 32;
  msg.chatId = chatId;
  msg.messageId = 1;

  auto command = std::make_unique<NewMessageCommand>(msg);

  std::unordered_map<UserId, std::shared_ptr<WsClient>> clients
  {
    {1, client}
  };

  command->operator()(clients);
  EXPECT_FALSE(client->lastMessage.empty());
}

TEST(NewMessageCommand, ParseNewMessageNotificationJson)
{
  constexpr int id = 1;
  constexpr int chatId = 1;


  std::shared_ptr<FakeClient> client = std::make_shared<FakeClient>();
  client->setId(id);
  client->insertChatId(chatId);

  Message msg;
  msg.username = "John Smith";
  msg.content = "test";
  msg.senderId = 32;
  msg.chatId = chatId;
  msg.messageId = 1;

  nlohmann::json expectedNotify = {
        {"type", "new_message"},
        {"chat_id", msg.chatId},
        {"message_id", msg.messageId},
        {"sender_id", msg.senderId},
        {"sender_name", msg.username},
        {"content", msg.content}
  };

  auto command = std::make_unique<NewMessageCommand>(msg);

  std::unordered_map<UserId, std::shared_ptr<WsClient>> clients
  {
    {1, client}
  };

  command->operator()(clients);
  nlohmann::json receivedNotify = nlohmann::json::parse(client->lastMessage);

  EXPECT_EQ(expectedNotify, receivedNotify);
}

TEST(NewChatCommand, SendNewChatNotification)
{
  constexpr int id = 1;
  constexpr int chatId = 1;

  std::shared_ptr<FakeClient> client = std::make_shared<FakeClient>();
  client->setId(id);
  client->insertChatId(chatId);

  UserFields user;
  user.name = "John Smith";
  user.username = "john_smith";
  user.description = "test";
  user.id = id;


  std::unordered_map<int, UserFields> usersId;
  usersId[id] = user;

  Chat chat;
  chat.chatId = chatId;
  chat.chatName = "test";
  chat.usersId = usersId;

  auto command = std::make_unique<NewChatCommand>(chat);

  std::unordered_map<UserId, std::shared_ptr<WsClient>> clients
  {
    {1, client}
  };

  command->operator()(clients);

  EXPECT_FALSE(client->lastMessage.empty());

}

TEST(NewChatCommand, ParseNewChatNotification)
{
  constexpr int id = 1;
  constexpr int chatId = 1;

  std::shared_ptr<FakeClient> client = std::make_shared<FakeClient>();
  client->setId(id);
  client->insertChatId(chatId);

  UserFields user;
  user.name = "John Smith";
  user.username = "john_smith";
  user.description = "test";
  user.id = id;

  std::unordered_map<int, UserFields> usersId;
  usersId[id] = user;

  Chat chat;
  chat.chatId = chatId;
  chat.chatName = "test";
  chat.usersId = usersId;

  nlohmann::json expectedNotify = {
    {"type", "new_chat"},
    {"chat_id", chat.chatId},
    {"chat_name", chat.chatName}
  };

  auto command = std::make_unique<NewChatCommand>(chat);

  std::unordered_map<UserId, std::shared_ptr<WsClient>> clients
  {
    {1, client}
  };

  command->operator()(clients);

  EXPECT_EQ(expectedNotify, nlohmann::json::parse(client->lastMessage));
}

TEST(DeleteMessageCommand, SendDeleteMessageNotify)
{
  constexpr int id = 1;
  constexpr int chatId = 1;

  std::shared_ptr<FakeClient> client = std::make_shared<FakeClient>();
  client->setId(id);
  client->insertChatId(chatId);

  UserFields user;
  user.name = "John Smith";
  user.username = "john_smith";
  user.description = "test";
  user.id = id;


  std::unordered_map<int, UserFields> usersId;
  usersId[id] = user;

  Chat chat;
  chat.chatId = chatId;
  chat.chatName = "test";
  chat.usersId = usersId;

  auto command = std::make_unique<DeleteChatCommand>(chat);

  std::unordered_map<UserId, std::shared_ptr<WsClient>> clients
  {
    {1, client}
  };

  command->operator()(clients);

  EXPECT_FALSE(client->lastMessage.empty());
}

TEST(DeleteMessageCommand, ParseDeleteMessageNotify)
{
  constexpr int id = 1;
  constexpr int chatId = 1;

  std::shared_ptr<FakeClient> client = std::make_shared<FakeClient>();
  client->setId(id);
  client->insertChatId(chatId);

  UserFields user;
  user.name = "John Smith";
  user.username = "john_smith";
  user.description = "test";
  user.id = id;

  std::unordered_map<int, UserFields> usersId;
  usersId[id] = user;

  Chat chat;
  chat.chatId = chatId;
  chat.chatName = "test";
  chat.usersId = usersId;

  nlohmann::json expectedNotify = {
    {"type", "delete_chat"},
    {"chat_id", chat.chatId}
  };

  auto command = std::make_unique<DeleteChatCommand>(chat);

  std::unordered_map<UserId, std::shared_ptr<WsClient>> clients
  {
    {1, client}
  };

  command->operator()(clients);

  EXPECT_EQ(expectedNotify, nlohmann::json::parse(client->lastMessage));
}
