#ifndef CAMEL_SUPERVISED_KNN_HPP_
#define CAMEL_SUPERVISED_KNN_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "../point.hpp"

typedef struct Knn {
    int k;
    std::vector<Point> points;
    std::string dist_metric = "euclidean";
public:
    void compile(
        const size_t k, 
        const std::vector<Point> points, 
        const std::string dist_metric = "euclidean"
    );
    Label predict(const Point point) const;
    std::vector<Label> evaluate(const std::vector<Point> points) const;
} Knn;

void Knn::compile(
    const size_t k, 
    const std::vector<Point> points, 
    const std::string dist_metric
) {
    assert_equal_n(points);
    if (dist_metric != "euclidean" && dist_metric != "manhattan") {
        std::cerr << "CAMEL ERROR: Invalid distance metric [";
        std::cerr << dist_metric << "]" << std::endl;
        exit(1);
    }
    this->dist_metric = dist_metric;
    this->k = k;
    if (points.size() == 0) return;
    this->points = points;
    if (k > points.size()) {
        std::cerr << "CAMEL ERROR: k(k nearest neighbors) > n(number of points) [";
        std::cerr << k << " > " << points.size() << "]" << std::endl;
        exit(1);
    }
}

Label Knn::predict(const Point point) const {
    if (this->points.size() == 0) return {};
    std::vector<Point> tmp = copy(this->points);
    sort(tmp.begin(), tmp.end(), [point, this](const Point& p1, const Point& p2) {
        return point.distance(p1, dist_metric) < point.distance(p2, dist_metric);
    });
    std::unordered_map<int, int> int_map;
    std::unordered_map<std::string, int> str_map;
    for (size_t i = 0; i < this->k; ++i) { 
        ++int_map[tmp[i].label.num];
        ++str_map[tmp[i].label.str];
    }
    int int_label = -1, max_freq = 0; 
    for (auto x: int_map) if (x.second > max_freq) int_label = x.first;
    std::string str_label = ""; 
    max_freq = 0;
    for (auto x: str_map) if (x.second > max_freq) str_label = x.first;
    return (Label){.str = str_label, .num = int_label};
}

std::vector<Label> Knn::evaluate(const std::vector<Point> points) const {
    if (points.size() == 0) return {};
    int correct_preds = 0;
    std::vector<Label> labels;
    for (Point point: points) {
        Label label = this->predict(point);
        correct_preds += label.num == point.label.num;
        labels.push_back(label);
    }
    std::cout << "CAMEL OBSERVATIONS: Correct predictions = ";
    std::cout << correct_preds << "/" << points.size() << std::endl;
    std::cout << "CAMEL OBSERVATIONS: Accuracy = ";
    std::cout << correct_preds / points.size() << std::endl;
    return labels;
};

#endif // CAMEL_SUPERVISED_KNN_HPP_
