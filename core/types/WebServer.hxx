#ifndef WEBSERVER_HXX
#define WEBSERVER_HXX

#include "types/HttpTypes.hxx"
#include <functional>
#include <memory>
#include <string>

class WebServer
{
  public:
    virtual ~WebServer() = default;
    virtual void addRoute(const std::string& path, http::Method method, std::function<std::unique_ptr<http::Response>(const http::Request&)> handler) = 0;
};

#endif
