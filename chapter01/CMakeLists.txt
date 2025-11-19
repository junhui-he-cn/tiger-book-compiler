cmake_minimum_required(VERSION 3.15)

project(tiger_book_chapter01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(${PROJECT_NAME} 
    main.cpp 
    SLP.cpp
    SLPPrinter.cpp
    SLPGraphviz.cpp
)


# Basic warning flags
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()