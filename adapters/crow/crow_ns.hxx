#include "crow/common.h"
#include "types/HttpTypes.hxx"

namespace adapter
{
  struct CrowRequest;
  struct CrowResponse;
  class CrowServer;
  class CrowWebsocketClient;

  http::Method convertMethod(crow::HTTPMethod method);
  crow::HTTPMethod convertToCrowMethod(const http::Method method);
};
