#ifndef SERVER_HXX
#define SERVER_HXX

#include "adapters/crow/crow_server.hxx"
#include "adapters/repository/message_repository_adapter.hxx"
#include "adapters/repository/user_repository_adapter.hxx"
#include "adapters/repository/chat_repository_adapter.hxx"
#include "auth_service.hxx"
#include "database.hxx"
#include "crow.h"
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
    UserRepositoryAdapter userRepository;
    ChatRepositoryAdapter chatRepository;
    MessageRepositoryAdapter messageRepository;
    RouteManager routeManager;
    Database<pqxx::connection> dbHandle;

    const int port{8080};

    const std::string secret;
};

#endif
