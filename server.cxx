#include "crow.h"
#include <spdlog/spdlog.h>
#include <libpq-fe.h>
#include <argon2.h>
#include "crow/common.h"
#include "env_utils.hxx"
#include "server.hxx"
#include <sodium.h>
// #include "routes/chats_messages_route.hxx"
// #include "routes/create_chat_route.hxx"
// #include "routes/delete_message_route.hxx"
// #include "routes/insert_member_route.hxx"
#include "routes/chat_messages_route.hxx"
#include "routes/create_chat_route.hxx"
#include "routes/delete_chat_route.hxx"
#include "routes/delete_message_route.hxx"
#include "routes/insert_member_route.hxx"
#include "routes/register_route.hxx"
#include "routes/login_route.hxx"
// #include "routes/send_message_route.hxx"
// #include "routes/chats_route.hxx"
// #include "routes/user_info_route.hxx"
// #include "routes/user_update_info_route.hxx"
// #include "routes/ws_route.hxx"
#include "routes/notify_ws_route.hxx"

#include "repositories/user_repository.hxx"
#include "repositories/chat_repository.hxx"
#include "routes/send_message_route.hxx"
#include "routes/user_info_route.hxx"
#include "routes/user_update_info_route.hxx"
#include "routes/chats_route.hxx"

Server::Server() :
  dbHandle(
    env_utils::getEnvVar("POSTGRES_USER"),
    env_utils::getEnvVar("POSTGRES_DB"),
    env_utils::getEnvVar("POSTGRES_PASSWORD"),
    4
  ),
  userRepository(dbHandle),
  chatRepository(dbHandle),
  messageRepository(dbHandle),

  secret(env_utils::getEnvVar("JWT_SECRET")),
  server(app),
  auth(),
  routeManager(server, auth, dbHandle, static_cast<UserRepository&>(userRepository), static_cast<ChatRepository&>(chatRepository), static_cast<MessageRepository&>(messageRepository))
{
  if (sodium_init() < 0) {
      throw std::runtime_error("libsodium init failed");
  }

  auto& cors = app.get_middleware<crow::CORSHandler>();
  cors.global()
    .origin("*")
    .methods(
        crow::HTTPMethod::Get,
        crow::HTTPMethod::Patch,
        crow::HTTPMethod::Post,
        crow::HTTPMethod::Put,
        crow::HTTPMethod::Delete,
        crow::HTTPMethod::Options
    )
    .headers("Content-Type, Authorization")
    .allow_credentials();

  spdlog::set_level(spdlog::level::debug);
  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");

  setupRoutes();
}

void Server::run() {
  app.port(port).multithreaded().run();
}

void Server::setupRoutes()
{
  spdlog::info("setup routes start");

  // routeManager.addRoute<WSRoute>();
  // routeManager.addRoute<LoginRoute>();
  // routeManager.addRoute<RegisterRoute>();
  // routeManager.addRoute<SendMessageRoute>();
  // routeManager.addRoute<ChatsRoute>();
  // routeManager.addRoute<ChatsMessagesRoute>();
  // routeManager.addRoute<CreateChatRoute>();
  // routeManager.addRoute<InsertMemberRoute>();
  // routeManager.addRoute<DeleteMessageRoute>();
  // routeManager.addRoute<UserInfoRoute>();
  // routeManager.addRoute<UserUpdateInfoRoute>();

  routeManager.addWebsocketRoute<NotifyWebsocketRoute>();
  routeManager.addRoute<LoginRoute>();
  routeManager.addRoute<RegisterRoute>();
  routeManager.addRoute<UserInfoRoute>();
  routeManager.addRoute<UserUpdateInfoRoute>();
  routeManager.addRoute<SendMessageRoute>();
  routeManager.addRoute<ChatMessagesRoute>();
  routeManager.addRoute<CreateChatRoute>();
  routeManager.addRoute<ChatsRoute>();
  routeManager.addRoute<DeleteChatRoute>();
  routeManager.addRoute<DeleteMessageRoute>();
  routeManager.addRoute<InsertMemberRoute>();

  routeManager.setupRoutes();
}
  
int main() {
  Server m;
  m.run();
}

