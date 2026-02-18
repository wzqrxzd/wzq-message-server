#ifndef DATABASE_HXX
#define DATABASE_HXX

#include <pqxx/pqxx>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <cstdint>

template <typename Connection>
concept ConnectionConcept = requires(Connection conn, const std::string test) {
  { conn.prepare(test, test) } -> std::same_as<void>;
};

template <ConnectionConcept Connection>
class ConnectionGuard;

template <ConnectionConcept Connection>
class Database
{
  public:
    using dbConnection = std::shared_ptr<Connection>;

    Database(const std::string& user, const std::string& name, const std::string& pass, uint16_t poolSize);
    ~Database();

  private:
    friend ConnectionGuard<Connection>;

    dbConnection acquireConnection();
    void releaseConnection(dbConnection conn);

    dbConnection prepareDB(dbConnection conn);

    std::queue<dbConnection> connPool;
    std::mutex mtx;
    std::condition_variable cv;

    const std::string connectionString;
};

template <ConnectionConcept Connection>
class ConnectionGuard
{
  public:
    ConnectionGuard(Database<Connection>& db) : db(db), conn(db.acquireConnection()) {};
    ~ConnectionGuard() { db.releaseConnection(conn); };
    Connection& get() { return *conn; }
    Connection* operator->() { return conn.get(); }
  private:
    Database<Connection>& db;
    Database<Connection>::dbConnection conn;
};

template <ConnectionConcept Connection>
ConnectionGuard(Database<Connection>&) -> ConnectionGuard<Connection>;

#include "database.inl"

#endif
