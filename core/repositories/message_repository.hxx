#ifndef MESSAGE_REPOSITORY_HXX
#define MESSAGE_REPOSITORY_HXX

#include "types/Message.hxx"
#include <vector>

class MessageRepository {
  public:
    virtual ~MessageRepository() = default;

    virtual int sendMessage(const Message& message) const = 0;
    virtual void deleteMessage(const int messageId) const = 0;

    virtual int getSenderId(const int messageId) const = 0;

    virtual std::vector<Message> getChatHistory(const int chatId, const int limit = 50, const int offset = 0) const = 0;
};

#endif
