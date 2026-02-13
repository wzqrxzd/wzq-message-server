#ifndef HTTP_TYPES
#define HTTP_TYPES

#include <optional>
#include <string>

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

  struct Response
  {
    virtual ~Response() = default;

    virtual int code() const = 0;
    virtual std::string_view body() const = 0;

    virtual void setCode(const int code) = 0;
    virtual void setBody(const std::string_view& sv) = 0;
  };

  struct CoreResponse : public Response
  {
    public:
      CoreResponse() {};

      CoreResponse(const CoreResponse &) = default;
      CoreResponse(CoreResponse &&) = default;
      CoreResponse &operator=(const CoreResponse &) = default;
      CoreResponse &operator=(CoreResponse &&) = default;

      int code() const override { return responseCode; };
      std::string_view body() const override { return responseBody; };

      void setCode(const int code) override { responseCode = code; };
      void setBody(const std::string_view& sv) override { responseBody = std::string(sv); };
    private:
      int responseCode = 200;
      std::string responseBody = "";
  };
};

#endif
