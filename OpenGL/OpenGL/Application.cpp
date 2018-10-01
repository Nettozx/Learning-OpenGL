#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* initialize glew */
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	//triangle positions
	float positions[6] = {
	   -0.5f, -0.5f, //x,y of vertex
		0.0f,  0.5f,
		0.5f, -0.5f
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer); //generate buffer with id and address
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //select buffer
	//put data in buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
	
	//enable vertexattrib
	glEnableVertexAttribArray(0);
	//index 0 because its first attirbute
	//2 coordinate position so size is 2
	//type of positions is floats
	//no normalization yet
	//amount of offset to get to the next vertex = 2 floats the x and y
	//no offset
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//this draws the last item that was selected using glBindBuffer
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}