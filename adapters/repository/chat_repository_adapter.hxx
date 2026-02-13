#ifndef CHAT_REPOSITORY_ADAPTER_HXX
#define CHAT_REPOSITORY_ADAPTER_HXX

#include "database.hxx"
#include "repositories/chat_repository.hxx"

class ChatRepositoryAdapter : public ChatRepository
{
  public:
    explicit ChatRepositoryAdapter(Database<pqxx::connection> &dbHandle)
        : dbHandle(dbHandle) {}
    int createChat(const Chat &chat) const override;
    void deleteChat(const int chatId) const override;

    void insertUserToChat(const int userId, const int chatId) const override;
    void removeUserFromChat(const int userId, const int chatId) const override;
  private:
    Database<pqxx::connection>& dbHandle;
};

#endif
