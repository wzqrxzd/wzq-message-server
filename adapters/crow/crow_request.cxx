#include "adapters/crow/crow_request.hxx"
#include "adapters/crow/crow_utils.hxx"
#include "types/HttpTypes.hxx"

adapter::CrowRequest::CrowRequest(const crow::request& req) : req(req) {}

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

std::optional<std::string> adapter::CrowRequest::getHeader(const std::string_view key) const
{
  auto it = req.headers.find(std::string(key));
  if (it == req.headers.end())
    return std::nullopt;
  return it->second;
}
