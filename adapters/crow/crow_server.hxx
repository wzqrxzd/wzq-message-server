#ifndef CROW_SERVER_HXX
#define CROW_SERVER_HXX

#include "adapters/crow/crow_ns.hxx"
#include "crow/middlewares/cors.h"
#include "types/WebServer.hxx"
#include "crow.h"

struct adapter::CrowServer : public WebServer
{
  public:
    CrowServer(crow::App<crow::CORSHandler>& app) : app(app) {};
    void addRoute(const std::string& path, http::Method method, std::function<std::unique_ptr<http::Response>(const http::Request&)> handler) override;
    void addWebsocketRoute(const std::string& path, onOpen openFunc, onMessage messageFunc, onClose closeFunc) override;
  private:
    crow::App<crow::CORSHandler>& app;
};

#endif
