cmake_minimum_required(VERSION 3.0)

add_definitions(-DQTFRAMEWORK)

add_subdirectory(Platform)
add_subdirectory(Crypto)
add_subdirectory(CryptoAPI)

# add_subdirectory(UnitTests)

# add_dependencies(CryptoAPI Crypto)
# add_dependencies(Crypto Platform)