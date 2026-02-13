#ifndef ROUTE_MANAGER
#define ROUTE_MANAGER

#include <spdlog/spdlog.h>

#include "route.hxx"

#include "auth_service.hxx"
#include "database.hxx"
#include "websocket_notify_controller.hxx"

#include "types/WebServer.hxx"

#include "repositories/chat_repository.hxx"
#include "repositories/user_repository.hxx"


template <typename T>
concept HasDeps = requires {
  typename T::Deps;
};

template <typename T>
concept RouteConcept = std::is_base_of_v<Route, T> && HasDeps<T>;

template <typename T>
concept WebsocketRouteConcept = std::is_base_of_v<WebsocketRoute, T> && HasDeps<T>;

class RouteManager {
  public:
    RouteManager(WebServer& server, AuthService& auth, Database<pqxx::connection>& db, UserRepository& user, ChatRepository& chat);

    void setupRoutes();

    template <RouteConcept T>
    void addRoute();

    template <WebsocketRouteConcept T>
    void addWebsocketRoute();

  private:
    template<typename T>
    T& getRepository();

    template <typename T, std::size_t... Is>
    auto makeDepsTuple(std::index_sequence<Is...>);

    UserRepository& userRepo;
    ChatRepository& chatRepo;

    WebServer& server;
    AuthService& auth;
    Database<pqxx::connection>& db;
    WebsocketNotifyController wsController;

    std::vector<std::unique_ptr<Route>> routes;
    std::vector<std::unique_ptr<WebsocketRoute>> websocketRoutes;
};

template <>
inline UserRepository& RouteManager::getRepository<UserRepository&>() { return userRepo; }

template <>
inline ChatRepository& RouteManager::getRepository<ChatRepository&>() { return chatRepo; }

template <typename T, std::size_t... Is>
inline auto RouteManager::makeDepsTuple(std::index_sequence<Is...>)
{
  return std::tuple<std::tuple_element_t<Is, typename T::Deps>...>{
    getRepository<std::tuple_element_t<Is, typename T::Deps>>()...
  };
}

template <RouteConcept Route>
void RouteManager::addRoute() {
  RouteContext context(auth, wsController);

  constexpr size_t tupleLength = std::tuple_size_v<typename Route::Deps>;
  
  auto deps = makeDepsTuple<Route>(std::make_index_sequence<tupleLength>{}); 

  std::unique_ptr<Route> routePtr = std::apply(
      [&](auto&... deps)
      {
        return std::make_unique<Route>(context, deps...);
      },
      deps
  );

  spdlog::debug("added route: {}", routePtr->info.path);

  routes.push_back(std::move(routePtr));
};

template <WebsocketRouteConcept Route>
void RouteManager::addWebsocketRoute() {
  RouteContext context(auth, wsController);
  
  constexpr size_t tupleLength = std::tuple_size_v<typename Route::Deps>;
  
  auto deps = makeDepsTuple<Route>(std::make_index_sequence<tupleLength>{}); 

  std::unique_ptr<Route> routePtr = std::apply(
      [&](auto&... deps)
      {
        return std::make_unique<Route>(context, deps...); },
      deps
  );

  spdlog::debug("added route websocket: {}", routePtr->path);

  websocketRoutes.push_back(std::move(routePtr));
};

#endif
