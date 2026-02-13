#ifndef CHAT_REPOSITORY_HXX
#define CHAT_REPOSITORY_HXX

#include "types/Chat.hxx"

class ChatRepository {
  public:
    virtual ~ChatRepository() = default;

    virtual int createChat(const Chat& chat) const = 0;
    virtual void deleteChat(const int chatId) const = 0;

    virtual void insertUserToChat(const int userId, const int chatId) const = 0;
    virtual void removeUserFromChat(const int userId, const int chatId) const = 0;
};

#endif
