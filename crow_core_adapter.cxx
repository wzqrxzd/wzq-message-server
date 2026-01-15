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
