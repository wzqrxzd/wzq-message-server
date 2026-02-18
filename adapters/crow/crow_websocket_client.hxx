#ifndef CROW_WEBSOCKET_CLIENT_HXX
#define CROW_WEBSOCKET_CLIENT_HXX

#include "adapters/crow/crow_ns.hxx"
#include "crow/websocket.h"
#include "websocket_notify_controller.hxx"

class adapter::CrowWebsocketClient : public WsClient
{
  public:
    CrowWebsocketClient(crow::websocket::connection* conn);

    void setUserId(const int userId) override;
    int getUserId() override;

    void sendText(const std::string_view& sv) override;
    std::unordered_set<int> getChatIds() override;

    void insertChatId(const int& id) override;
    void eraseChatId(const int& id) override;

  private:
    int userId;
    std::unordered_set<int> chatIds;
    crow::websocket::connection* conn;
};

#endif
