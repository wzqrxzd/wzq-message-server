#ifndef CROW_RESPONSE_HXX
#define CROW_RESPONSE_HXX

#include "adapters/crow/crow_ns.hxx"
#include "crow/http_response.h"
#include "types/HttpTypes.hxx"
#include <memory>

struct adapter::CrowResponse : public http::Response
{
  public:
    explicit CrowResponse(std::unique_ptr<http::Response> res);

    int code() const override;
    std::string_view body() const override;

    void setCode(const int& code) override;
    void setBody(const std::string_view& sv) override;

    crow::response getResponse();
  private:
    crow::response res;
};

#endif
