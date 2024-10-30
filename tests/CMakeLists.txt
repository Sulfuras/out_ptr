#find_package(GTest CONFIG REQUIRED)

include(CTest)
include(GoogleTest)

add_executable(test_out_ptr "main.cpp" "include/gtest.h" "include/c_api.h" "test_out_ptr.cpp")

target_include_directories(test_out_ptr PUBLIC include)

target_link_libraries(test_out_ptr PRIVATE gtest_main)
target_link_libraries(test_out_ptr PRIVATE GTest::gmock_main)
target_link_libraries(test_out_ptr PRIVATE GTest::gmock)

target_link_libraries(test_out_ptr PRIVATE out_ptr)

gtest_discover_tests(test_out_ptr
    DISCOVERY_MODE PRE_TEST
    WORKING_DIRECTORY $<TARGET_FILE_DIR:test_out_ptr>)

#project(test_out_ptr LANGUAGES CXX)
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)