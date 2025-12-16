add_executable(1Prim_test 1Prim_test.cpp)
add_executable(2Kruskal_test 2Kruskal_test.cpp)

target_link_libraries(1Prim_test PRIVATE Graph)
target_link_libraries(2Kruskal_test PRIVATE Graph)