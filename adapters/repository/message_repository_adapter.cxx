#include "message_repository_adapter.hxx"

int MessageRepositoryAdapter::sendMessage(const Message& message) const
{
  ConnectionGuard guard(dbHandle);
  pqxx::work worker(guard.get());

  pqxx::result resultId = worker.exec_prepared("insert_message",
      message.chatId,
      message.senderId,
      message.content
  );

  int messageId = resultId[0]["id"].as<int>();
  worker.commit();

  return messageId;
}

void MessageRepositoryAdapter::deleteMessage(const int messageId) const
{

}

std::vector<Message> MessageRepositoryAdapter::getChatHistory(const int chatId, const int limit, const int offset) const
{
  ConnectionGuard guard(dbHandle);

  pqxx::work worker(guard.get());

  pqxx::result resultChatMessages = worker.exec_prepared("get_chat_messages", chatId);

  std::vector<Message> chatMessages;

  for (auto row : resultChatMessages)
  {
    Message msg;
    msg.messageId = row["id"].as<int>();
    msg.senderId = row["sender_id"].as<int>();
    msg.username = row["sender_name"].as<std::string>();
    msg.content = row["content"].as<std::string>();

    chatMessages.push_back(msg);
  }

  return chatMessages;
}

int MessageRepositoryAdapter::getSenderId(const int messageId) const
{
  ConnectionGuard guard(dbHandle);

  pqxx::work worker(guard.get());

  pqxx::result resultSenderId = worker.exec_prepared("find_user_by_message", messageId);

  int senderId = resultSenderId[0]["sender_id"].as<int>();

  return senderId;
}
