#include <gtest/gtest.h>
#include "database.hxx"
#include <atomic>
#include <thread>
#include <chrono>

class FakeConnection
{
  public:
    static inline std::atomic<int> aliveCount{0};

    FakeConnection(const FakeConnection &) = delete;
    FakeConnection(FakeConnection &&) = delete;
    FakeConnection &operator=(const FakeConnection &) = delete;
    FakeConnection &operator=(FakeConnection &&) = delete;

    FakeConnection(const std::string &connectionString) { aliveCount++; };
    ~FakeConnection() { aliveCount--; }

    void prepare(const std::string& command, const std::string& sql){};
};

TEST(ConnectionPool, InitializePool) 
{
  {
    Database<FakeConnection> dbHandle("", "", "", 4);
    EXPECT_EQ(FakeConnection::aliveCount, 4);
  }
  EXPECT_EQ(FakeConnection::aliveCount, 0);
}

TEST(ConnectionPool, AquireAndRelease)
{
  {
    Database<FakeConnection> db("", "", "", 1);

    {
      ConnectionGuard guard(db);
      EXPECT_EQ(FakeConnection::aliveCount, 1);
    }

    EXPECT_EQ(FakeConnection::aliveCount, 1);
  }

  EXPECT_EQ(FakeConnection::aliveCount, 0);
}

TEST(ConnectionPool, ConnectionGuardRaii)
{
  Database<FakeConnection> db("", "", "", 1);

  {
    ConnectionGuard guard(db);
  }
  {
    ConnectionGuard guard(db);
  }
}


TEST(ConnectionPool, BlocksWhenPoolIsEmpty)
{
  Database<FakeConnection> db("", "", "", 1);
  std::atomic<bool> acquired{false};

  ConnectionGuard guard(db);

  std::thread t([&] {
      ConnectionGuard guard(db);
      acquired = true;
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(60));
  EXPECT_FALSE(acquired);

  guard.~ConnectionGuard();

  t.join();

  EXPECT_TRUE(acquired);
}

TEST(ConnectionPool, PoolLimitsRespected)
{
  Database<FakeConnection> db("", "", "", 3);

  std::atomic<int> current(0);
  std::atomic<int> maxSeen(0);

  auto worker = [&] {
    ConnectionGuard guard(db);

    int now = ++current;
    maxSeen = std::max(maxSeen.load(), now);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    --current;
  };

  {
    std::jthread t1(worker);
    std::jthread t2(worker);
    std::jthread t3(worker);
  }

  EXPECT_EQ(maxSeen, 3);
}
