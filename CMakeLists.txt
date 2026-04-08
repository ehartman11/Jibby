cmake_minimum_required(VERSION 3.16)

project(Jibby VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(jibby
    src/json.cpp
    src/json_exception.cpp
    src/json_io.cpp
    src/json_iterator.cpp
    src/json_parser.cpp
    src/json_serializer.cpp
    src/json_tokenizer.cpp
)

target_include_directories(jibby
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(MSVC)
    target_compile_options(jibby PRIVATE /W4)
else()
    target_compile_options(jibby PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(jibby_tests
    tests/test_main.cpp
)

target_link_libraries(jibby_tests
    PRIVATE
        jibby
)

enable_testing()

add_test(
    NAME jibby_tests
    COMMAND jibby_tests
)

set_tests_properties(jibby_tests PROPERTIES
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
