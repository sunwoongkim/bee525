cmake_minimum_required(VERSION 3.12)
project(client_dec)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")
add_executable(client_dec ${SOURCES})

find_package(SEAL)
target_link_libraries(client_dec SEAL::seal)
