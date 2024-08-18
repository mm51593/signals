cmake_minimum_required(VERSION 3.20)
project(Signals)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(.)
add_executable(signals "src/main.c" "src/handlers/ideal_handler.c")
