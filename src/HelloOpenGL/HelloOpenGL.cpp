#include "OpenGLSampleBase.h"

class HelloOpenGL :public OpenGLSampleBase
{
protected:
	void StartUp() override
	{
		//_vertexShaderSource =
		//"#version 430 core\n"
		//"void main()\n"
		//"{\n"
		//"gl_Position = vec4(0.0, 0.0, 0.5, 1.0);\n"
		//"}\n";
		//
		//_fragmentShaderSource =
		//"#version 430 core\n"
		//"out vec4 color;\n"
		//"void main()\n"
		//"{\n"
		//"color = vec4(0.0, 0.8, 1.0, 1.0);\n"
		//"}\n";

		_vertexShaderSource =
			"#version 430 core\n"
			"const vec4 verts[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),vec4(-0.25, -0.25, 0.5, 1.0),vec4( 0.25, 0.25, 0.5, 1.0));\n"
			"void main()\n"
			"{\n"
			"gl_Position = verts[gl_VertexID];\n"
			"}\n";

		_fragmentShaderSource =
			"#version 430 core\n"
			"out vec4 color;\n"
			"void main()\n"
			"{\n"
			"color = vec4(0.0, 0.8, 1.0, 1.0);\n"
			"}\n";

		_program = CompileShaders(_vertexShaderSource, _fragmentShaderSource);	
	}

	void Render()
	{
		static const GLfloat red[] = { 1.0f,0.6f,0.0f,0.5f };
		glClearBufferfv(GL_COLOR, 0, red);
		
		glUseProgram(_program);

		//glPointSize(40.0f);
		//glDrawArrays(GL_POINTS, 0, 1);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void EndUp()
	{
		glDeleteProgram(_program);
	}
};

DECLARE_MAIN(HelloOpenGL)