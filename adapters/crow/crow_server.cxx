#include "adapters/crow/crow_server.hxx"
#include "adapters/crow/crow_response.hxx"
#include "adapters/crow/crow_request.hxx"
#include "adapters/crow/crow_websocket_client.hxx"
#include "adapters/crow/crow_utils.hxx"
#include <spdlog/spdlog.h>

void adapter::CrowServer::addRoute(const std::string& path, http::Method method, std::function<std::unique_ptr<http::Response>(const http::Request&)> handler)
{
  app.route_dynamic(path)
  .methods(adapter::convertToCrowMethod(method))
  ([handler](const crow::request& req) {
    adapter::CrowRequest reqAdapt(req);
    adapter::CrowResponse response(handler(reqAdapt));
    return response.getResponse();
  });
}

void adapter::CrowServer::addWebsocketRoute(const std::string& path, onOpen openFunc, onMessage messageFunc, onClose closeFunc)
{
  app.route_dynamic(path)
    .websocket(&this->app)
    .onopen([openFunc](crow::websocket::connection& conn) {
        auto client = std::make_shared<adapter::CrowWebsocketClient>(&conn);
        openFunc(client);
    })
    .onclose([closeFunc](crow::websocket::connection& conn, const std::string& reason, uint16_t code){
        auto client = std::make_shared<adapter::CrowWebsocketClient>(&conn);
        closeFunc(client, reason);
    })
    .onmessage([messageFunc](crow::websocket::connection& conn, const std::string& message, bool){
        auto client = std::make_shared<adapter::CrowWebsocketClient>(&conn);
        messageFunc(client, message);
    });
}
