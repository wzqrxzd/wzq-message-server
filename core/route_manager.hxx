#ifndef ROUTE_MANAGER
#define ROUTE_MANAGER

#include "auth_service.hxx"
#include "route.hxx"
#include "types/WebServer.hxx"
#include "websocket_notify_controller.hxx"
#include <spdlog/spdlog.h>

template <typename T>
concept RouteConcept = std::is_base_of_v<Route, T>;

template <typename T>
concept WebsocketRouteConcept = std::is_base_of_v<WebsocketRoute, T>;

class RouteManager {
  public:
    RouteManager(WebServer& server, AuthService& auth, Database<pqxx::connection>& db);

    void setupRoutes();
    template <RouteConcept T>
    void addRoute() {
      RouteContext context(db, auth, wsController);

      std::unique_ptr ptr = std::make_unique<T>(context);

      spdlog::debug("added route: {}", ptr->info.path);

      routes.push_back(std::move(ptr));
    };

    template <WebsocketRouteConcept T>
    void addWebsocketRoute() {
      RouteContext context(db, auth, wsController);

      std::unique_ptr ptr = std::make_unique<T>(context);

      spdlog::debug("added websocket route: {}", ptr->path);

      websocketRoutes.push_back(std::move(ptr));
    }

  private:
    WebServer& server;
    AuthService& auth;
    Database<pqxx::connection>& db;
    WebsocketNotifyController wsController;

    std::vector<std::unique_ptr<Route>> routes;
    std::vector<std::unique_ptr<WebsocketRoute>> websocketRoutes;
};

#endif
