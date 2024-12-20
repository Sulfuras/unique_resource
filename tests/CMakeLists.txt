#find_package(GTest CONFIG REQUIRED)

include(CTest)
include(GoogleTest)

add_executable(test_unique_resource "main.cpp" "include/gtest.h" "test_unique_resource.cpp" "test_test.cpp")

target_include_directories(test_unique_resource PUBLIC include)

target_link_libraries(test_unique_resource PRIVATE gtest_main)
target_link_libraries(test_unique_resource PRIVATE GTest::gmock_main)
target_link_libraries(test_unique_resource PRIVATE GTest::gmock)

target_link_libraries(test_unique_resource PRIVATE unique_resource)

gtest_discover_tests(test_unique_resource
    DISCOVERY_MODE PRE_TEST
    WORKING_DIRECTORY $<TARGET_FILE_DIR:test_unique_resource>)

#project(test_unique_resource LANGUAGES CXX)
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)