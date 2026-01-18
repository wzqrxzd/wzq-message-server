#ifndef ROUTE_MANAGER
#define ROUTE_MANAGER

#include "auth_service.hxx"
#include "route.hxx"
#include "types/WebServer.hxx"
#include "websocket_controller.hxx"

template <typename T>
concept RouteConcept = requires(T t, const http::Request& req) {
  { t.handleRequest(req) } -> std::same_as<std::unique_ptr<http::Response>>;
};

template <typename T>
concept WsConcept = requires(T t) {
  { t.isWebSocket() } -> std::same_as<void>;
};

class RouteManager {
  public:
    RouteManager(WebServer& server, AuthService& auth, Database& db);

    void setupRoutes();
    template <RouteConcept T>
    void addRoute() {
      std::unique_ptr<Route> ptr = nullptr;

      if constexpr(WsConcept<T>) {
        ptr = std::make_unique<T>(wsController, auth, db);
      } else {
        ptr = std::make_unique<T>(auth, db);
      }

      routes.push_back(std::move(ptr));
    };
  private:
    WebServer& server;
    AuthService& auth;
    Database& db;
    WebsocketController wsController;

    std::vector<std::unique_ptr<Route>> routes;
};

#endif
