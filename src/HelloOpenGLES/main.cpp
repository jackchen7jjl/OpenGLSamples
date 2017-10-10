#if defined(EMSCRIPTEN)
#include "GLES2\gl2.h"
#include <emscripten\emscripten.h>
#else
#include "glew.h"
#endif
#include "GLFW\glfw3.h"

#include <math.h>
#include <iostream>
#include <fstream>
using namespace std;

GLFWwindow* window;

// Handle to a program object
static GLuint program;

// Texture handle
static GLuint textureId;

// Attribute locations Sampler location mvp location
static GLint  positionLoc;
static GLint  texCoordLoc;
static GLint samplerLoc;
static GLint mvpLoc;

static GLuint vertexBuffer, indexBuffer;

static float mvp[16] = { 0 };
static float rotZ;

static const GLfloat vVertices[] =
{
	-0.5,  0.5, -1.0, 1.0, // Position 0
	0.0,  0.0,       // TexCoord 0
	-0.5, -0.5, -1.0, 1.0,  // Position 1
	0.0,  1.0,       // TexCoord 1
	0.5, -0.5, -1.0, 1.0,  // Position 2
	1.0,  1.0,       // TexCoord 2
	0.5,  0.5, -1.0, 1.0,  // Position 3
	1.0,  0.0,        // TexCoord 3
};

static const GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

static const char* vertex_shader_source =
"attribute vec4 a_position;    \n"
"attribute vec2 a_texCoord;   \n"
"varying vec2 v_texCoord;     \n"
"uniform mat4 c_mvp;		  \n"
"void main()                  \n"
"{                            \n"
"   gl_Position = c_mvp * a_position;\n"
"   v_texCoord = a_texCoord;  \n"
"}                            \n";

static const char* fragment_shader_source =
"precision mediump float;							\n"
"varying vec2 v_texCoord;							\n"
"uniform sampler2D s_texture;						\n"
"void main()										\n"
"{													\n"
"  gl_FragColor = texture2D(s_texture, v_texCoord );\n"
"}													\n";

char* readRGBFile(const char* filename)
{
	ifstream vsIutput(filename, ios::in | ios::binary);
	if (vsIutput.is_open())
	{
		vsIutput.seekg(0, std::ios_base::end);
		unsigned int len = vsIutput.tellg();
		vsIutput.seekg(0, std::ios_base::beg);
		char* bytes = (char*)malloc(len);
		vsIutput.read(bytes, len);
		vsIutput.close();
		return bytes;
	}
	return nullptr;
}

GLuint CreateSimpleTexture2D()
{
	// Texture object handle
	GLuint textureId;

	GLubyte *pixels = (GLubyte *)readRGBFile("grass.bmp");

	// Use tightly packed data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Generate a texture object
	glGenTextures(1, &textureId);

	// Bind the texture object
	glBindTexture(GL_TEXTURE_2D, textureId);

	// Load the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	// Set the filtering mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return textureId;

}

GLuint LoadShader(GLenum type, const char *shaderSrc)
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

			cerr << "compilete shader error: " << infoLog << endl;
		}
		return 0;
	}

	return shader;
}

GLuint compile_shaders(void)
{
	GLuint vertex_shader = LoadShader(GL_VERTEX_SHADER, vertex_shader_source);

	GLuint fragment_shader = LoadShader(GL_FRAGMENT_SHADER, fragment_shader_source);

	if ((!vertex_shader) || (!fragment_shader))
		return 0;

	GLuint program = glCreateProgram();

	glAttachShader(program, vertex_shader);

	glAttachShader(program, fragment_shader);

	glLinkProgram(program);

	glDeleteShader(vertex_shader);

	glDeleteShader(fragment_shader);

	return program;
}

static void error_callback(int error, const char* description)
{
	cout << "err" << description << endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
}

void mainLoop()
{
	float ratio;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float)height;

	glViewport(0, 0, width, height);

	glClearColor(1.0, 0.6, 0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glVertexAttribPointer(positionLoc, 4, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 24, (void*)16);

	glEnableVertexAttribArray(positionLoc);
	glEnableVertexAttribArray(texCoordLoc);

	// Bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glUniform1i(samplerLoc, 0);


	rotZ += 0.01;

	mvp[0] = cos(rotZ);
	mvp[1] = sin(rotZ);
	mvp[4] = -sin(rotZ);
	mvp[5] = cos(rotZ);

	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, (const GLfloat*)mvp);

	glDepthRangef(-1.0, 1.0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

int main()
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(1024, 768, "Hello GLFW", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return 1;
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

#ifndef EMSCRIPTEN
	GLenum err = glewInit();
#endif // !EMSCRIPTEN

	program = compile_shaders();

	if (!program)
		return 0;

	positionLoc = glGetAttribLocation(program, "a_position");
	texCoordLoc = glGetAttribLocation(program, "a_texCoord");

	samplerLoc = glGetUniformLocation(program, "s_texture");

	mvpLoc = glGetUniformLocation(program, "c_mvp");
	mvp[15] = mvp[10] = 1;

	textureId = CreateSimpleTexture2D();

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	

#ifdef EMSCRIPTEN
	emscripten_set_main_loop(mainLoop, 0, 1);
#else
	while (!glfwWindowShouldClose(window))
	{
		mainLoop();
	}
#endif // EMSCRIPTEN

	glDeleteProgram(program);
	glfwTerminate();

	return 0;
}

