#ifndef USER_REPOSITORY_HXX
#define USER_REPOSITORY_HXX

#include "types/UserFields.hxx"
#include <string>
#include <unordered_set>

class UserRepository {
  public:
    virtual ~UserRepository() = default;

    virtual void createUser(const UserFields& user) const = 0;

    virtual int getUserId(const std::string& username) const = 0;

    virtual void ensureUserExists(const std::string& username) const = 0;
    virtual void ensureUserNotExists(const std::string& username) const = 0;

    virtual std::string getUserHashedPassword(const std::string& username) const = 0;

    virtual UserFields getUserData(const int userId) const = 0;
    virtual void changeUserData(const int userId, const UserFields& user) const = 0;

    virtual std::unordered_set<int> getUserChats(const int userId) const = 0;
};

#endif
