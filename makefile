.RECIPEPREFIX = +

build:
+ @g++ tests/knn_test.cpp -o bin/knn_test
+ @g++ tests/kmeans_test.cpp -o bin/kmeans_test

test:
+ @./bin/knn_test
+ @./bin/kmeans_test
