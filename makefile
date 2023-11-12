.RECIPEPREFIX = +

build:
+ @g++ tests/knn_test.cpp -o bin/knn_test

test:
+ @./bin/knn_test