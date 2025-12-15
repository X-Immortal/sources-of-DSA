include_directories(0base)

file(GLOB BASE_SRCS "0base/*.cpp")

add_library(GraphBase STATIC ${BASE_SRCS})

target_include_directories(GraphBase PUBLIC 0base)

add_subdirectory(1search)
add_subdirectory(2topological_sort)
add_subdirectory(3shortest_path)
