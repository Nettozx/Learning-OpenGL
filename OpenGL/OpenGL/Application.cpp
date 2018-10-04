#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

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

	//make framerate match monitor vsync
	glfwSwapInterval(1);

	/* initialize glew */
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		//triangle positions
		float positions[] = {
		   -0.5f, -0.5f, 0.0f, 0.0f, //x,y of vertex, then 2 texture coords
			0.5f, -0.5f, 1.0f, 0.0f,
			0.5f,  0.5f, 1.0f, 1.0f,
		   -0.5f,  0.5f, 0.0f, 1.0f
		};

		//index buffer, so we don't have redundant positions
		unsigned int indicies[] =
		{
			0, 1, 2, //refers to lines of xy in positions[]
			2, 3, 0
		};

		//how opengl blends transperancy
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2); //vertex coords
		layout.Push<float>(2); //texture coords
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indicies, 6);

		//make a shader
		Shader shader("Basic.shader");
		shader.Bind();

		//uniforms let you send data from cpu to gpu
		shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

		//set a texture
		Texture texture("ZERO.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		//unbind everything
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;

		float r = 0.0f;
		float increment = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			shader.Bind();
			//animate colors by changing value of red
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			renderer.Draw(va, ib, shader);

			//change value of red
			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;
			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}