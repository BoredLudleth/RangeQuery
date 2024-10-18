find_package(GTest REQUIRED)

include_directories(${INCLUDE_DIR})

add_executable(tree_test tree_test.cpp)

target_link_libraries(tree_test GTest::gtest_main)

add_test(tree_gtest tree_test)

add_custom_command(TARGET tree_test
                   COMMENT "Run tests"
                   POST_BUILD 
                   WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                   COMMAND ${CMAKE_CTEST_COMMAND})