#include <stdio.h>
#include <GLFW/glfw3.h>

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int argc, char** argv)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit()) return 1;
	GLFWwindow* window = glfwCreateWindow(900, 900, "hilbertgl", NULL, NULL);
	if (window == NULL) return 1;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 1.00f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw square
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_QUADS); // Start drawing a quad primitive
		{
			glVertex2d(-0.5f, -0.5f);
			glVertex2d(0.5f, -0.5f);
			glVertex2d(0.5f, 0.5f);
			glVertex2d(-0.5f, 0.5f);
		}
		glEnd();

		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}