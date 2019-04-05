#include "headers/k_means.h"
#include <time.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*	       GLOBALS		  */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

point_t dataset[500]; 		// The array that holds the dataset
cluster_t **centroid; 		// The 2D array where all the centroid values are stored
int num_of_recalculations = 0; 	// Number of times the current simulation had to recalculate the centroids
int num_of_sim; 		// The number of the current simulation
float min_dispersion = INFINITY;// The least dispersion found
int min_simulation; 		// The number of the simulation with the least dispersion
int min_sim_recalc; 		// The number of recalculations the <min_simulation> had to perform

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*	       FUNCTIONS	  */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* @seed: Creates a dataset.txt file with 500 two-dimensional points.		*/
/*		  Feel free to comment out the call to this function in 	*/
/*		  kmeans_main to maintain the dataset for more than one run. 	*/
void seed()
{
	FILE *fp;
	int i, rectangle;
	float x, y;

	fp = fopen("dataset.txt", "w+");

	srand((unsigned int)time(NULL));
	for (i = 0; i < 500; i++)
	{
		rectangle = i / 100;
		if (rectangle == 0)
		{ /* Rectangle [-0.3, 0.3]x[-0.3, 0.3] */
			x = ((float)rand() / (float)(RAND_MAX)) * 0.6 - 0.3;
			y = ((float)rand() / (float)(RAND_MAX)) * 0.6 - 0.3;
		}
		else if (rectangle == 1)
		{ /* Rectangle [-1.1, -0.5]x[0.5, 1.1] */
			x = ((float)rand() / (float)(RAND_MAX)) * (-0.6) - 0.5;
			y = ((float)rand() / (float)(RAND_MAX)) * 0.6 + 0.5;
		}
		else if (rectangle == 2)
		{ /* Rectangle [-1.1, -0.5]x[-1.1, -0.5] */
			x = ((float)rand() / (float)(RAND_MAX)) * (-0.6) - 0.5;
			y = ((float)rand() / (float)(RAND_MAX)) * (-0.6) - 0.5;
		}
		else if (rectangle == 3)
		{ /* Rectangle [0.5, 1.1]x[-1.1, -0.5] */
			x = ((float)rand() / (float)(RAND_MAX)) * 0.6 + 0.5;
			y = ((float)rand() / (float)(RAND_MAX)) * (-0.6) - 0.5;
		}
		else if (rectangle == 4)
		{ /* Rectangle [0.5, 1.1]x[0.5, 1.1] */
			x = ((float)rand() / (float)(RAND_MAX)) * 0.6 + 0.5;
			y = ((float)rand() / (float)(RAND_MAX)) * 0.6 + 0.5;
		}

		fprintf(fp, "%f %f \n", x, y);
	}

	fclose(fp);
}

/* @dataset_processing: Reads input data from a .txt file and stores them in a dataset array */
void dataset_processing()
{
	int i;
	float x, y;
	FILE *fp;

	fp = fopen("dataset.txt", "r");

	for (i = 0; i < 500; i++)
	{
		fscanf(fp, "%f", &x);
		fscanf(fp, "%f", &y);

		dataset[i].x = x;
		dataset[i].y = y;
		dataset[i].cluster = 0;
	}

	fclose(fp);
}

/* @init: Selects m random points from the dataset	*/
/*	  to act as initial centroids			*/
void init()
{
	int i, n;

	centroid[num_of_sim] = (cluster_t *)malloc(CLUSTERS * sizeof(cluster_t));

	srand((unsigned int)time(NULL));
	for (i = 0; i < CLUSTERS; i++)
	{
		n = rand() % 500;
		centroid[num_of_sim][i].x = dataset[n].x;
		centroid[num_of_sim][i].y = dataset[n].y;
		printf("Initial centroid representing cluster %d with coordinates x: %f, y: %f.\n", 
				i, centroid[num_of_sim][i].x, centroid[num_of_sim][i].y);
	}
}

/* @recalculate_centroids: Finds the mean coordinates of all the points belonging   */
/*			   to each cluster and assigns the new centroid coordinates */
/*			   of said cluster.					    */	
void recalculate_centroids()
{
	int i, j, cluster_size;
	float sum_of_x, sum_of_y;
	cluster_t *re_centroid;

	printf("------------------------------------------------------\n");
	printf("------------------------------------------------------\n");

	centroid[num_of_sim] = (cluster_t *)realloc(centroid[num_of_sim], (num_of_recalculations + 1) * CLUSTERS * sizeof(cluster_t));
	for (j = 0; j < CLUSTERS; j++)
	{
		sum_of_x = 0.0f;
		sum_of_y = 0.0f;
		cluster_size = 0;
		for (i = 0; i < 500; i++)
		{
			if (dataset[i].cluster == j)
			{
				sum_of_x += dataset[i].x;
				sum_of_y += dataset[i].y;
				cluster_size++;
			}
		}
		centroid[num_of_sim][num_of_recalculations*CLUSTERS + j].x = sum_of_x / cluster_size;
		centroid[num_of_sim][num_of_recalculations*CLUSTERS + j].y = sum_of_y / cluster_size;
		printf("New Centroid %d = x: %f, y: %f.\n", j, centroid[num_of_sim][num_of_recalculations*CLUSTERS + j].x, centroid[num_of_sim][num_of_recalculations*CLUSTERS + j].y);
	}
}

/* @check_centroids: Checks if the centroids have moved in the last two recalculations	*/
/*		     Returns 1 (True) = "Moved" or 0 (False) = "Not moved"		*/
int check_centroids()
{
	int i;

	if (num_of_recalculations == 0)
	{
		return 1;
	}

	for (i = 0; i < CLUSTERS; i++)
	{
		if (centroid[num_of_sim][num_of_recalculations*CLUSTERS + i].x != centroid[num_of_sim][(num_of_recalculations - 1)*CLUSTERS + i].x ||
			centroid[num_of_sim][num_of_recalculations*CLUSTERS + i].y != centroid[num_of_sim][(num_of_recalculations - 1)*CLUSTERS + i].y)
		{
			return 1;
		}
	}
	return 0;
}

/* @euclidean_distance: Calculates the euclidean distance between	*/
/*			a dataset point and a centroid.			*/
float euclidean_distance(int i, int j)
{
	return sqrtf(powf(dataset[i].x - centroid[num_of_sim][num_of_recalculations * CLUSTERS + j].x, 2) +
		powf(dataset[i].y - centroid[num_of_sim][num_of_recalculations * CLUSTERS + j].y, 2));
}

/* @assign_points: Assigns the dataset points to the cluster with the closest centroid */
void assign_points()
{
	int i, j, closest_cluster;
	float current_distance, min_distance;

	for (i = 0; i < 500; i++)
	{
		min_distance = 10.0f;
		for (j = 0; j < CLUSTERS; j++)
		{
			current_distance = euclidean_distance(i, j);
			if (current_distance < min_distance)
			{
				min_distance = current_distance;
				closest_cluster = j;
			}
		}
		dataset[i].cluster = closest_cluster;
	}
}

/* @k_means: Implements the k-means algorithm for clustering */
void k_means()
{
	int i, j, closest_cluster;
	float current_distance, min_distance;

	num_of_recalculations = 0;

	while (check_centroids())
	{
		assign_points();
		num_of_recalculations++;
		recalculate_centroids();
	}
}

/* @calculate_dispersion: Computes the dispersion of a cluster as the sum of squares of	*/
/*			  the differences between the cluster points and the centroid	*/
void calculate_dispersion()
{
	int i;
	float total_dispersion = 0;
	for (i = 0; i < 500; i++)
	{
		total_dispersion += powf(dataset[i].x - centroid[num_of_sim][num_of_recalculations * CLUSTERS + dataset[i].cluster].x, 2);
		total_dispersion += powf(dataset[i].y - centroid[num_of_sim][num_of_recalculations * CLUSTERS + dataset[i].cluster].y, 2);
	}
	if (total_dispersion < min_dispersion)
	{
		min_dispersion = total_dispersion;
		min_simulation = num_of_sim;
		min_sim_recalc = num_of_recalculations;
	}
	printf("Dispersion of Simulation %d: %f.\n", num_of_sim, total_dispersion);
	printf("Recalculations of Simulation %d: %d.\n", num_of_sim, num_of_recalculations);
}

/* @kmeans_main: Executes the k-means algorithm <SIMULATIONS> times and stores the run with 	*/
/*		  the least dispersion. At the end, adjusts the dataset points to show the	*/
/*		  correct cluster assignment.							*/	
int kmeans_main()
{
	int i;

	// Creates a 2D array that holds <SIMULATIONS> number of
	// runs with N * <CLUSTERS>.
	// The clusters are dynamically allocated whenever
	// the @recalculate_centroids method is called.
	centroid = (cluster_t **)malloc(SIMULATIONS * sizeof(cluster_t *));

	seed(); // Generate the dataset
	dataset_processing();

	for (i = 0; i < SIMULATIONS; i++)
	{
		num_of_sim = i;
		printf("^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v\n");
		printf("^v^v^v^v^v^v^v^v^v^  PASSTHROUGH: %d  ^v^v^v^v^v^v^v^v^v^v\n", i);
		printf("^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v\n");

		init();
		k_means();
		calculate_dispersion();
		sleep(2);
	}

	/* The final simulation leaves the dataset points assigned to some clusters based on the centroids	*/
	/* found on THAT simulation. In order to project the dataset points assigned to the correct cluster,	*/
	/* we reassign them based on the centroids found in the simulation with the	least dispersion.	*/
	num_of_sim = min_simulation;
	num_of_recalculations = min_sim_recalc;
	assign_points();

	return 0;
}
