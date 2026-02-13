#include "user_repository_adapter.hxx"
#include "error.hxx"

void UserRepositoryAdapter::createUser(const UserFields& user) const
{
  ConnectionGuard guard(dbHandle);
  pqxx::work worker(guard.get());

  worker.exec_prepared("insert_user", user.username, user.password, user.name, user.description);
  worker.commit();
}

int UserRepositoryAdapter::getUserId(const std::string& username) const
{

  ConnectionGuard guard(dbHandle);

  pqxx::work worker(guard.get());

  pqxx::result result = worker.exec_prepared("find_user_by_username", username);

  if (result.empty())
    throw UserNotExistsException("User repository adapter result of find user is empty");

  int userId = result[0]["id"].as<int>();

  return userId;
}

void UserRepositoryAdapter::ensureUserExists(const std::string& username) const
{
  ConnectionGuard guard(dbHandle);

  pqxx::work worker(guard.get());
  pqxx::result resultUser = worker.exec_prepared("find_user", username);

  if(resultUser.size() == 0)
    throw AuthException(AuthError::InvalidCredentials, "wrong username");
}

void UserRepositoryAdapter::ensureUserNotExists(const std::string& username) const
{
  ConnectionGuard guard(dbHandle);

  pqxx::work worker(guard.get());
  pqxx::result resultUser = worker.exec_prepared("find_user", username);

  if(resultUser.size() != 0)
    throw AuthException(AuthError::UserAlreadyExist);
}

std::string UserRepositoryAdapter::getUserHashedPassword(const std::string& username) const
{
  ConnectionGuard guard(dbHandle);

  pqxx::work worker(guard.get());
  pqxx::result result = worker.exec_prepared("find_user", username);

  std::string storedHash = result[0]["password_hash"].c_str();

  return storedHash;
}

void UserRepositoryAdapter::changeUserData(const int userId, const UserFields& updatedFields) const
{
  ConnectionGuard guard(dbHandle);

  pqxx::work worker(guard.get());

  worker.exec_prepared("change_user_info",
      userId,
      updatedFields.username.c_str(),
      updatedFields.username.c_str(),
      updatedFields.description.c_str()
  );

  worker.commit();
}

std::unordered_set<int> UserRepositoryAdapter::getUserChats(const int userId) const
{
  ConnectionGuard guard(dbHandle);
  pqxx::work worker(guard.get());
  pqxx::result resultChats = worker.exec_prepared("get_user_chats", userId);

  std::unordered_set<int> chatIds;

  for (auto row : resultChats)
  {
    int chatId = row["id"].as<int>();
    chatIds.insert(chatId);
  }

  return chatIds;
}

UserFields UserRepositoryAdapter::getUserData(const int userId) const
{
  ConnectionGuard guard(dbHandle);
  pqxx::work worker(guard.get());
  pqxx::result resultUserFields = worker.exec_prepared("get_user_by_id", userId);

  return UserFields {
      .name = resultUserFields[0]["name"].as<std::string>(),
      .username = resultUserFields[0]["username"].as<std::string>(),
      .description = resultUserFields[0]["description"].as<std::string>(),
      .id = userId
  };
}
