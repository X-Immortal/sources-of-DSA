add_executable(1dfs_test 1dfs_test.cpp)
add_executable(2bfs_test 2bfs_test.cpp)

target_link_libraries(1dfs_test PRIVATE Graph)
target_link_libraries(2bfs_test PRIVATE Graph)
