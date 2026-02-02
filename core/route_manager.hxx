#ifndef ROUTE_MANAGER
#define ROUTE_MANAGER

#include "auth_service.hxx"
#include "route.hxx"
#include "types/WebServer.hxx"
#include "websocket_notify_controller.hxx"
#include <spdlog/spdlog.h>

template <typename T>
concept RouteConcept = requires(T t, const http::Request& req) {
  { t.handleRequest(req) } -> std::same_as<std::unique_ptr<http::Response>>;
};

template <typename T>
concept WebsocketRouteConcept = requires(T t, std::shared_ptr<WsClient> client, const std::string_view& sv) {
  { t.onOpen(client) } -> std::same_as<void>;
  { t.onMessage(client, sv) } -> std::same_as<void>;
  { t.onClose(client, sv) } -> std::same_as<void>;
};


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
