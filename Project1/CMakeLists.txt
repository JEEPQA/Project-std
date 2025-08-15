cmake_minimum_required(VERSION 3.10)
project(Project1)
file(GLOB SOURCES "*.cpp")
add_executable(Project1 ${SOURCES})
