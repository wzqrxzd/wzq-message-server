#include "types/HttpTypes.hxx"
#include "adapters/crow/crow_ns.hxx"

inline http::Method adapter::convertMethod(crow::HTTPMethod method)
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

inline crow::HTTPMethod adapter::convertToCrowMethod(const http::Method method)
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
