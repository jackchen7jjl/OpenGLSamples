#include "OpenGLSampleBase.h"
#include <iostream>

bool OpenGLSampleBase::Init()
{
	glfwSetErrorCallback(ErrorCallback);

	if (!glfwInit())
	{
		return false;
	}

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	_window = glfwCreateWindow(1024, 768, "Hello GLFW", NULL, NULL);

	if (!_window)
	{
		glfwTerminate();
		return false;
	}

	glfwSetKeyCallback(_window, KeyCallback);

	glfwMakeContextCurrent(_window);
	//glfwSwapInterval(1);

	glewExperimental = true; // Needed for core profile

	GLenum err = glewInit();

	if (err != 0)
		return false;
	
	std::cout << glGetString(GL_VERSION) << std::endl;

	return true;
}

void OpenGLSampleBase::Run()
{
	glGenVertexArrays(1, &_vertexArrayObject);
	glBindVertexArray(_vertexArrayObject);

	StartUp();

	while (!glfwWindowShouldClose(_window))
	{
		Render();

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
	
	EndUp();
	glDeleteVertexArrays(1, &_vertexArrayObject);

	glfwTerminate();
}

GLuint OpenGLSampleBase::LoadShader(GLenum type, const char *shaderSrc)
{
	// Create the shader object
	GLuint shader = glCreateShader(type);

	if (shader == 0)
		return 0;

	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL);

	// Compile the shader
	glCompileShader(shader);

	//Check the compile status
	GLint compileResult = GL_TRUE;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

	if (compileResult == GL_FALSE)
	{
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = (char*)malloc(sizeof(char) * infoLen);

			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);

			std::cout << "compilete shader error: " << infoLog << std::endl;
		}
		return 0;
	}

	return shader;
}

GLuint OpenGLSampleBase::CompileShaders(const char *vertextShader, const char *fragmentShader)
{
	GLuint vertex_shader = LoadShader(GL_VERTEX_SHADER, vertextShader);

	GLuint fragment_shader = LoadShader(GL_FRAGMENT_SHADER, fragmentShader);

	if ((!vertex_shader) || (!fragment_shader))
		return 0;

	GLuint program = glCreateProgram();

	glAttachShader(program, vertex_shader);

	glAttachShader(program, fragment_shader);

	glLinkProgram(program);

	glDeleteShader(vertex_shader);

	glDeleteShader(fragment_shader);

	if (program == 0)
		std::cout << "CompileShaders error" << std::endl;

	return program;
}

void OpenGLSampleBase::ErrorCallback(int error, const char* description)
{
	std::cout << "err" << description << std::endl;
}

void OpenGLSampleBase::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}
