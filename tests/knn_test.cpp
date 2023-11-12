#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "../src/supervised/knn.hpp"

int main() {
    Knn knn;
    std::vector<std::vector<float>> vector_points = {
        {14,  81,  15,  41,  87,  30,  95,  92,  37},
        {26,  49,  72,  51,  43,  92,  68,  62,  18},
        {12,  72,  44,  33,  86,  16,  12,  19,  90},
        {53,  81,  18,  38,  47,  81,  55,  66,  48},
        {48,  60,  59,  48,  81,  15,  19,  57,  81},
        {84,  39,  94,  84,  96,  95,  33,  97,  25},
        {60,  81,  67,  95,  75,  76,  13,  54,  14},
        {31,  57,  55,  66,  71,  39,  48,  48,  89},
        {36,  34,  22,  41,  69,  87,  25,  76,  98},
        {60,  84,  58,  29,  81,  27,  33,  27,  28}
    };
    std::vector<Point> points = vectors_to_points(vector_points);
    std::vector<int> int_labels = {1, 1, 2, 3, 4, 1, 2, 2, 4, 3}; 
    for (size_t i = 0; i < 10; ++i) points[i].label.num = int_labels[i];
    knn.compile(5, points, "euclidean");
    Point point = (Point){.n = 9, .coords = {14,  81,  15,  41,  87,  30,  95,  92,  37}};
    std::cout << "CAMEL TEST PREDICTION: " << knn.predict(point).num << std::endl;
    return 0;
}