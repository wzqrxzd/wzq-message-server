#ifndef MESSAGE_REPOSITORY_ADAPTER_HXX
#define MESSAGE_REPOSITORY_ADAPTER_HXX

#include "database.hxx"
#include "repositories/message_repository.hxx"

class MessageRepositoryAdapter : public MessageRepository
{
  public:
    MessageRepositoryAdapter(Database<pqxx::connection>& dbHandle) : dbHandle(dbHandle) {}

    int sendMessage(const Message& message) const override;
    void deleteMessage(const int messageId) const override;

    int getSenderId(const int messageId) const override;

    std::vector<Message> getChatHistory(const int chatId, const int limit = 50, const int offset = 0) const override;
  private:
    Database<pqxx::connection>& dbHandle;
};

#endif
