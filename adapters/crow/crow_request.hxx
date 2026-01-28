#ifndef CROW_REQUEST_HXX
#define CROW_REQUEST_HXX

#include "adapters/crow/crow_ns.hxx"
#include "crow/http_request.h"
#include "types/HttpTypes.hxx"

struct adapter::CrowRequest : public http::Request
{
  public:
    CrowRequest(const crow::request& req);
    http::Method method() const override;
    std::string_view url() const override;
    std::string_view body() const override;

    std::optional<std::string_view> getHeader(const std::string_view& key) const override;
  private:
    const crow::request& req;
};

#endif
