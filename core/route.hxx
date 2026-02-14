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

inline std::regex getRegexString(std::string templatePath)
{
  return std::regex(std::regex_replace(templatePath, std::regex("<int>"), "(\\d+)"));
}

inline std::vector<std::string> getParamsArgs(const std::string& templatePath, const std::string& requestPath)
{
  const std::regex pattern = getRegexString(templatePath);
  std::vector<std::string> args;
  
  std::smatch match;

  if (std::regex_search(requestPath, match, pattern))
  {
    for (int i{1}; i < match.size(); i++)
      args.push_back(match[i].str());
  }

  return args;
}

template <typename... Args>
class VariadicRoute : public Route
{
  public:
    using TypedArgs = std::tuple<Args...>;

    VariadicRoute(const RouteInfo& info, const RouteContext& context) : Route(info, context) {}

    virtual ~VariadicRoute() = default;

    std::unique_ptr<http::Response> handleRequest(const http::Request& req);
    
    virtual std::unique_ptr<http::Response> handleTypedRequest(const http::Request& req, Args...) = 0;
  protected:
    friend class RouteManager;

    using Route::info;
    using Route::context;
};

template <typename T>
T fromString(const std::string&);

template <typename... Args, std::size_t... Is>
std::tuple<Args...> makeTupleImpl(const std::vector<std::string>& vec, std::index_sequence<Is...>)
{
  if (vec.size() != sizeof...(Args))
    throw std::runtime_error("template args not same as request args");

  return std::tuple<Args...>{
    fromString<Args>(vec[Is])...
  };
}

template <typename... Args>
std::tuple<Args...> makeTuple(const std::vector<std::string>& vec)
{
  return makeTupleImpl<Args...>(vec, std::index_sequence_for<Args...>{});
}

template <typename... Args>
std::unique_ptr<http::Response> VariadicRoute<Args...>::handleRequest(const http::Request& req)
{
  const std::vector<std::string> requestArgs = getParamsArgs(Route::info.path, std::string(req.url()));

  std::tuple<Args...> typedArgs = makeTuple<Args...>(requestArgs);

  return std::apply(
      [this, &req](std::decay_t<Args>... args) -> std::unique_ptr<http::Response> {
        return handleTypedRequest(req, args...);
      }, typedArgs
  );
}

template <>
inline int fromString(const std::string& str)
{
  return std::stoi(str);
}

template <>
inline std::string fromString(const std::string& str)
{
  return str;
}

#endif
