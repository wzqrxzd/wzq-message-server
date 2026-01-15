#ifndef CROW_CORE_ADAPTER
#define CROW_CORE_ADAPTER

#include "crow.h"
#include "types/HttpTypes.hxx"

namespace adapter
{
  http::Method convertMethod(crow::HTTPMethod method);


  struct CrowRequest : public http::Request
  {
    public:
      CrowRequest(const crow::request& req);
      ~CrowRequest() override;
      http::Method method() const override;
      std::string_view url() const override;
      std::string_view body() const override;

      std::optional<std::string_view> getHeader(const std::string_view& key) const override;
    private:
      const crow::request& req;
  };
}

#endif
