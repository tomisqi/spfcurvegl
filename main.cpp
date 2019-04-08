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

		glClearColor(0.45f, 0.55f, 1.00f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}