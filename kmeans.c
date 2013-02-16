#include <stdlib.h>
#include <stdio.h>
//#include <assert.h>
//#include <float.h>
#include <math.h>
#include <time.h>

#define LINE_SIZE 80
#define BIG_NUMBER 999999999
#define datax(i) data_points[i * 2]
#define datay(i) data_points[i * 2 + 1]

typedef struct {
  int    centroid_id;
  double distance;
} assignment;

char *input_file_name;
int num_points;
int num_clusters;
int *centroids;
double *data_points;
assignment *point_assignments;


void setup_data_points();
void setup_centroids();
void setup_assignments();

double distance(int from_point_id, int to_point_id);
void update_assignments();


int main(int argc, char **argv)
{
  // usage: ./kmeans /path/to/file num_points num_clusters
  input_file_name = argv[1];
  num_points = atoi(argv[2]);
  num_clusters = atoi(argv[3]);
//  printf("file: %s, num_points: %d, num_clusters: %d\n", argv[1], num_points, num_clusters);

  setup_data_points();
  setup_centroids();
  setup_assignments();


  update_assignments();

  return 1;
}


void update_assignments()
{
  double min_dist;
  int min_centroid;
  for (int i = 0; i < num_points; i++) {
    for (int j = 0, d; j < num_clusters; j++) {
      d = distance(i, centroids[j]);
      if (j == 0 || min_dist > d) {
        min_dist = d;
        min_centroid = j;
      }
//      printf("point %d (%.1lf, %.1lf) to centroid %d (%.1lf, %.1lf), min_dist: %.1lf, min_centroid: %d (%.1lf, %.1lf)\n", i, datax(i), datay(i), j, datax(centroids[j]), datay(centroids[j]), min_dist, min_centroid, datax(centroids[min_centroid]), datay(centroids[min_centroid]));
    }
  }
}

double distance(int from_point_id, int to_point_id)
{
  return (datax(to_point_id) - datax(from_point_id)) * (datax(to_point_id) - datax(from_point_id))
    + (datay(to_point_id) - datay(from_point_id)) * (datay(to_point_id) - datay(from_point_id));
}

void setup_data_points()
{
  FILE *file;
  char line[LINE_SIZE];

  // allocate data array
  data_points = (double *)calloc(num_points * 2, sizeof(double));

  // read data from file into array
  file = fopen(input_file_name, "rt");
  for (int i = 0; i < num_points; i++) {
    fgets(line, LINE_SIZE, file);
    sscanf(line, "%lf,%lf", &datax(i), &datay(i));
  }
  fclose(file);

//  // check values
//  for (int i = 0; i < num_points; i++) {
//    printf("data[%d] = %lf, %lf\n", i / 2, datax(i), datay(i));
//  }
}

void setup_assignments()
{
  point_assignments = (assignment *)calloc(num_points, sizeof(assignment));
}

void setup_centroids()
{
  // allocate centroids array
  centroids = (int *)calloc(num_clusters, sizeof(int));

  // pick starting centroids
  srand((unsigned)time(NULL));
  for (int i = 0, d = num_points / num_clusters, j = d / 2; i < num_clusters; i++, j += d) {
    centroids[i] = j;
  }

//  // check values
//  for (int i = 0; i < num_clusters; i++) {
//    printf("centroids[%d] = %lf, %lf\n", i, datax(centroids[i]), datay(centroids[i]));
//  }
}

















//
//
///*
//
//int** data       array of data points
//int      n          number of data points
//int      m          DIMENSIONS
//int      k          desired number of clusters
//int   t          error tolerance (suggested: 0.0001)
//int** centroids  array of centroids
//
//*/
//
//int *k_means(int **data, int n, int m, int k, int t, int **centroids)
//{
//  /* output cluster label for each data point */
//  int *labels = (int*)calloc(n, sizeof(int));
//
//  int h, i, j; /* loop counters, of course :) */
//  int *counts = (int*)calloc(k, sizeof(int)); /* size of each cluster */
//  int old_error, error = DBL_MAX; /* sum of squared euclidean distance */
//  int **c = centroids ? centroids : (int**)calloc(k, sizeof(int*));
//  int **c1 = (int**)calloc(k, sizeof(int*)); /* temp centroids */
//
//  assert(data && k > 0 && k <= n && m > 0 && t >= 0); /* for debugging */
//
//  /****
//  ** initialization */
//
//  for (h = i = 0; i < k; h += n / k, i++) {
//    c1[i] = (int*)calloc(m, sizeof(int));
//    if (!centroids) {
//       c[i] = (int*)calloc(m, sizeof(int));
//    }
//    /* pick k points as initial centroids */
//    for (j = m; j-- > 0; c[i][j] = data[h][j]);
//  }
//
////  return labels;
//
//  /****
//  ** main loop */
//
//  do {
//    /* save error from last step */
//    old_error = error, error = 0;
//
//    /* clear old counts and temp centroids */
//    for (i = 0; i < k; counts[i++] = 0) {
//       for (j = 0; j < m; c1[i][j++] = 0);
//    }
//
//    for (h = 0; h < n; h++) {
//      /* identify the closest cluster */
//      int min_distance = DBL_MAX;
//      for (i = 0; i < k; i++) {
//        int distance = 0;
//        for (j = m; j-- > 0; distance += pow(data[h][j] - c[i][j], 2));
//        if (distance < min_distance) {
//          labels[h] = i;
//          min_distance = distance;
//        }
//      }
//      /* update size and temp centroid of the destination cluster */
//      for (j = m; j-- > 0; c1[labels[h]][j] += data[h][j]);
//      counts[labels[h]]++;
//      /* update standard error */
//      error += min_distance;
//    }
//
//    for (i = 0; i < k; i++) { /* update all centroids */
//       for (j = 0; j < m; j++) {
//          c[i][j] = counts[i] ? c1[i][j] / counts[i] : c1[i][j];
//       }
//    }
//
//  } while (fabs(error - old_error) > t);
//
//  /****
//  ** housekeeping */
//
//  for (i = 0; i < k; i++) {
//    if (!centroids) {
//       free(c[i]);
//    }
//    free(c1[i]);
//  }
//
//  if (!centroids) {
//    free(c);
//  }
//  free(c1);
//
//  free(counts);
//
//  return labels;
//}
//
