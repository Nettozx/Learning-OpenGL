#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
};

static bool GLLogCall()
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")\n";
		return false;
	}
	return true;
}

//so we can return multiple things
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		//alloca lets you allocate on the stack dynamically
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << 
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") 
			<< " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	//shader is just code that runs on the gpu
	//vertex shaders are called 3 times, fragment shaders are called per pixel
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

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
	float positions[] = {
	   -0.5f, -0.5f, //x,y of vertex
		0.5f, -0.5f,
		0.5f,  0.5f,
	   -0.5f,  0.5f
	};

	//index buffer, so we don't have redundant positions
	unsigned int indicies[] =
	{
		0, 1, 2, //refers to lines of xy in positions[]
		2, 3, 0
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer); //generate buffer with id and address
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //select buffer
	//put data in buffer
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);
	
	//enable vertexattrib
	glEnableVertexAttribArray(0);
	//index 0 because its first attirbute
	//2 coordinate position so size is 2
	//type of positions is floats
	//no normalization yet
	//amount of offset to get to the next vertex = 2 floats the x and y
	//no offset
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	//use our index buffer to draw a square
	unsigned int ibo;
	glGenBuffers(1, &ibo); //generate buffer with id and address
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //select buffer
	//put data in buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indicies, GL_STATIC_DRAW);

	//make a shader
	ShaderProgramSource source = ParseShader("Basic.shader");

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//clear errors
		GLClearError();
		//this draws the last item that was selected using glBindBuffer
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		//get errors
		ASSERT(GLLogCall());

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}