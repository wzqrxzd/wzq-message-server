#ifndef WEBSOCKET_NOTIFY_CONTROLLER_HXX
#define WEBSOCKET_NOTIFY_CONTROLLER_HXX

#include "commands/command.hxx"
#include <memory>
#include <unordered_map>
#include "types/WebsocketClient.hxx"

class TestWebsocketRoute;

class WebsocketNotifyController
{
  public:
    WebsocketNotifyController();

    void addClient(std::shared_ptr<WsClient> client);
    void deleteClient(std::shared_ptr<WsClient> client);

    void dispatch(std::unique_ptr<Command> command);
  private:
    std::mutex mtx;
    std::unordered_map<UserId, std::shared_ptr<WsClient>> clients;
};

#endif
