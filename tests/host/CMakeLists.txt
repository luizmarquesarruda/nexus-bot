cmake_minimum_required(VERSION 3.16)
project(nexus_bot_host_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(nexus_safety_tests
  safety_controller_test.cpp
  ../../firmware/esp32/src/safety/SafetyController.cpp
)
target_include_directories(nexus_safety_tests PRIVATE
  ../../firmware/esp32/include
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(nexus_safety_tests PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

enable_testing()
add_test(NAME nexus_safety_tests COMMAND nexus_safety_tests)
