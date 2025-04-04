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

project(06_opengl_brdf)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets OpenGL REQUIRED)

file(GLOB sources CONFIGURE_DEPENDS *.cpp)
add_executable(06_opengl_brdf ${sources})
target_link_libraries(06_opengl_brdf PRIVATE Qt5::Core Qt5::Widgets Qt5::OpenGL)
target_include_directories(06_opengl_brdf PRIVATE .)
