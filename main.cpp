#include <stdio.h>
#include <GLFW/glfw3.h>

#define WINDOW_SIZE 900


static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int argc, char** argv)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit()) return 1;
	GLFWwindow* window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "hilbertgl", NULL, NULL);
	if (window == NULL) return 1;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);
		glClearColor(0.0f, 0.0f, 1.00f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);

		// ignore model view matrix - dont need it
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// load a different projection matrix
		// this will let us use a screen space coordinates that are then transformed to clip space by opengl
		// any point Pcl= Mproj x P
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

		// draw square
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS); // Start drawing a quad primitive
		{
			glVertex2d(0.0f, 0.0f);
			glVertex2d(450.0f, 0.0f);
			glVertex2d(450.0f, 450.0f);
			glVertex2d(0.0f, 450.0f);
		}
		glEnd();

		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}