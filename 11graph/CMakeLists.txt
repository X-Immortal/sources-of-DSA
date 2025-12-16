set(INCLUDE_PATH "00src/0base")
set(DS_DIR ${PROJECT_SOURCE_DIR}/12disjoint_set)

include_directories(${INCLUDE_PATH})

file(GLOB GRAPH_SRCS "00src/*/*.cpp")
file(GLOB DS_SRCS "${DS_DIR}/*.cpp")

add_library(Graph STATIC ${GRAPH_SRCS} ${DS_SRCS})

target_include_directories(Graph PUBLIC ${INCLUDE_PATH} ${DS_DIR})

add_subdirectory(99test)