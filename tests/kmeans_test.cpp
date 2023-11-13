#include <stdio.h>
#include <stdlib.h>
#include "../src/cluster.hpp"

int main() {
    Cluster c1 = {
        .id = 0, 
        .centroid = random_point(2),
        .members = {
            random_point(2),
            random_point(2),
        }
    };
    c1.print(); std::cout << std::endl;
    c1.recompute_centroid();
    c1.print(); std::cout << std::endl;
    return 0;
}
