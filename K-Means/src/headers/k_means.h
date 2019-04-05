#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*	       DEFINES		  */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define SIMULATIONS 5
#define CLUSTERS	5

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*	       STRUCTS		  */
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
}cluster_t;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*	       GLOBALS		  */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

extern point_t dataset[];
extern cluster_t **centroid;
extern float min_dispersion;
extern int min_simulation;
extern int min_sim_recalc;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*	      FUNCTIONS		  */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

int kmeans_main();
