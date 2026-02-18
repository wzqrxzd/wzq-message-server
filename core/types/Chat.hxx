#ifndef CHAT_HXX
#define CHAT_HXX

#include <unordered_map>
#include "types/UserFields.hxx"
#include <string>

struct Chat
{
  int chatId;
  std::string chatName;
  std::unordered_map<int, UserFields> usersId;
};

#endif
