#ifndef ROUTE_HXX
#define ROUTE_HXX

#include "auth_service.hxx"
#include "websocket_notify_controller.hxx"
#include <map>
#include <regex>

struct RouteInfo
{
  std::string path;
  http::Method method;
};

struct RouteContext
{
  AuthService& auth;
  WebsocketNotifyController& ws;
};

class Route
{
  public:
    Route(const RouteInfo& info, const RouteContext& context) : info(info), context(context) {}
    virtual ~Route() = default;

    virtual std::unique_ptr<http::Response> handleRequest(const http::Request& req) = 0;
  protected:
    friend class RouteManager;

    const RouteInfo info;
    const RouteContext context;
};

class WebsocketRoute
{
  public:
    WebsocketRoute(const std::string& path, const RouteContext& context) : path(path), context(context) {}
    virtual ~WebsocketRoute() = default;

    virtual void onOpen(std::shared_ptr<WsClient> client) = 0;
    virtual void onMessage(std::shared_ptr<WsClient> client, const std::string_view& message) = 0;
    virtual void onClose(std::shared_ptr<WsClient> client, const std::string_view& reason) = 0;

  protected:
    friend class RouteManager;

    const std::string path;
    const RouteContext context;
};

#endif
