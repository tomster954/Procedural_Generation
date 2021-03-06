#include "Tutorial2.h"

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;

#include "Camera.h"

//contructor
Tutorial2::Tutorial2()
{
	StartUp();
	GenerateGrid(100, 100);
}

//destructor
Tutorial2::~Tutorial2()
{

}

void Tutorial2::StartUp()
{
	// create shaders
	const char* vsSource = "#version 410\n \ layout(location=0) in vec4 Position; \
							layout(location=1) in vec4 Colour; \
							out vec4 vColour; \
							uniform mat4 ProjectionView; \
							void main() { vColour = Colour; gl_Position = ProjectionView * Position; }";

	const char* fsSource = "#version 410\n \ in vec4 vColour; \	out vec4 FragColor; \ void main() { FragColor = vColour; }";

	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);
	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
	int infoLogLength = 0;
	glGetShaderiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	char* infoLog = new char[infoLogLength];
	glGetShaderInfoLog(m_programID, infoLogLength, 0, infoLog);
	printf("Error: Failed to link shader program!\n");
	printf("%s\n",infoLog);
	delete[] infoLog;
	}
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
}

void Tutorial2::GenerateGrid ( unsigned int rows, unsigned int cols)
{
	Vertex* aoVertices = new Vertex[ rows * cols ];
	for( unsigned int r = 0; r < rows; ++r )
	{
		for( unsigned int c = 0; c < cols; ++c)
		{
			aoVertices[ r * cols + c ].position = vec4((float)c, 0, (float)r, 1);
			// create some arbitrary colour based off something
			// that might not be related to tiling a texture

			vec3 colour = vec3( sinf( (c / (float)(cols - 1)) * (r / (float)(rows - 1))), sinf( (c / (float)(cols - 1)) * (r / (float)(rows - 1))), 0.25);

			aoVertices[ r * cols + c ].colour = vec4(colour, 1);
		}
	}

	// defining index count based off quad count (2 triangles per quad)
	unsigned int* auiIndices = new unsigned int[ (rows - 1) * (cols - 1) * 6 ];
	unsigned int index = 0;

	for ( unsigned int r = 0 ; r < (rows - 1) ; ++r ) 
	{
		for ( unsigned int c = 0 ; c < (cols - 1) ; ++c ) 
		{
			// triangle 1
			auiIndices[ index++ ] = r * cols + c;
			auiIndices[ index++ ] = (r + 1) * cols + c;
			auiIndices[ index++ ] = (r + 1) * cols + (c + 1);
			// triangle 2
			auiIndices[ index++ ] = r * cols + c;
			auiIndices[ index++ ] = (r + 1) * cols + (c + 1);
			auiIndices[ index++ ] = r * cols + (c + 1);
		}
	}


	//Add the following line to generate a VertexArrayObject
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray( m_VAO );

	// create and bind buffers to a vertex array object
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, (rows * cols) * sizeof(Vertex),
	 aoVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	 (void*)(sizeof(vec4)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (rows - 1) * (cols - 1) * 6 *
	sizeof(unsigned int), auiIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	glBindVertexArray(0);

	delete[] aoVertices;
}

void Tutorial2::Update(float _dt)
{

}

void Tutorial2::Draw(Camera* pCamera)
{
	Gizmos::addSphere(glm::vec3(0, 0, 0), 5.0f, 25, 25, glm::vec4(1, 0, 0, 0));


	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glUseProgram(m_programID);

	unsigned int projectionViewUniform = glGetUniformLocation(m_programID,"ProjectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(pCamera->GetProjectionView()));

	glBindVertexArray(m_VAO);
	unsigned int indexCount = 99 * 99 * 6;
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}
