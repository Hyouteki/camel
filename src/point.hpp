#ifndef CAMEL_POINT_HPP_
#define CAMEL_POINT_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cmath>
#include <vector>

typedef struct Label {
    std::string str = "";
    int num = -1;
} Label;

typedef struct Point {
    size_t n;
    Label label;
    std::vector<float> coords;
public:
    void print() const;
    float euclidean_distance(const Point point) const;
    float manhattan_distance(const Point point) const;
    float distance(const Point point, const std::string dist_metric) const;
    void assert_equal_n(const Point point) const;
    Point copy() const;
} Point; 

std::vector<std::vector<float>> points_to_matrix(const std::vector<Point> points);
std::vector<Point> vectors_to_points(const std::vector<std::vector<float>> vectors);
void assert_equal_n(const std::vector<Point> points);
std::vector<Point> copy(const std::vector<Point> points);
Point random_point(const size_t n);

void Point::print() const {
    std::cout << this->label.num << ": [";
    for (size_t i = 0; i < this->coords.size(); ++i) {
        std::cout << this->coords[i];
        if (i < this->coords.size()-1) std::cout << ", ";
    }
    std::cout << "]";
}

void Point::assert_equal_n(const Point point) const {
    if (this->n != point.n) {
        std::cerr << "CAMEL ERROR: Dimension of points are not the same [";
        std::cerr << this->n << " != " << point.n << "]" << std::endl;
        exit(1);
    }
}

float Point::euclidean_distance(const Point point) const {
    this->assert_equal_n(point);
    float tmp = 0;
    for (size_t i = 0; i < this->n; ++i) {
        float diff = this->coords[i] - point.coords[i];
        tmp += diff * diff;
    }
    return (float)sqrt(tmp);
}

float Point::manhattan_distance(const Point point) const {
    this->assert_equal_n(point);
    float tmp = 0;
    for (size_t i = 0; i < this->n; ++i)
        tmp += abs(this->coords[i] - point.coords[i]);
    return tmp;
}

float Point::distance(const Point point, const std::string dist_metric) const {
    return (dist_metric == "manhattan")? 
        this->manhattan_distance(point): 
        this->euclidean_distance(point); 
}

Point Point::copy() const {
    Point tmp;
    tmp.n = this->n;
    tmp.label = this->label;
    tmp.coords = this->coords;
    return tmp;
}

std::vector<std::vector<float>> points_to_matrix(const std::vector<Point> points) {
    if (points.size() == 0) return {};
    std::vector<std::vector<float>> tmp;
    Point p0 = points[0];
    for (size_t i = 0; i < points.size(); ++i) {
        p0.assert_equal_n(points[i]);
        tmp.push_back(points[i].coords);
    }
    return tmp;
}

std::vector<Point> vectors_to_points(const std::vector<std::vector<float>> vectors) {
    if (vectors.size() == 0) return {};
    std::vector<Point> tmp;
    Point p0 = (Point){.n = vectors[0].size(), .coords = vectors[0]};
    for (size_t i = 0; i < vectors.size(); ++i) {
        Point pi = (Point){.n = vectors[i].size(), .coords = vectors[i]};
        p0.assert_equal_n(pi);
        tmp.push_back(pi);
    }
    return tmp;
}

void assert_equal_n(const std::vector<Point> points) {
    if (points.size() == 0) return;
    Point p0 = points[0];
    for (Point pi: points) p0.assert_equal_n(pi);
}

std::vector<Point> copy(const std::vector<Point> points) {
    std::vector<Point> tmp;
    for (Point point: points) tmp.push_back(point.copy());
    return tmp;
}

Point random_point(const size_t n) {
    std::vector<float> coords;
    for (size_t i = 0; i < n; ++i) coords.push_back(rand()%100);
    return (Point){.n = n, .coords = coords};
}

#endif // CAMEL_POINT_HPP_
