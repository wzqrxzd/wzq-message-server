#include "chat_repository_adapter.hxx"
#include "error.hxx"

int ChatRepositoryAdapter::createChat(const Chat& chat) const 
{
  ConnectionGuard guard(dbHandle);

  pqxx::work worker(guard.get());

  int chatId = worker.exec_prepared("insert_chat", chat.chatName)[0]["id"].as<int>();

  worker.commit();

  return chatId;
}

void ChatRepositoryAdapter::deleteChat(const int chatId) const
{
  ConnectionGuard guard(dbHandle);

  pqxx::work worker(guard.get());

  worker.exec_prepared("delete_chat_messages", chatId);
  worker.exec_prepared("delete_chat_members", chatId);
  worker.exec_prepared("delete_chat", chatId);

  worker.commit();
}

void ChatRepositoryAdapter::insertUserToChat(const int userId, const int chatId) const
{
  ConnectionGuard guard(dbHandle);

  pqxx::work worker(guard.get());

  worker.exec_prepared("insert_chat_member", chatId, userId);

  worker.commit();
}

void ChatRepositoryAdapter::removeUserFromChat(const int userId, const int chat) const
{

}

void ChatRepositoryAdapter::ensureUserInChat(const int userId, const int chatId) const
{
  ConnectionGuard guard(dbHandle);

  pqxx::work worker(guard.get());
  pqxx::result result = worker.exec_prepared("check_user_in_chat", chatId, userId);

  if (result.empty())
    throw AuthException(AuthError::PermissionDenied);
}

std::vector<Chat> ChatRepositoryAdapter::getUserChats(const int userId) const
{
  ConnectionGuard guard(dbHandle);

  pqxx::work worker(guard.get());
  pqxx::result resultChats = worker.exec_prepared("get_user_chats", userId);

  std::vector<Chat> userChats;

  for (auto row : resultChats)
  {
    Chat chat {
      .chatId = row["id"].as<int>(),
      .chatName = row["name"].as<std::string>()
    };

    userChats.push_back(chat);
  }

  return userChats;
}
