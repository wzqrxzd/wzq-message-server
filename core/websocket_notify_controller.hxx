#ifndef WEBSOCKET_NOTIFY_CONTROLLER_HXX
#define WEBSOCKET_NOTIFY_CONTROLLER_HXX

#include "types/Message.hxx"
#include "types/Chat.hxx"
#include "commands/command.hxx"
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include "types/WebsocketClient.hxx"

class WebsocketNotifyController
{
  public:
    WebsocketNotifyController();

    void addClient(std::shared_ptr<WsClient> client);
    void deleteClient(std::shared_ptr<WsClient> client);

    void dispatch(std::unique_ptr<Command> command);

    void notifyNewMessage(const Message& msg);
    void notifyNewChat(const Chat& chat);
    void notifyDeleteChat(const Chat& chat);

  private:
    std::mutex mtx;
    std::unordered_map<UserId, std::shared_ptr<WsClient>> clients;
};

#endif
