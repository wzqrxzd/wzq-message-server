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
    using ClientsMap = std::unordered_map<UserId, std::shared_ptr<WsClient>>;

    virtual ~Command() = default;

    virtual void operator()(const ClientsMap&) const = 0;

};

using FilterFunc = std::function<bool(const std::shared_ptr<WsClient>&)>;

inline Command::ClientsMap filter(FilterFunc filter, Command::ClientsMap clients) {
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
