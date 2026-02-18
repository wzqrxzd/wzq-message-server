#ifndef WSCLIENT_HXX
#define WSCLIENT_HXX

#include <string_view>
#include <unordered_set>
typedef int UserId;

class WsClient
{
  public:
    virtual ~WsClient() = default;

    virtual void setUserId(const int userId) = 0;
    virtual int getUserId() = 0;

    virtual void sendText(const std::string_view& sv) = 0;

    virtual std::unordered_set<int> getChatIds() = 0;
    virtual void insertChatId(const UserId& id) = 0;
    virtual void eraseChatId(const UserId& id) = 0;
};

#endif
