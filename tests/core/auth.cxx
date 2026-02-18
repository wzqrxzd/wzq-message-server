#include <fmt/format.h>
#include <gtest/gtest.h>
#include "auth_service.hxx"
#include "types/HttpTypes.hxx"

struct TestRequest : public http::Request {
  public:
    TestRequest() {};

    http::Method method() const override {
      return reqMethod;
    };

    std::string_view url() const override {
      return "";
    };

    std::string_view body() const override {
      return reqBody;
    };

    std::optional<std::string> getHeader(const std::string_view key) const override {
      auto it = headers.find(std::string(key));
      if (it == headers.end())
        return std::nullopt;

      return it->second;
    };

    void setHeader(const std::string& header, const std::string& value) { headers[header] = value; }
  private:
    static constexpr http::Method reqMethod = http::Method::GET;
    std::unordered_map<std::string, std::string> headers;
    std::string reqBody;
};

TEST(Auth, VerifyPasswordHash)
{
  AuthService auth;
  const std::string password = "pass";

  const std::string hashedPassword = auth.hashPassword(password);

  EXPECT_TRUE(auth.verifyPassword(hashedPassword, password));
}

TEST(Auth, Authorize)
{
  AuthService auth;
  const std::string username("John_Smith");

  std::string token = auth.generateJWT(username);

  TestRequest req;
  req.setHeader("Authorization", fmt::format("Bearer {}", token));

  EXPECT_TRUE(auth.authorize(req) == username);
}

TEST(Auth, AuthorizeRequest)
{
  AuthService auth;

  const std::string username("John_Smith");

  std::string token = auth.generateJWT(username);

  TestRequest req;
  req.setHeader("Authorization", fmt::format("Bearer {}", token));

  EXPECT_TRUE(auth.authorizeRequest(req));
}

TEST(Auth, VerifyUsernameFromToken)
{
  AuthService auth;
  const std::string username("John_Smith");

  std::string token = auth.generateJWT(username);

  EXPECT_TRUE(auth.getUsernameFromToken(token) == username);
}
