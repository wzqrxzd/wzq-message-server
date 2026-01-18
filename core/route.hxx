#ifndef ROUTE_HXX
#define ROUTE_HXX

#include "crow/middlewares/cors.h"
#include "database.hxx"
#include "auth_service.hxx"
#include "crow.h"
#include "websocket_controller.hxx"

class Route
{
  public:
    Route(const std::string& path, const http::Method& method, AuthService& auth, Database& db) : path(path), method(method), dbHandle(db), auth(auth) {}
    virtual std::unique_ptr<http::Response> handleRequest(const http::Request& req) = 0;
    virtual ~Route() = default;
  protected:
    friend class RouteManager;

    Database& dbHandle;
    AuthService& auth;

    std::string path;
    http::Method method;
};

class WsAccessRoute : public Route
{
  public:
    WsAccessRoute(const std::string& path, const http::Method& method, WebsocketController& ws, AuthService& auth, Database& db) : Route(path, method, auth, db), wsController(ws){}
    void isWebSocket(){};
  protected:
    WebsocketController& wsController;
};

#endif
