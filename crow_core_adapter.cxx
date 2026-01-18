#include "crow_core_adapter.hxx"

adapter::CrowRequest::CrowRequest(const crow::request& req) : req{req} {};
adapter::CrowRequest::~CrowRequest() = default;

http::Method adapter::convertMethod(crow::HTTPMethod method)
{
  switch (method) {
    case crow::HTTPMethod::GET: return http::Method::GET;
    case crow::HTTPMethod::DELETE: return http::Method::DELETE;
    case crow::HTTPMethod::HEAD: return http::Method::HEAD;
    case crow::HTTPMethod::POST: return http::Method::POST;
    case crow::HTTPMethod::PUT: return http::Method::PUT;
    case crow::HTTPMethod::OPTIONS: return http::Method::OPTIONS;
    case crow::HTTPMethod::TRACE: return http::Method::TRACE;
    case crow::HTTPMethod::PATCH: return http::Method::PATCH;
    default: return http::Method::GET;
  }
}

crow::HTTPMethod adapter::convertToCrowMethod(const http::Method method)
{
  switch (method) 
  {
    case http::Method::GET: return crow::HTTPMethod::GET;
    case http::Method::DELETE: return crow::HTTPMethod::DELETE;
    case http::Method::HEAD: return crow::HTTPMethod::HEAD;
    case http::Method::POST: return crow::HTTPMethod::POST;
    case http::Method::PUT: return crow::HTTPMethod::PUT;
    case http::Method::OPTIONS: return crow::HTTPMethod::OPTIONS;
    case http::Method::TRACE: return crow::HTTPMethod::TRACE;
    case http::Method::PATCH: return crow::HTTPMethod::PATCH;
    default: return crow::HTTPMethod::GET;
  }
}

http::Method adapter::CrowRequest::method() const
{
  return convertMethod(req.method);
}

std::string_view adapter::CrowRequest::url() const
{
  return req.url;
}

std::string_view adapter::CrowRequest::body() const
{
  return req.body;
}

std::optional<std::string_view> adapter::CrowRequest::getHeader(const std::string_view& key) const
{
  auto it = req.headers.find(std::string(key));
  if (it == req.headers.end())
    return std::nullopt;
  return it->second;
}

adapter::CrowResponse::CrowResponse(std::unique_ptr<http::Response> response)
{
  res.code = response->code();
  res.body = response->body();
}

adapter::CrowResponse::~CrowResponse() = default;

int adapter::CrowResponse::code() const
{
  return res.code;
}

std::string_view adapter::CrowResponse::body() const
{
  return res.body;
}

void adapter::CrowResponse::setCode(const int& code)
{
  res.code = code;
}

void adapter::CrowResponse::setBody(const std::string_view& sv)
{
  res.body = std::string(sv);
}

crow::response adapter::CrowResponse::getResponse()
{
  return std::move(res);
}

adapter::CrowServer::~CrowServer() = default;

void adapter::CrowServer::addRoute(const std::string& path, http::Method method, std::function<std::unique_ptr<http::Response>(const http::Request&)> handler)
{
  app.route_dynamic(path)
  .methods(adapter::convertToCrowMethod(method))
  ([handler](const crow::request& req) {
    adapter::CrowRequest reqAdapt(req);
    adapter::CrowResponse response(handler(reqAdapt));
    return response.getResponse();
  });
}

