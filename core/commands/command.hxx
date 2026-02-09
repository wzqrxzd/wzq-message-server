#ifndef COMMAND_HXX
#define COMMAND_HXX

#include <functional>
#include <memory>
#include <unordered_map>
#include "types/WebsocketClient.hxx"

typedef int UserId;

class Command
{
  public:
    virtual ~Command() = default;

    using FilterFunc = std::function<bool(const std::shared_ptr<WsClient>&)>;
    using ClientsMap = std::unordered_map<UserId, std::shared_ptr<WsClient>>;

    virtual void operator()(const ClientsMap&) const = 0;

};

static Command::ClientsMap filter(Command::FilterFunc filter, Command::ClientsMap clients) {
  Command::ClientsMap filtered;

  for (auto& [id, client] : clients)
  {
    if (filter(client)) {
      filtered[id] = client;
    }
  }

  return filtered;
};

#endif
