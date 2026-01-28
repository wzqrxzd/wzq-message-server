#ifndef WEBSOCKET_NOTIFY_CONTROLLER_HXX
#define WEBSOCKET_NOTIFY_CONTROLLER_HXX

#include "types/Message.hxx"
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

class WsClient
{
  public:
    virtual ~WsClient() = default;
    virtual int getUserId() = 0;
    virtual void sendText(const std::string_view& sv) = 0;
    virtual std::unordered_set<int> getChatIds() = 0;
    virtual void insertChatId(const int& id) = 0;
    virtual void eraseChatId(const int& id) = 0;
};

class [[nodiscard]] WebsocketNotifyController
{
  public:
    WebsocketNotifyController();

    void addClient(std::shared_ptr<WsClient> client);
    void deleteClient(std::shared_ptr<WsClient> client);

    void notifyNewMessage(Message& msg);
    void notifyNewChat(const int& chatId, const int& userId, const std::string& chatName);
    void notifyDeleteChat(const int& chatId, const int& userId);

  private:
    std::mutex mtx;
    std::unordered_map<int, std::shared_ptr<WsClient>> clients;
};

#endif
