#ifndef TESTWSROUTE_HXX
#define TESTWSROUTE_HXX

#include "repositories/user_repository.hxx"

#include "route.hxx"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

class NotifyWebsocketRoute : public WebsocketRoute
{
  public:
    using Deps = std::tuple<UserRepository&>;

    NotifyWebsocketRoute(RouteContext context, UserRepository& userRepository) : WebsocketRoute("/ws", context), userRepository(userRepository) {}

    void onOpen(std::shared_ptr<WsClient> client) override;
    void onMessage(std::shared_ptr<WsClient> client, const std::string_view& message) override;
    void onClose(std::shared_ptr<WsClient> client, const std::string_view& reason) override;
  private:
    const UserRepository& userRepository;
};

#endif
