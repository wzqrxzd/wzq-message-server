#include "route_manager.hxx"
#include <spdlog/spdlog.h>

RouteManager::RouteManager(WebServer& server, AuthService& auth, Database<pqxx::connection>& db, UserRepository& user, ChatRepository& chat) : server(server), auth(auth), db(db), userRepo(user), chatRepo(chat) {}

void RouteManager::setupRoutes()
{
  for (const auto& route : routes)
  {
     server.addRoute(
         route->info.path,
         route->info.method,
         [handler = route.get()](const http::Request& req) {
            return handler->handleRequest(req);
         }
     );
  }
  for (const auto& route : websocketRoutes)
  {
    server.addWebsocketRoute(
        route->path,
        [handler = route.get()](std::shared_ptr<WsClient> client) {
          handler->onOpen(client);
        },
        [handler = route.get()](std::shared_ptr<WsClient> client, const std::string_view& sv) {
          handler->onMessage(client, sv);
        },
        [handler = route.get()](std::shared_ptr<WsClient> client, const std::string_view& sv) {
          handler->onClose(client, sv);
        }
    );
  }
}
