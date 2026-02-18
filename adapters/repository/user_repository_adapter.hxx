#ifndef USER_REPOSITORY_ADAPTER_HXX
#define USER_REPOSITORY_ADAPTER_HXX

#include "database.hxx"
#include "repositories/user_repository.hxx"
#include "types/UserFields.hxx"
#include <string>

class UserRepositoryAdapter : public UserRepository {
  public:
    explicit UserRepositoryAdapter(Database<pqxx::connection>& dbHandle)
        : dbHandle(dbHandle) {}

    void createUser(const UserFields& user) const override;

    int getUserId(const std::string& username) const override;

    void ensureUserExists(const std::string& username) const override;
    void ensureUserNotExists(const std::string& username) const override;

    std::string getUserHashedPassword(const std::string& username) const override;


    UserFields getUserData(const int userId) const override;
    void changeUserData(const int userId, const UserFields& user) const override;
    std::unordered_set<int> getUserChats(const int userId) const override;

  private:
    Database<pqxx::connection>& dbHandle;
};

#endif
