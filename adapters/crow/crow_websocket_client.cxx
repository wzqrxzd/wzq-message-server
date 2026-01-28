#include "adapters/crow/crow_websocket_client.hxx"

adapter::CrowWebsocketClient::CrowWebsocketClient(crow::websocket::connection* conn) : conn(conn) {}

int adapter::CrowWebsocketClient::getUserId()
{
  return userId;
}

void adapter::CrowWebsocketClient::sendText(const std::string_view& sv)
{
  conn->send_text(std::string(sv));
}

void adapter::CrowWebsocketClient::insertChatId(const int& id)
{
  chatIds.insert(id);
}

void adapter::CrowWebsocketClient::eraseChatId(const int& id)
{
  chatIds.erase(id);
}

std::unordered_set<int> adapter::CrowWebsocketClient::getChatIds()
{
  return chatIds;
}
