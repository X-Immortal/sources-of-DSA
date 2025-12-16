//
// Created by xyx on 2025/12/16.
//

#include "Graph.h"
#include <iostream>
#include <vector>

using std::cout;
using std::endl;



/*   ----(4)---> v1 ---(-2)---
 *   |                       |
 *   |                       v
 *  v2 ---------(3)--------> v3
 *   ^                       |
 *   |                       |
 *   ---(-1)--- v4 <---(2)----
 */
int main() {
    Vertex v1("v1");
    Vertex v2("v2");
    Vertex v3("v3");
    Vertex v4("v4");

    v1.edges.assign({
        Edge(&v3, -2)
    });
    v2.edges.assign({
        Edge(&v1, 4),
        Edge(&v3, 3)
    });
    v3.edges.assign({
        Edge(&v4, 2)
    });
    v4.edges.assign({
        Edge(&v2, -1), // 正常情况
        // Edge(&v2, -6), // 负环
    });

    std::vector graph = {&v1, &v2, &v3, &v4};

    try {
        auto &&arrs = Floyd_Warshall(graph);
        print_path(graph, arrs);
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
