cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (WIN32)
    add_compile_definitions(NOMINMAX _USE_MATH_DEFINES)
endif()
if (MSVC)
    add_compile_options(-Zc:preprocessor -utf-8 -EHsc -bigobj)
else()
    if (WIN32)
        add_compile_options(-finput-charset=utf-8 -fexec-charset=utf-8)
    endif()
    add_compile_options(-Wall -Wextra -Werror=return-type)
endif()

project(04_mvc_role)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets REQUIRED)

file(GLOB sources *.cpp)
add_executable(04_mvc_role ${sources})
target_link_libraries(04_mvc_role PRIVATE Qt5::Core Qt5::Widgets)
target_include_directories(04_mvc_role PRIVATE .)
