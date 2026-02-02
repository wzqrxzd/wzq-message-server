cmake_minimum_required(VERSION 3.14)
project(wzq-message-server)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POLICY_VERSION 3.5)

set(LIBS ${CMAKE_SOURCE_DIR}/libs)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

find_package(spdlog REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_library(ARGON2_LIB argon2 REQUIRED)
find_path(LIBSODIUM_INCLUDE_DIR sodium.h REQUIRED)
find_library(LIBSODIUM_LIBRARY sodium REQUIRED)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main
)

set (gtest_force_shared_crt ON CACHE BOOl "" FORCE)

FetchContent_MakeAvailable(googletest)


file(GLOB CORE_SRC "${CMAKE_SOURCE_DIR}/core/*.cxx")
add_library(wzq_core STATIC ${CORE_SRC})
target_include_directories(wzq_core PUBLIC ${CMAKE_SOURCE_DIR}/core ${LIBS}/jwt-cpp/include/ ${PostgreSQL_INCLUDE_DIRS} ${LIBSODIUM_INCLUDE_DIR})
target_link_libraries(wzq_core PUBLIC spdlog fmt pqxx ${ARGON2_LIB} nlohmann_json OpenSSL::SSL OpenSSL::Crypto ${PostgreSQL_LIBRARIES} ${LIBSODIUM_LIBRARY})

enable_testing()

file(GLOB TEST_SRC "${CMAKE_SOURCE_DIR}/tests/core/*.cxx")

add_executable(wzq_core_tests ${TEST_SRC})

target_link_libraries(wzq_core_tests
  PRIVATE
  wzq_core
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(wzq_core_tests)

file(GLOB CROW_ADAPTERS_SRC "${CMAKE_SOURCE_DIR}/adapters/crow/*.cxx")
# file(GLOB ROUTES_SRC "${CMAKE_SOURCE_DIR}/routes/*.cxx")

add_executable(wzq-message-server
  server.cxx
  env_utils.cxx
  ${CROW_ADAPTERS_SRC}
  # ${ROUTES_SRC}
)

target_compile_options(wzq-message-server PRIVATE -Wno-deprecated-declarations)

target_include_directories(wzq-message-server PRIVATE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/core ${LIBS}/crow/include/ ${LIBS}/jwt-cpp/include/ ${PostgreSQL_INCLUDE_DIRS} ${LIBSODIUM_INCLUDE_DIR})

target_link_libraries(wzq-message-server PRIVATE wzq_core spdlog fmt pqxx ${ARGON2_LIB} nlohmann_json OpenSSL::SSL OpenSSL::Crypto ${PostgreSQL_LIBRARIES} ${LIBSODIUM_LIBRARY})
