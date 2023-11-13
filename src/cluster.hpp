#ifndef CAMEL_CLUSTER_HPP_
#define CAMEL_CLUSTER_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include "point.hpp"

typedef struct Cluster {
    int id = -1;
    Point centroid = {};
    std::vector<Point> members;
public:
    void print() const;
    void randomize_centroid(const size_t dim);
    void recompute_centroid();    
} Cluster; 

void Cluster::print() const {
    std::cout << "{" << std::endl << "\tId: " << this->id << "," << std::endl;
    if (this->members.size() == 0) {
        std::cout << "}";
        return;
    }
    std::cout << "\tCentroid: "; this->centroid.print();
    std::cout << ", " << std::endl;
    std::cout << "\tMembers:" << std::endl << "\t{" << std::endl;
    for (size_t i = 0; i < 5 && i < this->members.size(); ++i) {
        std::cout << "\t\t";
        this->members[i].print();
        if (members.size() > 5 or i < this->members.size()-1) 
            std::cout << ",";
        std::cout << std::endl;
    }
    if (this->members.size() > 5) std::cout << "\t\t..." << std::endl;
    std::cout << "\t}" << std::endl << "}";
}

void Cluster::randomize_centroid(const size_t dim) {
    this->centroid = random_point(dim);
}

void Cluster::recompute_centroid() {
    if (this->members.size() == 0) {
        this->centroid = {};
        return;
    }
    assert_equal_n(this->members);
    std::vector<float> centroid_coords(this->members[0].n, 0);
    for (Point point: this->members) {
        for (size_t i = 0; i < point.n; ++i) 
            centroid_coords[i] += point.coords[i];
    }
    for (size_t i = 0; i < this->members[0].n; ++i) 
        centroid_coords[i] /= this->members.size();
    this->centroid = {.n = this->members[0].n, .coords = centroid_coords};
}

#endif // CAMEL_CLUSTER_HPP_
