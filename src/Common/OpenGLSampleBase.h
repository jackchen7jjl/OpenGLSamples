#include "glew.h"
#include "GLFW\glfw3.h"

class OpenGLSampleBase
{
public:
	~OpenGLSampleBase() = default;

	bool Init();
	void Run();

	GLuint OpenGLSampleBase::LoadShader(GLenum type, const char *shaderSrc);
	GLuint OpenGLSampleBase::CompileShaders(const char *vertextShader, const char *fragmentShader);

protected:
	virtual void StartUp() = 0;
	virtual void Render() = 0;
	virtual void EndUp() = 0;

	GLFWwindow *_window;
	GLchar* _vertexShaderSource;
	GLchar* _fragmentShaderSource;

	GLuint _program;
	GLuint _vertexArrayObject;

	GLuint _vertexBuffer;
	GLuint _indexBuffer;

private:
	static void ErrorCallback(int error, const char* description);
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};


#define DECLARE_MAIN(OpenGLSample)\
OpenGLSampleBase *sample = 0;\
int main()\
{\
	sample = new OpenGLSample(); \
	if(!(sample->Init()))\
		return 1;\
	sample->Run();\
	delete sample;\
	return 0;\
}