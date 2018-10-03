#include "Renderer.h"

#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function <<
			" " << file << ":" << line << "\n";
		return false;
	}
	return true;
}

void Renderer::Draw(const VertexArray & va, const IndexBuffer & ib, const Shader & shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	//this draws the last item that was selected using glBindBuffer
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
