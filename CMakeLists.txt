cmake_minimum_required(VERSION 3.16)
project(SimpleShuffleCipher C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_library(cipher_lib
    src/cipher.c
)

target_include_directories(cipher_lib PUBLIC include)

add_executable(cipher_app src/main.c)
target_link_libraries(cipher_app cipher_lib)

add_executable(cipher_bench benchmarks/bench_cipher.c)
target_link_libraries(cipher_bench cipher_lib)

enable_testing()
find_package(GTest REQUIRED)

add_executable(cipher_tests
    tests/test_avalanche.cpp
    tests/test_rfc_7539.cpp
    tests/test_side_channels.cpp
)

target_include_directories(cipher_tests PRIVATE include)
target_link_libraries(cipher_tests
    cipher_lib
    GTest::gtest
    GTest::gtest_main
)

add_test(NAME CipherTests COMMAND cipher_tests)
