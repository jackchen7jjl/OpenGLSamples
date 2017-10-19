#include "OpenGLSampleBase.h"

static const GLfloat vVertices[] =
{
	-0.3, -0.3, 0,1.0,     // - 1st vertex x,y,z
	0, 0.3, 0,1.0,         // - 2nd vertex x,y,z 
	0.3, -0.3, 0,1.0    // - 3rd vertex x,y,z
};

static const GLfloat instanceColors[] =
{
	1.0, 0.0, 0.0,     // - 1st instance r,g,b
	0.0, 1.0, 0.0,  // - 2nd instance r,g,b
	1.0, 1.0, 1.0,    // - 3rd instance r,g,b
	0.7, 0.0, 1.0   // - 4th instance r,g,b
};

static const GLfloat instancePositions[] =
{
	-0.3, -0.3, 0.0,0.0,     // - 1st instance x,y,z
	0.3, 0.3, 0.0,0.0,         // - 2nd instance x,y,z
	-0.3, 0.3, 0.0,0.0,        // - 3rd instance x,y,z
	0.3, -0.3, 0.0,0.0      // - 4th instance x,y,z
};

static const GLushort indices[] = { 0, 1, 2 };

class Instancing :public OpenGLSampleBase
{
protected:
	void StartUp()
	{
		_vertexShaderSource =
			"attribute vec4 aPosition;    \n"
			"attribute vec4 aInstanceColor;   \n"
			"attribute vec4 aInstancePosition;   \n"
			"varying vec4 vColor;     \n"
			"void main()                  \n"
			"{                            \n"
			"   gl_Position = aPosition + aInstancePosition;\n"
			"   vColor = aInstanceColor;  \n"
			"}                            \n";

		_fragmentShaderSource =
			"varying vec4 vColor;							\n"
			"void main()										\n"
			"{													\n"
			"  gl_FragColor = vColor;\n"
			"}\n";

		_program = CompileShaders(_vertexShaderSource, _fragmentShaderSource);

		_positionLoc = glGetAttribLocation(_program, "aPosition");
		_instancePositionLoc = glGetAttribLocation(_program, "aInstancePosition");
		_instanceColorLoc = glGetAttribLocation(_program, "aInstanceColor");

		glGenBuffers(1, &_vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices) + sizeof(instanceColors) + sizeof(instancePositions), NULL, GL_STATIC_DRAW);

		GLuint offset = 0;
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(vVertices), vVertices);
		offset += sizeof(vVertices);
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(instanceColors), instanceColors);
		offset += sizeof(instanceColors);
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(instancePositions), instancePositions);

		glGenBuffers(1, &_indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	}

	void Render()
	{
		glClearColor(1.0, 0.6, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(_program);

		glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);

		glVertexAttribPointer(_positionLoc, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glVertexAttribPointer(_instanceColorLoc, 4, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(vVertices));
		glVertexAttribPointer(_instancePositionLoc, 4, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(vVertices) + sizeof(instanceColors)));

		glEnableVertexAttribArray(_positionLoc);
		glEnableVertexAttribArray(_instanceColorLoc);
		glEnableVertexAttribArray(_instancePositionLoc);

		glVertexAttribDivisor(_instanceColorLoc, 1);
		glVertexAttribDivisor(_instancePositionLoc, 1);

		glDepthRangef(-1.0, 1.0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);

		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0, 4);

		//glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0, 4);
	}

	void EndUp()
	{
		glDeleteProgram(_program);
	}

private:
	GLint  _positionLoc;
	GLint  _instanceColorLoc;
	GLint  _instancePositionLoc;
};

DECLARE_MAIN(Instancing);