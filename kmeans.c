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

//#define DEBUG
#define LINE_SIZE  80
#define FALSE      0
#define TRUE       1

#define uint             unsigned int
#define uint64           unsigned long
#define datax(i)         data_points[i * 2]
#define datay(i)         data_points[i * 2 + 1]
#define distance(i, j)   (datax(j) - datax(i)) * (datax(j) - datax(i)) + (datay(j) - datay(i)) * (datay(j) - datay(i))
#define is_assigned(i)   point_assignments[i].assigned
#define set_assigned(i)  point_assignments[i].assigned = TRUE
#define assign(i, c)     point_assignments[i].centroid_id = c
#define assigned_to(i)   point_assignments[i].centroid_id


typedef struct {
  uint    point_id;
  uint    num_points;
  uint64  sum_x;
  uint64  sum_y;
} centroid;

typedef struct {
  uint  centroid_id;
  uint  distance;
  char  assigned;
} assignment;

char *input_file_name;
uint num_points;
uint num_clusters;
centroid *centroids;
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
  printf("file: %s, num_points: %d, num_clusters: %d\n", argv[1], num_points, num_clusters);

  setup_data_points();
  setup_assignments();
  setup_centroids();


  update_assignments();

  return 0;
}


void update_assignments()
{
  uint      i, j, d;
  uint      nearest_centroid;
  uint64    min_dist;
  centroid  *cp, *cplast;

  // clear out centroids
  for (cp = centroids, cplast = centroids + num_clusters; cp < cplast; cp++) {
    cp->num_points = 0;
    cp->sum_x = 0;
    cp->sum_y = 0;
  }

  // for each data point...
  for (i = 0; i < num_points; i++) {
    // find nearest centroid
    for (j = 0; j < num_clusters; j++) {
      min_dist = HUGE_VAL;
      d = distance(i, centroids[j].point_id);
      if (min_dist > d) {
        min_dist = d;
        nearest_centroid = j;
      }

      #ifdef DEBUG
      printf("point %d (%d, %d) to centroid %d (%d, %d), min_dist: %ld, nearest_centroid: %d (%d, %d)\n", i, datax(i), datay(i), j, datax(centroids[j].point_id), datay(centroids[j].point_id), min_dist, nearest_centroid, datax(centroids[nearest_centroid].point_id), datay(centroids[nearest_centroid].point_id));
      #endif
    }

    // assign point to centroid
    point_assignments[i].centroid_id = nearest_centroid;
    point_assignments[i].distance = min_dist;

    // update centroid stats
    centroids[nearest_centroid].sum_x += datax(i);
    centroids[nearest_centroid].sum_y += datay(i);
    centroids[nearest_centroid].num_points++;

    #ifdef DEBUG
    printf("point %d (%d, %d) => centroid %d (%d, %d), distance: %ld\n", i, datax(i), datay(i), nearest_centroid, datax(centroids[nearest_centroid].point_id), datay(centroids[nearest_centroid].point_id), min_dist);
    #endif
  }

  #ifdef DEBUG
  for (i = 0, cp = centroids, cplast = centroids + num_clusters; cp < cplast; i++, cp++)
    printf("centroid %d (%d, %d) => %d points, mean = (%.1lf, %.1lf)\n", i, datax(cp->point_id), datay(cp->point_id), cp->num_points, (double)cp->sum_x / cp->num_points, (double)cp->sum_y / cp->num_points);
  #endif
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

  #ifdef DEBUG
  for (uint i = 0; i < num_points; i++)
    printf("data[%d] = %d, %d\n", i, datax(i), datay(i));
  #endif
}

void setup_assignments()
{
  point_assignments = (assignment *)calloc(num_points, sizeof(assignment));

  #ifdef DEBUG
  for (uint i = 0; i < num_points; i++)
    printf("assignments[%d]: centroid_id = %d, distance = %d, assigned = %d\n", i, point_assignments[i].centroid_id, point_assignments[i].distance, point_assignments[i].assigned);
  #endif
}

void setup_centroids()
{
  uint i, j, k, x, y;

  // allocate centroids array
  centroids = (centroid *)calloc(num_clusters, sizeof(centroid));

  // pick starting centroids, ensuring uniqueness
  srand((unsigned)time(NULL));
  for (i = 0; i < num_clusters; ) {
    // grab a random data point
    j = random() % num_points;

    // take if first one or unassigned
    if (i == 0 || !is_assigned(j)) {
      centroids[i].point_id = j;

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

  #ifdef DEBUG
  for (uint i = 0; i < num_points; i++)
    printf("assignments[%d]: centroid_id = %d, distance = %d, assigned = %d\n", i, point_assignments[i].centroid_id, point_assignments[i].distance, point_assignments[i].assigned);
  #endif

  // it's possible that the actual number of clusters is less than asked for
  if (i < num_clusters)
    num_clusters = i;

  #ifdef DEBUG
  for (uint i = 0; i < num_clusters; i++)
    printf("centroids[%d] = %d, %d\n", i, datax(centroids[i].point_id), datay(centroids[i].point_id));
  #endif
}















