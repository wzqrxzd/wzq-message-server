#ifndef CROW_CORE_ADAPTER
#define CROW_CORE_ADAPTER

#include "crow.h"
#include "route.hxx"
#include "types/HttpTypes.hxx"
#include "types/WebServer.hxx"

namespace adapter
{
  http::Method convertMethod(crow::HTTPMethod method);
  crow::HTTPMethod convertToCrowMethod(http::Method method);

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

  struct CrowResponse : public http::Response
  {
    public:
      explicit CrowResponse(std::unique_ptr<http::Response> res);
      ~CrowResponse() override;

      int code() const override;
      std::string_view body() const override;

      void setCode(const int& code) override;
      void setBody(const std::string_view& sv) override;

      crow::response getResponse();
    private:
      crow::response res;
  };

  struct CrowServer : public WebServer
  {
    public:
      CrowServer(crow::App<crow::CORSHandler>& app) : app(app) {};
      ~CrowServer() override;
      void addRoute(const std::string& path, http::Method method, std::function<std::unique_ptr<http::Response>(const http::Request&)> handler) override;
    private:
      crow::App<crow::CORSHandler>& app;
  };
}

#endif
