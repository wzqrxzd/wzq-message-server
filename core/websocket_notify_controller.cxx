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
