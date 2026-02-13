#include "chat_repository_adapter.hxx"

int ChatRepositoryAdapter::createChat(const Chat& chat) const 
{
  ConnectionGuard guard(dbHandle);

  pqxx::work worker(guard.get());

  int chatId = worker.exec_prepared("insert_chat", chat.chatName)[0]["id"].as<int>();

  return chatId;
}

void ChatRepositoryAdapter::deleteChat(const int chatId) const
{
  ConnectionGuard guard(dbHandle);

  pqxx::work worker(guard.get());

  worker.exec_prepared("delete_chat_messages", chatId);
  worker.exec_prepared("delete_chat_members", chatId);
  worker.exec_prepared("delete_chat", chatId);
}

void ChatRepositoryAdapter::insertUserToChat(const int userId, const int chatId) const
{
  ConnectionGuard guard(dbHandle);

  pqxx::work worker(guard.get());

  worker.exec_prepared("insert_chat_member", chatId, userId);
}

void ChatRepositoryAdapter::removeUserFromChat(const int userId, const int chat) const
{

}
