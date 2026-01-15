cmake_minimum_required(VERSION 3.14)
project(wzq-message-server)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POLICY_VERSION 3.5)

set(LIBS ${CMAKE_SOURCE_DIR}/libs)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

find_package(spdlog REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_library(ARGON2_LIB argon2)
find_path(LIBSODIUM_INCLUDE_DIR sodium.h)
find_library(LIBSODIUM_LIBRARY sodium)


file(GLOB CORE_SRC "${CMAKE_SOURCE_DIR}/core/*.cxx")
add_library(wzq_core STATIC ${CORE_SRC})
target_include_directories(wzq_core PRIVATE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/core ${LIBS}/crow/include/ ${LIBS}/jwt-cpp/include/ ${PostgreSQL_INCLUDE_DIRS} ${LIBSODIUM_INCLUDE_DIR})
target_link_libraries(wzq_core PRIVATE spdlog fmt pqxx ${ARGON2_LIB} nlohmann_json OpenSSL::SSL OpenSSL::Crypto ${PostgreSQL_LIBRARIES} ${LIBSODIUM_LIBRARY})


file(GLOB ROUTES_SRC "${CMAKE_SOURCE_DIR}/routes/*.cxx")

add_executable(wzq-message-server
  server.cxx
  env_utils.cxx
  crow_core_adapter.cxx
  ${ROUTES_SRC}
)

target_compile_options(wzq-message-server PRIVATE -Wno-deprecated-declarations)

target_include_directories(wzq-message-server PRIVATE ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/core ${LIBS}/crow/include/ ${LIBS}/jwt-cpp/include/ ${PostgreSQL_INCLUDE_DIRS} ${LIBSODIUM_INCLUDE_DIR})

target_link_libraries(wzq-message-server PRIVATE wzq_core spdlog fmt pqxx ${ARGON2_LIB} nlohmann_json OpenSSL::SSL OpenSSL::Crypto ${PostgreSQL_LIBRARIES} ${LIBSODIUM_LIBRARY})
