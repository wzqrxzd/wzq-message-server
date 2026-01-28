#ifndef TEST_ROUTE_HXX
#define TEST_ROUTE_HXX

#include "route.hxx"

class TestRoute : public Route
{
  public:
    TestRoute(RouteContext context) : Route(RouteInfo("/test", http::Method::POST), context) {}
    std::unique_ptr<http::Response> handleRequest(const http::Request& req) override
    {
      http::CoreResponse response;
      response.setCode(202);
      response.setBody("test");

      return std::make_unique<http::CoreResponse>(response);
    }
};

#endif
