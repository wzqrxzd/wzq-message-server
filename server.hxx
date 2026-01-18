#ifndef SERVER_HXX
#define SERVER_HXX

#include "auth_service.hxx"
#include "crow_core_adapter.hxx"
#include "database.hxx"
#include "crow/middlewares/cors.h"
#include "route.hxx"
#include "route_manager.hxx"
#include <fmt/format.h>
#include <pqxx/pqxx>
#include <spdlog/spdlog.h>

class Server
{
  public:
    Server();
    void run();
  private:
    void setupRoutes();

    crow::App<crow::CORSHandler> app;
    adapter::CrowServer server;
    AuthService auth;
    RouteManager routeManager;
    Database dbHandle;

    const int port{8080};

    const std::string secret;
};

#endif
