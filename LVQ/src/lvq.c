#include "headers/lvq.h"
#include <time.h>
#include <unistd.h>

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*	       GLOBALS 	  	  */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

point_t dataset[500]; 		// The array that holds the dataset
cluster_t ***centroid_heads; 	// The heads of the linked lists of the centroid values of all the simulations
cluster_t ***centroid_tails;	// The tails of the linked lists of the centroid values of all the simulations
int num_of_sim; 		// The number of the current simulation
float min_dispersion = INFINITY;// The least dispersion found
int min_simulation; 		// The simulation with the least dispersion

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*	       FUNCTIONS 	  */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* @seed: Creates a dataset.txt file with 500 two-dimensional points.		*/
/*		  Feel free to comment out the call to this function in 	*/
/*		  lvq_main to maintain the dataset for more than one run. 	*/
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
/*		  to act as initial centroids		*/
void init()
{
	int i, n;
	cluster_t *centroid;
	sleep(2); // Used to give srand a bit of time to seed "random" numbers
	srand((unsigned int)time(NULL));
	for (i = 0; i < CLUSTERS; i++)
	{
		n = rand() % 500;
		printf("Random: %d.\n", n);
		centroid = (cluster_t *)malloc(sizeof(cluster_t));
		centroid->x = dataset[n].x;
		centroid->y = dataset[n].y;
		centroid->next = NULL;
		centroid_heads[num_of_sim][i] = centroid;
		centroid_tails[num_of_sim][i] = centroid;
		printf("Initial centroid representing cluster %d with coordinates x: %f, y: %f.\n",
			i, centroid_heads[num_of_sim][i]->x, centroid_heads[num_of_sim][i]->y);
	}
}

/* @euclidean_distance: Calculates the euclidean distance between	*/
/*			a dataset point and a centroid.			*/
float euclidean_distance(int i, int j, int simulation)
{
	return sqrtf(powf(dataset[i].x - centroid_tails[simulation][j]->x, 2) +
				powf(dataset[i].y - centroid_tails[simulation][j]->y, 2));
}

/* @assign_points_to_cluster: Reassigns the points to the correct clusters.	*/	
void assign_points_to_clusters(int simulation)
{
	int i, j, closest_cluster;
	float min_distance, point_distance;

	for (i = 0; i < 500; i++)
	{
		min_distance = INFINITY;
		for (j = 0; j < CLUSTERS; j++)
		{
			point_distance = euclidean_distance(i, j, simulation);
			if (point_distance < min_distance)
			{
				min_distance = point_distance;
				closest_cluster = j;
			}
		}
		dataset[i].cluster = closest_cluster;
	}
}

/* @lvq: Executes the LearningVectorQuantization algorithm for clustering */
void lvq()
{
	float a, min_distance, point_distance;
	int i, j, closest_cluster;
	cluster_t *centroid;

	a = 0.1f; // Learning factor

	while (a > 0.05f)
	{
		for (i = 0; i < 500; i++)
		{
			min_distance = INFINITY;
			for (j = 0; j < CLUSTERS; j++)
			{
				point_distance = euclidean_distance(i, j, num_of_sim);
				if (point_distance < min_distance)
				{
					min_distance = point_distance;
					closest_cluster = j;
				}
			}
			dataset[i].cluster = closest_cluster;

			/* Create a new centroid struct with the new recalculated coordinates */
			centroid = (cluster_t *)malloc(sizeof(cluster_t));
			centroid->x = centroid_tails[num_of_sim][closest_cluster]->x 
						+ a * (dataset[i].x - centroid_tails[num_of_sim][closest_cluster]->x);
			centroid->y = centroid_tails[num_of_sim][closest_cluster]->y 
						+ a * (dataset[i].y - centroid_tails[num_of_sim][closest_cluster]->y);
			centroid->next = NULL;

			/* Add the new centroid at the end of the linked list of centroids	*/
			/* for the particular cluster <closest_cluster>				*/
			centroid_tails[num_of_sim][closest_cluster]->next = centroid;
			centroid_tails[num_of_sim][closest_cluster] = centroid;
		}
		a = a * 0.95f; // Adjust learning factor by 95%
	}
	/* One final loop to assign the points to the correct clusters */
	assign_points_to_clusters(num_of_sim);
}

/* @calculate_dispersion: Computes the dispersion of a cluster as the sum of the squares	*/
/*			  of the differences between the cluster points and the centroid	*/
void calculate_dispersion()
{
	int i;
	float total_dispersion = 0;
	for (i = 0; i < 500; i++)
	{
		total_dispersion += powf(dataset[i].x - centroid_tails[num_of_sim][dataset[i].cluster]->x, 2);
		total_dispersion += powf(dataset[i].y - centroid_tails[num_of_sim][dataset[i].cluster]->y, 2);
	}
	if (total_dispersion < min_dispersion)
	{
		min_dispersion = total_dispersion;
		min_simulation = num_of_sim;
	}
	printf("Dispersion of Simulation %d: %f.\n", num_of_sim, total_dispersion);
}

/* @lvq_main: Orchestrates the whole program by initializing arrays and centroids. */
void lvq_main()
{
	int i;
	/* Initialize the head and tail pointers to all linked lists for all simulation */
	centroid_heads = (cluster_t ***)malloc(SIMULATIONS * sizeof(cluster_t **));
	centroid_tails = (cluster_t ***)malloc(SIMULATIONS * sizeof(cluster_t **));
	for (i = 0; i < SIMULATIONS; i++)
	{
		/* Creates an array that holds <CLUSTERS> amount of centroids. */
		centroid_heads[i] = (cluster_t **)malloc(CLUSTERS * sizeof(cluster_t *));
		centroid_tails[i] = (cluster_t **)malloc(CLUSTERS * sizeof(cluster_t *));
	}

	seed(); // Generate the dataset
	dataset_processing();
	for (i = 0; i < SIMULATIONS; i++)
	{
		num_of_sim = i;
		printf("^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v\n");
		printf("^v^v^v^v^v^v^v^v^v^  PASSTHROUGH: %d  ^v^v^v^v^v^v^v^v^v^v\n", i);
		printf("^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v^v\n");

		init();
		lvq();
		calculate_dispersion();
	}
	assign_points_to_clusters(min_simulation);
	for (i = 0; i < CLUSTERS; i++)
	{
		printf("Final position of cluster %d = x: %f, y: %f.\n",
			i, centroid_tails[min_simulation][i]->x, centroid_tails[min_simulation][i]->y);
	}
}
