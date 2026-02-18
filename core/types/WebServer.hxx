#ifndef WEBSERVER_HXX
#define WEBSERVER_HXX

#include "types/HttpTypes.hxx"
#include "websocket_notify_controller.hxx"
#include <functional>
#include <memory>
#include <string>

class WebServer
{
  public:
    using onOpen = std::function<void(std::shared_ptr<WsClient>)>;
    using onMessage = std::function<void(std::shared_ptr<WsClient>, const std::string_view&)>;
    using onClose = std::function<void(std::shared_ptr<WsClient>, const std::string_view&)>;

    virtual ~WebServer() = default;
    virtual void addRoute(const std::string& path, http::Method method, std::function<std::unique_ptr<http::Response>(const http::Request&)> handler) = 0;
    virtual void addWebsocketRoute(const std::string& path, onOpen openFunc, onMessage messageFunc, onClose closeFunc) = 0;
};

#endif
