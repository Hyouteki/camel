#ifndef CAMEL_UNSUPERVISED_KMEANS_HPP_
#define CAMEL_UNSUPERVISED_KMEANS_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "../cluster.hpp"

typedef struct Kmeans {
    size_t k;
    std::vector<Point> points;
    std::vector<Cluster> clusters;
    std::string dist_metric = "euclidean";
    int max_iters = 200;
    float eps = DEFAULT_EPS;
public:
    void compile(
        const size_t k, 
        const std::vector<Point> points, 
        const std::string dist_metric = "euclidean",
        const int max_iters = 200,
        const float eps = DEFAULT_EPS
    );
    Label predict(const Point point) const;
    std::vector<Label> evaluate(const std::vector<Point> points) const;
private:
    void recompute_centroids();
    void reassign_memberships();
} Kmeans;

void Kmeans::recompute_centroids() {
    for (size_t i = 0; i < k; ++i) this->clusters[i].recompute_centroid();
}

void Kmeans::reassign_memberships() {
    if (k == 0) return;
    for (size_t i = 0; i < k; ++i) this->clusters[i].members.clear();
    for (Point point: points) {
        sort(this->clusters.begin(), this->clusters.end(), [point, this](const Cluster& c1, const Cluster& c2) {
           return c1.centroid.distance(point, this->dist_metric) < c2.centroid.distance(point, this->dist_metric);
        });
        this->clusters[0].members.push_back(point);
    } 
}

void Kmeans::compile(
    const size_t k, 
    const std::vector<Point> points, 
    const std::string dist_metric,
    const int max_iters,
    const float eps
) {
    assert_equal_n(points);
    this->k = k;
    if (dist_metric != "euclidean" && dist_metric != "manhattan") {
        std::cerr << "CAMEL ERROR: Invalid distance metric [";
        std::cerr << dist_metric << "]" << std::endl;
        exit(1);
    }
    this->dist_metric = dist_metric;
    this->max_iters = max_iters;
    this->points = points;
    if (points.size() == 0) return;
    for (size_t i = 0; i < k; ++i) {
        Cluster cluster;
        cluster.randomize_centroid(points[0].n); 
        this->clusters.push_back(cluster);
    }
    this->reassign_memberships();
    for (int i = 0; i < this->max_iters; ++i) {
        std::vector<Point> centroids_old;
        for (Cluster cluster: this->clusters)
            centroids_old.push_back(cluster.centroid.copy());
        this->recompute_centroids();
        this->reassign_memberships();
        std::vector<Point> centroids_new;
        for (Cluster cluster: this->clusters)
            centroids_new.push_back(cluster.centroid.copy());
        if (allclose(centroids_old, centroids_new)) break;
    } 
}

Label Kmeans::predict(const Point point) const {
    std::vector<Cluster> tmp(this->clusters);
    sort(tmp.begin(), tmp.end(), [point, this](const Cluster& c1, const Cluster& c2){
       return c1.centroid.distance(point, this->dist_metric) < c2.centroid.distance(point, this->dist_metric);
    });
    std::unordered_map<int, int> int_map;
    std::unordered_map<std::string, int> str_map;
    for (Point p: tmp[0].members) { 
        ++int_map[p.label.num];
        ++str_map[p.label.str];
    }
    int int_label = -1, max_freq = 0; 
    for (auto x: int_map) if (x.second > max_freq) int_label = x.first;
    std::string str_label = ""; 
    max_freq = 0;
    for (auto x: str_map) if (x.second > max_freq) str_label = x.first;
    return (Label){.str = str_label, .num = int_label};
}

std::vector<Label> Kmeans::evaluate(const std::vector<Point> points) const {
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

#endif // CAMEL_UNSUPERVISED_KMEANS_HPP_
