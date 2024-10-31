cmake_minimum_required(VERSION 3.0)

project(rmscrypto_sdk)

set(RMSCRYPTO_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(ROOT_DIR ${RMSCRYPTO_DIR}/../..)
set(ROOT_BIN_DIR ${ROOT_DIR}/bin)
set(ROOT_BIN_CRYPTO_DIR ${ROOT_BIN_DIR}/crypto)
set(ROOT_BIN_CRYPTO_PLATFORM_DIR ${ROOT_BIN_CRYPTO_DIR}/platform)

# add_compile_definitions(-Wall -Wextra -Werror)
add_compile_options(-fPIC)

add_definitions(-DQTFRAMEWORK)

add_subdirectory(Platform)
add_subdirectory(Crypto)
add_subdirectory(CryptoAPI)

# add_subdirectory(UnitTests)