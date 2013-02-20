kmeans
======

K-means in 2D, a C implementation.

Run a test on 100 random points:

```
% make test100
gcc -Wall -std=gnu9x -g -o kmeans kmeans.c -lm
./generate_test_data.sh 100 10000 | sort -t, -k 1,1n -k 2,2n > test100.txt
./kmeans ./test100.txt 100 2 100
5543	2195	49	1361.9
4880	6481	51	1366.9
```
