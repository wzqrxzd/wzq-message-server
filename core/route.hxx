#ifndef ROUTE_HXX
#define ROUTE_HXX

#include "database.hxx"
#include "auth_service.hxx"
#include "websocket_notify_controller.hxx"

struct RouteInfo
{
  std::string path;
  http::Method method;
};

struct RouteContext
{
  Database& dbHandle;
  AuthService& auth;
  WebsocketNotifyController& ws;
};

class Route
{
  public:
    Route(RouteInfo info, RouteContext context) : info(info), context(context) {}
    virtual std::unique_ptr<http::Response> handleRequest(const http::Request& req) = 0;
    virtual ~Route() = default;
  protected:
    friend class RouteManager;

    RouteInfo info;
    RouteContext context;
};

class WebsocketRoute
{
  public:
    WebsocketRoute(const std::string& path, RouteContext context) : path(path), context(context) {}
    virtual void onOpen(std::shared_ptr<WsClient> client) = 0;
    virtual void onMessage(std::shared_ptr<WsClient> client, const std::string_view& message) = 0;
    virtual void onClose(std::shared_ptr<WsClient> client, const std::string_view& reason) = 0;
  private:
    friend class RouteManager;

    const std::string path;
    RouteContext context;
};

#endif
