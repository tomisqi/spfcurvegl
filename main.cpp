#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include "color.h"
#include "vector.h"
#include "hilbert.h"
#include "peano.h"
#include "string.h"

// TODOs
// [x] Build and run from command line
// [x] Draw hilbert curve based on level
// [x] Peano 
// [ ] Readme
// [ ] Finish curve.h

#define WINDOW_SIZE 900

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void DrawRect(Vector2 pos, Vector2 size, Color color)
{
	glColor4f(color.r, color.g, color.b, color.a);
	glBegin(GL_LINE_LOOP);
	{
		glVertex2f(pos.x, pos.y);
		glVertex2f(pos.x + size.x, pos.y);
		glVertex2f(pos.x + size.x, pos.y + size.y);
		glVertex2f(pos.x, pos.y + size.y);
	}
	glEnd();
}

static void DrawRectFill(Vector2 pos, Vector2 size, Color color)
{
	glColor4f(color.r, color.g, color.b, color.a);
	glBegin(GL_QUADS);
	{
		glVertex2f(pos.x, pos.y);
		glVertex2f(pos.x + size.x, pos.y);
		glVertex2f(pos.x + size.x, pos.y + size.y);
		glVertex2f(pos.x, pos.y + size.y);
	}
	glEnd();
}

static void DrawLine(Vector2 start, Vector2 end, Color color)
{
	glColor4f(color.r, color.g, color.b, color.a);
	glBegin(GL_LINES);
	{
		glVertex2f(start.x, start.y);
		glVertex2f(end.x, end.y);
	}
	glEnd();
}

static void DrawCurve(Vector2 points[], int count, Color color)
{
	glColor4f(color.r, color.g, color.b, color.a);
	glLineWidth(4.0f);
	glBegin(GL_LINES);
	for (int i = 1; i < count; i++)
	{
		glVertex2f(points[i - 1].x, points[i - 1].y);
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();
}

static void DrawGrid(Vector2 tileSize, Color color)
{
	glColor4f(color.r, color.g, color.b, color.a);
	glLineWidth(1.0f);
	glBegin(GL_LINES);

	// vertical lines
	int iter = WINDOW_SIZE / tileSize.x;
	float x = 0;
	for (int i = 0; i < iter; i++)
	{
		glVertex2f(x, 0);
		glVertex2f(x, WINDOW_SIZE);
		x += tileSize.x;
	}

	// horizontal lines
	iter = WINDOW_SIZE / tileSize.y;
	float y = 0;
	for (int i = 0; i < iter; i++)
	{
		glVertex2f(0, y);
		glVertex2f(WINDOW_SIZE, y);
		y += tileSize.y;
	}

	glEnd();
}

static void ShowHelp()
{
	printf("options: -hilbert <level>\n");
	printf("         -peano   <level>\n");
}

#define HILBERT_LEVEL 3
#define PEANO_LEVEL 3
int main(int argc, char** argv)
{
	Curve curve;
	if (argc > 1)
	{
		int level = argc > 2 ? strtol(argv[2], NULL, 10) : 1;
		if (!strcmp(argv[1], "-hilbert"))
		{
			curve = HilbertGenCurve(level, WINDOW_SIZE);
		}
		else if (!strcmp(argv[1], "-peano"))
		{
			curve = PeanoGenCurve(level, WINDOW_SIZE);
		}
		else
		{
			ShowHelp();
			return 1;
		}
	}
	else
	{
		// Generate a level-2 Hilbert curve if nothing is passed
		ShowHelp();
		curve = HilbertGenCurve(2, WINDOW_SIZE);
	}

	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit()) return 1;
	GLFWwindow* window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "spfcurvegl", NULL, NULL);
	if (window == NULL) return 1;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	Color clearColor = COLOR_BLACK;
	Color gridColor = Col(119, 119, 119);//gray
	Color curveColor = COLOR_YELLOW;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		glClear(GL_COLOR_BUFFER_BIT);

		// ignore model view matrix - dont need it
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// load a different projection matrix
		// this will let us use a screen space coordinates that are then transformed to clip space by opengl
		// Pcl= Mproj x P
		// where,
		//  Pcl: point in clip space
		//  Mproj: Projection maxtrix
		//  P: point in screen space
		glMatrixMode(GL_PROJECTION);
		float v = 2.0f / WINDOW_SIZE;
		float Mproj[] =
		{
		 	 v,  0,  0,  0,
			 0,  v,  0,  0,
			 0,  0,  1,  0,
			-1, -1,  0,  1,
		};
		glLoadMatrixf(Mproj);

		// Draw the grid and the curve
		DrawGrid(V2(curve.tileSize, curve.tileSize), gridColor);
		DrawCurve(curve.points, curve.pointCount, curveColor);

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}