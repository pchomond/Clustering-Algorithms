#include "headers/k_means.h"
#include <GL/freeglut.h>

#define KEY_ESC 27

int redraw_counter = 0;

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
	int i, j;

	for (i = 0; i < 500; i++) {
		glPointSize(4.0f);
		glBegin(GL_POINTS);
		glColor3fv(colors[dataset[i].cluster]);
		glVertex2f((float)dataset[i].x, (float)dataset[i].y);
		glEnd();
	}
	for (j = 0; j < CLUSTERS; j++)
	{
		glPointSize(8.0f);
		glBegin(GL_POINTS);
		glColor3fv(colors[7]);
		glVertex2f(centroid[min_simulation][redraw_counter * CLUSTERS + j].x, centroid[min_simulation][redraw_counter * CLUSTERS + j].y);
		glEnd();
	}
	sleep(2); // Make the change in centroid values visible by slowing down the redraw time
	if (redraw_counter < min_sim_recalc)
	{
		redraw_counter++;
		glutPostRedisplay();
	}
}

void free_data()
{
	int i;

	for (i = 0; i < SIMULATIONS; i++)
	{
		free(centroid[i]);
		centroid[i] = NULL;
	}
	free(centroid);
	centroid = NULL;
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
		free_data();
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
	glutCreateWindow("K-MEANS Visualization");

	init_GL();

	/* Execute the k-means algorithm multiple times on the same dataset */
	/* Keep and render the solution with the least dispersion			*/
	kmeans_main();

	// Register callbacks
	glutDisplayFunc(render_scene);
	glutReshapeFunc(reshape_scene);
	//glutIdleFunc(render_scene);
	glutKeyboardFunc(keyboard_func);
	glutMouseFunc(mouse_func);

	glutMainLoop();

	return 1;
}
