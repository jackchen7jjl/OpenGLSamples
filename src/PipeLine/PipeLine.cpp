#include <math.h>
#include "OpenGLSampleBase.h"

class PipeLine :public OpenGLSampleBase
{
protected:
	void StartUp() override
	{
		_vertexShaderSource =
			"#version 430 core\n"
			"layout(location=0) in vec4 offset;\n"
			"layout(location=1) in vec4 color;\n"
			"out VS_OUT\n"
			"{\n"
			"vec4 color;\n"
			"} vs_out;\n"
			"const vec4 verts[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),vec4(-0.25, -0.25, 0.5, 1.0),vec4( 0.25, 0.25, 0.5, 1.0));\n"
			"void main()\n"
			"{\n"
			"gl_Position = verts[gl_VertexID] + offset;\n"
			"vs_out.color = color;\n"
			"}\n";

		_fragmentShaderSource =
			"#version 430 core\n"
			"in VS_OUT\n"
			"{\n"
			"vec4 color;\n"
			"} fs_in;\n"
			"out vec4 color;\n"
			"void main()\n"
			"{\n"
			//"color = vec4(0.0, 0.8, 1.0, 1.0);\n"
			"color = fs_in.color;\n"
			"}\n";

		_program = CompileShaders(_vertexShaderSource, _fragmentShaderSource);	
	}

	void Render() override
	{
		static const GLfloat red[] = { 1.0f,0.6f,0.0f,0.5f };
		glClearBufferfv(GL_COLOR, 0, red);
		
		glUseProgram(_program);
		
		double t = glfwGetTime();
		GLfloat attrib[] = {(float)sin(t) * 0.5f,(float)cos(t) * 0.5f,0.0f,0.0f};

		glVertexAttrib4fv(0, attrib);
		glVertexAttrib4fv(1, attrib);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void EndUp() override
	{
		glDeleteProgram(_program);
	}
};



//================================================(Tessellation)===========================================================
class Tessellation :public OpenGLSampleBase
{
protected:
	void StartUp() override
	{
		_vertexShaderSource =
			"#version 430 core\n"
			"layout(location=0) in vec4 offset;\n"
			"layout(location=1) in vec4 color;\n"
			"out VS_OUT\n"
			"{\n"
			"vec4 color;\n"
			"} vs_out;\n"
			"const vec4 verts[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),vec4(-0.25, -0.25, 0.5, 1.0),vec4( 0.25, 0.25, 0.5, 1.0));\n"
			"void main()\n"
			"{\n"
				//"gl_Position = verts[gl_VertexID] + offset;\n"
				"gl_Position = verts[gl_VertexID];\n"
				"vs_out.color = color;\n"
			"}\n";

		GLchar *tcs =
			"#version 430 core\n"
			"layout(vertices=3) out;\n"
			"void main()\n"
			"{\n"
				"if(gl_InvocationID == 0)\n"
				"{\n"
					"gl_TessLevelInner[0] = 5.0;\n"
					"gl_TessLevelOuter[0] = 5.0;\n"
					"gl_TessLevelOuter[1] = 5.0;\n"
					"gl_TessLevelOuter[2] = 5.0;\n"
				"}\n"
				"gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;\n"
			"}\n";

		GLchar *tes =
			"#version 430 core\n"
			"layout(triangles,equal_spacing,cw) in;\n"
			"void main()\n"
			"{\n"
			"gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position + gl_TessCoord.y * gl_in[1].gl_Position + gl_TessCoord.z * gl_in[2].gl_Position);\n"
			"}\n";

		_fragmentShaderSource =
			"#version 430 core\n"
			"in VS_OUT\n"
			"{\n"
			"vec4 color;\n"
			"} fs_in;\n"
			"out vec4 color;\n"
			"void main()\n"
			"{\n"
			"color = fs_in.color;\n"
			"}\n";

		GLuint vertex_shader = LoadShader(GL_VERTEX_SHADER, _vertexShaderSource);

		GLuint tcShader = LoadShader(GL_TESS_CONTROL_SHADER, tcs);
		GLuint teShader = LoadShader(GL_TESS_EVALUATION_SHADER, tes);

		GLuint fragment_shader = LoadShader(GL_FRAGMENT_SHADER, _fragmentShaderSource);

		if ((!vertex_shader) || (!fragment_shader) || (!tcShader) || (!teShader))
			return;

		GLuint program = glCreateProgram();

		glAttachShader(program, vertex_shader);
		glAttachShader(program, tcShader);
		glAttachShader(program, teShader);
		glAttachShader(program, fragment_shader);

		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(tcShader);
		glDeleteShader(teShader);
		glDeleteShader(fragment_shader);

		_program = program;

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void Render() override
	{
		static const GLfloat red[] = { 1.0f,0.6f,0.0f,0.5f };
		glClearBufferfv(GL_COLOR, 0, red);

		glUseProgram(_program);

		double t = glfwGetTime();
		GLfloat attrib[] = { (float)sin(t) * 0.5f,(float)cos(t) * 0.5f,0.0f,0.0f };

		glVertexAttrib4fv(0, attrib);
		glVertexAttrib4fv(1, attrib);

		glDrawArrays(GL_PATCHES, 0, 3);
	}

	void EndUp() override
	{
		glDeleteProgram(_program);
	}
};


//================================================(Geometry Shader)===========================================================
class GeometryShader :public OpenGLSampleBase
{
protected:
	void StartUp() override
	{
		_vertexShaderSource =
			"#version 430 core\n"
			"layout(location=0) in vec4 offset;\n"
			"layout(location=1) in vec4 color;\n"
			"out VS_OUT\n"
			"{\n"
			"vec4 color;\n"
			"} vs_out;\n"
			"const vec4 verts[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),vec4(-0.25, -0.25, 0.5, 1.0),vec4( 0.25, 0.25, 0.5, 1.0));\n"
			"void main()\n"
			"{\n"
			//"gl_Position = verts[gl_VertexID] + offset;\n"
			"gl_Position = verts[gl_VertexID];\n"
			"vs_out.color = color;\n"
			"}\n";

		GLchar *tcs =
			"#version 430 core\n"
			"layout(vertices=3) out;\n"
			"void main()\n"
			"{\n"
			"if(gl_InvocationID == 0)\n"
			"{\n"
			"gl_TessLevelInner[0] = 5.0;\n"
			"gl_TessLevelOuter[0] = 5.0;\n"
			"gl_TessLevelOuter[1] = 5.0;\n"
			"gl_TessLevelOuter[2] = 5.0;\n"
			"}\n"
			"gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;\n"
			"}\n";

		GLchar *tes =
			"#version 430 core\n"
			"layout(triangles,equal_spacing,cw) in;\n"
			"void main()\n"
			"{\n"
			"gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position + gl_TessCoord.y * gl_in[1].gl_Position + gl_TessCoord.z * gl_in[2].gl_Position);\n"
			"}\n";

		GLchar *geometryShaderSource =
			"#version 430 core\n"
			"layout(triangles) in;\n"
			"layout(points,max_vertices = 3) out;\n"
			"void main()\n"
			"{\n"
				"for(int i = 0; i < gl_in.length(); i++)\n"
				"{\n"
					"gl_Position = gl_in[i].gl_Position;\n"
					"EmitVertex();\n"
				"}\n"
			"}\n";

		_fragmentShaderSource =
			"#version 430 core\n"
			"in VS_OUT\n"
			"{\n"
			"vec4 color;\n"
			"} fs_in;\n"
			"out vec4 color;\n"
			"void main()\n"
			"{\n"
			"color = fs_in.color;\n"
			"}\n";

		GLuint vertex_shader = LoadShader(GL_VERTEX_SHADER, _vertexShaderSource);

		GLuint tcShader = LoadShader(GL_TESS_CONTROL_SHADER, tcs);
		GLuint teShader = LoadShader(GL_TESS_EVALUATION_SHADER, tes);

		GLuint geometryShader = LoadShader(GL_GEOMETRY_SHADER, geometryShaderSource);

		GLuint fragment_shader = LoadShader(GL_FRAGMENT_SHADER, _fragmentShaderSource);

		if ((!vertex_shader) || (!fragment_shader) || (!tcShader) || (!teShader) || (!geometryShader))
			return;

		GLuint program = glCreateProgram();

		glAttachShader(program, vertex_shader);
		glAttachShader(program, tcShader);
		glAttachShader(program, teShader);
		glAttachShader(program, geometryShader);
		glAttachShader(program, fragment_shader);

		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(tcShader);
		glDeleteShader(teShader);
		glDeleteShader(geometryShader);
		glDeleteShader(fragment_shader);

		_program = program;

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glPointSize(5);
	}

	void Render() override
	{
		static const GLfloat red[] = { 1.0f,0.6f,0.0f,0.5f };
		glClearBufferfv(GL_COLOR, 0, red);

		glUseProgram(_program);

		double t = glfwGetTime();
		GLfloat attrib[] = { (float)sin(t) * 0.5f,(float)cos(t) * 0.5f,0.0f,0.0f };

		glVertexAttrib4fv(0, attrib);
		glVertexAttrib4fv(1, attrib);

		glDrawArrays(GL_PATCHES, 0, 3);
	}

	void EndUp() override
	{
		glDeleteProgram(_program);
	}
};



//================================================(Fragment Shader)===========================================================
class FragmentShaderSample :public OpenGLSampleBase
{
protected:
	void StartUp() override
	{
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
			"color = vec4(sin(gl_FragCoord.x * 0.25) * 0.5 + 0.5,cos(gl_FragCoord.x * 0.25) * 0.5 + 0.5,sin(gl_FragCoord.x * 0.15) * cos(gl_FragCoord.y * 0.15), 1.0);\n"
			"}\n";

		_program = CompileShaders(_vertexShaderSource, _fragmentShaderSource);
	}

	void Render() override
	{
		static const GLfloat red[] = { 1.0f,0.6f,0.0f,0.5f };
		glClearBufferfv(GL_COLOR, 0, red);

		glUseProgram(_program);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void EndUp() override
	{
		glDeleteProgram(_program);
	}
};




//================================================(Fragment Interpolates)===========================================================
class FragmentInterpolatesSample :public OpenGLSampleBase
{
protected:
	void StartUp() override
	{
		_vertexShaderSource =
			"#version 430 core\n"
			"const vec4 verts[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),vec4(-0.25, -0.25, 0.5, 1.0),vec4( 0.25, 0.25, 0.5, 1.0));\n"
			"const vec4 colors[3] = vec4[3](vec4(1.0, 0.0, 0.0, 1.0),vec4(0.0,1.0, 0.0, 1.0),vec4(0.0, 0.0, 1.0, 1.0));\n"
			"out vec4 vs_color;\n"
			"void main()\n"
			"{\n"
			"gl_Position = verts[gl_VertexID];\n"
			"vs_color = colors[gl_VertexID];\n"
			"}\n";

		_fragmentShaderSource =
			"#version 430 core\n"
			"in vec4 vs_color;\n"
			"out vec4 color;\n"
			"void main()\n"
			"{\n"
			"color = vs_color;\n"
			"}\n";

		_program = CompileShaders(_vertexShaderSource, _fragmentShaderSource);
	}

	void Render() override
	{
		static const GLfloat red[] = { 1.0f,0.6f,0.0f,0.5f };
		glClearBufferfv(GL_COLOR, 0, red);

		glUseProgram(_program);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void EndUp() override
	{
		glDeleteProgram(_program);
	}
};

//DECLARE_MAIN(PipeLine)
//DECLARE_MAIN(Tessellation)
//DECLARE_MAIN(GeometryShader)
//DECLARE_MAIN(FragmentShaderSample)
DECLARE_MAIN(FragmentInterpolatesSample)