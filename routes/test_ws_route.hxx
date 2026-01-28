#ifndef TESTWSROUTE_HXX
#define TESTWSROUTE_HXX

#include "route.hxx"
#include <spdlog/spdlog.h>

class TestWebsocketRoute : public WebsocketRoute
{
  public:
    TestWebsocketRoute(RouteContext context) : WebsocketRoute("/ws", context) {};

    void onOpen(std::shared_ptr<WsClient> client) override {
      spdlog::debug("Opened websocket connection");
      client->sendText("test");
    };

    void onMessage(std::shared_ptr<WsClient> client, const std::string_view& message) override {
      client->sendText("test");
    };

    void onClose(std::shared_ptr<WsClient> client, const std::string_view& reason) override {
      client->sendText("test");
    };
};

#endif
