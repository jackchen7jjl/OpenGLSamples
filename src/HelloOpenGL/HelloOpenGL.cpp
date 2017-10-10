#include "sb6.h"
#include <math.h>
#include <iostream>
using namespace std;

class my_application :public sb6::application
{
private:
	GLuint rendering_program;
	GLuint vertex_arrray_object;

public:
	void render(double currentTime)
	{			
		const GLfloat red[] = { 0,0, 0.0, 1.0 };
		glClearBufferfv(GL_COLOR,0,red);

		glUseProgram(rendering_program);
		
		glDrawArrays(GL_TRIANGLES,0,3);
	}

	void shutdown()
	{
		glDeleteVertexArrays(1, &vertex_arrray_object);
		glDeleteProgram(rendering_program);
	}

	void startup()
	{
		rendering_program = compile_shaders();

		const float data[] =
		{
			0.25,-0.25,0.5,1.0,  1.0,0.0,0.0,1.0,
			-0.25,-0.25,0.5,1.0, 0.0,1.0,0.0,1.0,
			0.25,0.25,0.5,1.0,   0.0,0.0,1.0,1.0
		};

		glGenVertexArrays(1, &vertex_arrray_object);
		glBindVertexArray(vertex_arrray_object);

		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 32, (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 32, (void*)16);
		glEnableVertexAttribArray(1);
	}


	
	GLuint compile_shaders(void)
	{
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;


		const GLchar* vertex_shader_source =
			"#version 430 core\n"
			"layout (location = 0) in vec4 position;\n"
			"layout (location = 1) in vec4 color;\n"
			"out vec4 vs_color;\n"
			"void main()\n"
			"{\n"
			"gl_Position = position;\n"
			"vs_color = color;\n"
			"}\n";

		const GLchar* fragment_shader_source =
			"#version 430 core\n"
			"in vec4 vs_color;\n"
			"out vec4 color;\n"
			"void main()\n"
			"{\n"
			"color = vs_color;\n"
			"}\n";

	
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1,&vertex_shader_source, NULL);
		glCompileShader(vertex_shader);
		checkCompiletResult(vertex_shader);

		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
		glCompileShader(fragment_shader);
		checkCompiletResult(fragment_shader);

		program = glCreateProgram();
		glAttachShader(program,vertex_shader);
		glAttachShader(program,fragment_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}

	void checkCompiletResult(GLuint shader)
	{
		GLint compileResult = GL_TRUE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
		if (compileResult == GL_FALSE)
		{
			char szlog[1024] = { 0 };
			GLsizei loglen = 0;
			glGetShaderInfoLog(shader,1024,&loglen,szlog);
			cerr << "compilete shader error: " << szlog << endl;
		}
	}
};

DECLARE_MAIN(my_application);