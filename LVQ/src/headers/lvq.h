#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*			  DEFINES			  */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define CLUSTERS 5
#define SIMULATIONS 5

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*			  STRUCTS			  */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

typedef struct Point
{
	/* Coordinates (x,y) of a 2D point	*/
	/* and the cluster it belongs to	*/
	float x;
	float y;
	int cluster;
}point_t;

typedef struct Cluster
{
	/* Coordinates (x,y) of the centroid of cluster */
	float x;
	float y;
	struct Cluster *next;
}cluster_t;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*			  GLOBALS			  */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

extern point_t dataset[];
extern cluster_t ***centroid_heads;
extern cluster_t ***centroid_tails;
extern int num_of_sim;
extern float min_dispersion;
extern int min_simulation;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*			 FUNCTIONS			  */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void lvq_main();
