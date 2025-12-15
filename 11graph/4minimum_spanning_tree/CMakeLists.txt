add_executable(1Prim 1Prim.cpp)
add_executable(2Kruskal 2Kruskal.cpp)

target_link_libraries(1Prim PRIVATE GraphBase)
target_link_libraries(2Kruskal PRIVATE GraphBase)