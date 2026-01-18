#include "route_manager.hxx"
#include <spdlog/spdlog.h>
#include "utils.hxx"

RouteManager::RouteManager(WebServer& server, AuthService& auth, Database& db) : server(server), auth(auth), db(db), wsController() {}

void RouteManager::setupRoutes()
{
  for (const auto& route : routes)
  {
     server.addRoute(
         route->path,
         route->method,
         [handler = route.get()](const http::Request& req) {
            return handler->handleRequest(req);
         }
     );
  }
}
