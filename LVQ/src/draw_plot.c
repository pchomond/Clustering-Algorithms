#include "headers/lvq.h"
#include <GL/freeglut.h>

#define KEY_ESC 27

cluster_t **drawn_centroids;

float colors[8][3] = {
	{ 1.0f, 0.0f, 0.0f },	// RED
	{ 0.0f, 0.5f, 0.0f },	// GREEN
	{ 0.0f, 0.0f, 1.0f },	// BLUE
	{ 1.0f, 0.0f, 1.0f },	// MAGENTA
	{ 0.0f, 0.5f, 1.0f },	// DARK BLUE
	{ 0.5f, 0.5f, 0.0f },	// OLIVE
	{ 1.0f, 0.5f, 0.0f },	// ORANGE
	{ 0.0f, 0.0f, 0.0f }	// BLACK
};

void draw_axes()
{
	float i;

	glBegin(GL_LINE_STRIP);
	glColor3f(1, 0, 0);
	glVertex2f(-2.0f, 0.0f);
	glVertex2f(2.0f, 0.0f);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 1);
	glVertex2f(0.0f, -2.0f);
	glVertex2f(0.0f, 2.0f);
	glEnd();

	for (i = 0.5; i <= 2; i += 0.5) {
		glBegin(GL_LINES);
		glColor3f(0, 0, 0);
		glVertex2f(i, 0.05f);
		glVertex2f(i, -0.05f);
		glVertex2f(-i, 0.05f);
		glVertex2f(-i, -0.05f);
		glVertex2f(0.05f, i);
		glVertex2f(-0.05f, i);
		glVertex2f(0.05f, -i);
		glVertex2f(-0.05f, -i);
		glEnd();
	}
}

void draw_points()
{
	int i;

	for (i = 0; i < 500; i++) {
		glPointSize(4.0f);
		glBegin(GL_POINTS);
		glColor3fv(colors[dataset[i].cluster]);
		glVertex2f((float)dataset[i].x, (float)dataset[i].y);
		glEnd();
	}
	for (i = 0; i < CLUSTERS; i++)
	{
		glPointSize(8.0f);
		glBegin(GL_POINTS);
		glColor3fv(colors[7]);
		glVertex2f(drawn_centroids[i]->x, drawn_centroids[i]->y);
		glEnd();
		
		if (drawn_centroids[i]->next != NULL)
		{
			drawn_centroids[i] = drawn_centroids[i]->next;
		}
	}
}

void init_GL()
{
	glClearColor(1, 1, 1, 0);
}

// Simple reshape callback in case the window is resized
void reshape_scene(int w, int h)
{
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glViewport(0, 0, w, h);

	glMatrixMode(GL_MODELVIEW);
}

void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.5, 1.5, -1.5, 1.5);
	glMatrixMode(GL_MODELVIEW);

	draw_axes();

	draw_points();

	glutSwapBuffers();
}

void keyboard_func(unsigned char key, int x, int y)
{
	switch (key)
	{
	case KEY_ESC:
		exit(1);
		break;
	default:
		printf("Pressing %d doesn't do anything.\n", key);
		break;
	}
}

void mouse_func(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		printf("x: %d, y: %d.\n", x, y);
	}
}

int main(int argc, char **argv)
{
	int i;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(600, 300);
	glutInitWindowSize(600, 500);
	glutCreateWindow("LVQ Visualization");

	init_GL();

	/* Execute the LVQ algorithm repetitively on the same dataset.	*/
	/* Render the solution with the least dispersion.		*/
	lvq_main();

	/* Used to loop through the linked list of centroid values */
	drawn_centroids = (cluster_t **)malloc(CLUSTERS * sizeof(cluster_t *));
	for (i = 0; i < CLUSTERS; i++)
	{
		drawn_centroids[i] = centroid_heads[min_simulation][i];
	}

	/* Register callbacks */
	glutDisplayFunc(render_scene);
	glutReshapeFunc(reshape_scene);
	glutIdleFunc(render_scene);
	glutKeyboardFunc(keyboard_func);
	glutMouseFunc(mouse_func);

	glutMainLoop();

	return 1;
}
