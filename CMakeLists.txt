cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

project(qtguide)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets REQUIRED)

file(GLOB sources *.cpp)
add_executable(qtguide ${sources})
target_link_libraries(qtguide PRIVATE Qt5::Core Qt5::Widgets)
