#ifndef HTTP_TYPES
#define HTTP_TYPES

#include <optional>
#include <string>
#include <unordered_map>

namespace http {
  enum class Method {GET, HEAD, OPTIONS, TRACE, PUT, DELETE, POST, PATCH, CONNECT};

  struct Request
  {
    virtual ~Request() = default;

    virtual Method method() const = 0;
    virtual std::string_view url() const = 0;
    virtual std::string_view body() const = 0;

    virtual std::optional<std::string_view> getHeader(const std::string_view& key) const = 0;
  };
};

#endif
