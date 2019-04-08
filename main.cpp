#include <stdio.h>
#include <GLFW/glfw3.h>

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int argc, char** argv)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	printf("Hello world\n");
	return 1;
}