#include "adapters/crow/crow_response.hxx"

adapter::CrowResponse::CrowResponse(std::unique_ptr<http::Response> response)
{
  res.code = response->code();
  res.body = response->body();
}

int adapter::CrowResponse::code() const
{
  return res.code;
}

std::string_view adapter::CrowResponse::body() const
{
  return res.body;
}

void adapter::CrowResponse::setCode(const int code)
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
