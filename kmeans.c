/*



ASSUMPTIONS:

- data points are 2D
- data points are positive integers
- data points are sorted


*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define LINE_SIZE  80
#define BIG_NUM    999999999
#define FALSE      0
#define TRUE       1

#define uint            unsigned int
#define datax(i)        data_points[i * 2]
#define datay(i)        data_points[i * 2 + 1]
#define distance(i, j)  (datax(j) - datax(i)) * (datax(j) - datax(i)) + (datay(j) - datay(i)) * (datay(j) - datay(i))
#define is_assigned(i)  point_assignments[i].assigned
#define set_assigned(i) point_assignments[i].assigned = TRUE
#define assign(i, c)    point_assignments[i].centroid_id = c


typedef struct {
  uint centroid_id;
  uint distance;
  char assigned;
} assignment;

char *input_file_name;
uint num_points;
uint num_clusters;
uint *centroids;
uint *data_points;
assignment *point_assignments;


void setup_data_points();
void setup_centroids();
void setup_assignments();

void update_assignments();


int main(int argc, char **argv)
{
  // usage: ./kmeans /path/to/file num_points num_clusters
  input_file_name = argv[1];
  num_points = atoi(argv[2]);
  num_clusters = atoi(argv[3]);
//  printf("file: %s, num_points: %d, num_clusters: %d\n", argv[1], num_points, num_clusters);

  setup_data_points();
  setup_assignments();
  setup_centroids();


  update_assignments();

  return 1;
}


void update_assignments()
{
  uint min_dist;
  uint min_centroid;
  for (uint i = 0; i < num_points; i++) {
    for (uint j = 0, d; j < num_clusters; j++) {
      d = distance(i, centroids[j]);
      if (j == 0 || min_dist > d) {
        min_dist = d;
        min_centroid = j;
      }
//      printf("point %d (%d, %d) to centroid %d (%d, %d), min_dist: %d, min_centroid: %d (%d, %d)\n", i, datax(i), datay(i), j, datax(centroids[j]), datay(centroids[j]), min_dist, min_centroid, datax(centroids[min_centroid]), datay(centroids[min_centroid]));
    }
    point_assignments[i].centroid_id = min_centroid;
    point_assignments[i].distance = min_dist;
//    printf("point %d (%d, %d) => centroid %d (%d, %d), distance: %d\n", i, datax(i), datay(i), min_centroid, datax(centroids[min_centroid]), datay(centroids[min_centroid]), min_dist);
  }
}


void setup_data_points()
{
  FILE *file;
  char line[LINE_SIZE];

  // allocate data array
  data_points = (uint *)calloc(num_points * 2, sizeof(uint));

  // read data from file into array
  file = fopen(input_file_name, "rt");
  for (uint i = 0; i < num_points; i++) {
    fgets(line, LINE_SIZE, file);
    sscanf(line, "%d,%d", &datax(i), &datay(i));
  }
  fclose(file);

//  // check values
//  for (uint i = 0; i < num_points; i++) {
//    printf("data[%d] = %d, %d\n", i, datax(i), datay(i));
//  }
}

void setup_assignments()
{
  point_assignments = (assignment *)calloc(num_points, sizeof(assignment));

//  // check values
//  for (uint i = 0; i < num_points; i++) {
//    printf("assignments[%d]: centroid_id = %d, distance = %d, assigned = %d\n", i, point_assignments[i].centroid_id, point_assignments[i].distance, point_assignments[i].assigned);
//  }
}

void setup_centroids()
{
  uint i, j, k, x, y;

  // allocate centroids array
  centroids = (uint *)calloc(num_clusters, sizeof(uint));

  // pick starting centroids, ensuring uniqueness
  srand((unsigned)time(NULL));
  for (i = 0; i < num_clusters; ) {
    // grab a random data point
    j = random() % num_points;

    // take if first one or unassigned
    if (i == 0 || !is_assigned(j)) {
      centroids[i] = j;

      // mark this point (and all points like it) as assigned;
      // this traversal works because we know the data set is sorted
      x = datax(j);
      y = datay(j);
      for (k = j; k < num_points && x == datax(k) && y == datay(k); k++)
        set_assigned(k);
      for (k = j; k-- > 0 && x == datax(k) && y == datay(k); )
        set_assigned(k);

      // on to next centroid
      i++;
    }
  }

//  // check values
//  for (uint i = 0; i < num_points; i++) {
//    printf("assignments[%d]: centroid_id = %d, distance = %d, assigned = %d\n", i, point_assignments[i].centroid_id, point_assignments[i].distance, point_assignments[i].assigned);
//  }

  // it's possible that the actual number of clusters is less than asked for
  if (i < num_clusters)
    num_clusters = i;

//  // check values
//  for (uint i = 0; i < num_clusters; i++) {
//    printf("centroids[%d] = %d, %d\n", i, datax(centroids[i]), datay(centroids[i]));
//  }
}















