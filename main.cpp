#include <stdio.h>
#include <GLFW/glfw3.h>
#include "color.h"
#include "vector.h"

// TODOs
// [ ] Build and run from command line
// [ ] Pass hilbert level via command line
// [x] Draw hilbert curve based on level
// [ ] Readme

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
	glBegin(GL_LINES);
	for (int i = 0; i < count; i++)
	{
		glVertex2f(points[i].x, points[i].y);
	}
	glEnd();
}

static Vector2 DrawLineTranslate(Vector2 start, Vector2 translation, Color color)
{
	Vector2 end = start + translation;
	DrawLine(start, end, color);
	return end;
}

static void DrawGrid(Vector2 tileSize, Color color)
{
	glColor4f(color.r, color.g, color.b, color.a);
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

static float GetHilbertTileSize(int screenSize, int hilbertLevel)
{
	float result = screenSize / (2 << (hilbertLevel - 1));
	return result;
}

static Vector2 Hilbert(int level, float tileSize, Vector2 pos, Vector2 direction)
{
	Vector2 newPos = pos;
	if (level > 0)
	{
		if (direction == VECTOR2_LEFT)
		{
			newPos = Hilbert(level - 1, tileSize, newPos, VECTOR2_UP);
			newPos = DrawLineTranslate(newPos, tileSize * VECTOR2_RIGHT, COLOR_GREEN);
			newPos = Hilbert(level - 1, tileSize, newPos, VECTOR2_LEFT);
			newPos = DrawLineTranslate(newPos, tileSize * VECTOR2_DOWN, COLOR_GREEN);
			newPos = Hilbert(level - 1, tileSize, newPos, VECTOR2_LEFT);
			newPos = DrawLineTranslate(newPos, tileSize * VECTOR2_LEFT, COLOR_GREEN);
			newPos = Hilbert(level - 1, tileSize, newPos, VECTOR2_DOWN);
		}
		if (direction == VECTOR2_RIGHT)
		{
			newPos = Hilbert(level - 1, tileSize, newPos, VECTOR2_DOWN);
			newPos = DrawLineTranslate(newPos, tileSize * VECTOR2_LEFT, COLOR_GREEN);
			newPos = Hilbert(level - 1, tileSize, newPos, VECTOR2_RIGHT);
			newPos = DrawLineTranslate(newPos, tileSize * VECTOR2_UP, COLOR_GREEN);
			newPos = Hilbert(level - 1, tileSize, newPos, VECTOR2_RIGHT);
			newPos = DrawLineTranslate(newPos, tileSize * VECTOR2_RIGHT, COLOR_GREEN);
			newPos = Hilbert(level - 1, tileSize, newPos, VECTOR2_UP);
		}
		if (direction == VECTOR2_UP)
		{
			newPos = Hilbert(level - 1, tileSize, newPos, VECTOR2_LEFT);
			newPos = DrawLineTranslate(newPos, tileSize * VECTOR2_DOWN, COLOR_GREEN);
			newPos = Hilbert(level - 1, tileSize, newPos, VECTOR2_UP);
			newPos = DrawLineTranslate(newPos, tileSize * VECTOR2_RIGHT, COLOR_GREEN);
			newPos = Hilbert(level - 1, tileSize, newPos, VECTOR2_UP);
			newPos = DrawLineTranslate(newPos, tileSize * VECTOR2_UP, COLOR_GREEN);
			newPos = Hilbert(level - 1, tileSize, newPos, VECTOR2_RIGHT);
		}
		if (direction == VECTOR2_DOWN)
		{
			newPos = Hilbert(level - 1, tileSize, newPos, VECTOR2_RIGHT);
			newPos = DrawLineTranslate(newPos, tileSize * VECTOR2_UP, COLOR_GREEN);
			newPos = Hilbert(level - 1, tileSize, newPos, VECTOR2_DOWN);
			newPos = DrawLineTranslate(newPos, tileSize * VECTOR2_LEFT, COLOR_GREEN);
			newPos = Hilbert(level - 1, tileSize, newPos, VECTOR2_DOWN);
			newPos = DrawLineTranslate(newPos, tileSize * VECTOR2_DOWN, COLOR_GREEN);
			newPos = Hilbert(level - 1, tileSize, newPos, VECTOR2_LEFT);
		}
	}

	return newPos;
}

static void DrawHilbertCurve(int level)
{
	float tileSize = GetHilbertTileSize(WINDOW_SIZE, level);
	Vector2 startPos = V2(0.0f, WINDOW_SIZE) + V2(tileSize / 2.0f, -tileSize / 2.0f); // startPos is the middle of the upper-left tile
	Hilbert(level, tileSize, startPos, VECTOR2_UP);
}

#define HILBERT_LEVEL 3
int main(int argc, char** argv)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit()) return 1;
	GLFWwindow* window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "hilbertgl", NULL, NULL);
	if (window == NULL) return 1;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	Color clearColor = COLOR_BLACK;
	Color gridColor = COLOR_WHITE;
	Color curveColor = COLOR_GREEN;
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
		float projM[] =
		{
		 	 v,  0,  0,  0,
			 0,  v,  0,  0,
			 0,  0,  1,  0,
			-1, -1,  0,  1,
		};
		glLoadMatrixf(projM);

		float tileSize = GetHilbertTileSize(WINDOW_SIZE, HILBERT_LEVEL);
		DrawGrid(V2(tileSize, tileSize), gridColor); // uncomment to see grid
		DrawHilbertCurve(HILBERT_LEVEL);

		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}