#ifndef USER_FIELDS_HXX
#define USER_FIELDS_HXX

#include <optional>
#include <string>

struct UserFields {
  std::string name;
  std::string username;
  std::string password;
  std::string description;
  int id;
};

#endif
