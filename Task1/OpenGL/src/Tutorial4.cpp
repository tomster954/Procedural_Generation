
#include "Tutorial4.h"
#include "Camera.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

#include <stb-master/stb_image.h>

Tutorial4::Tutorial4 (const char *fileName)
{
	m_fileName = fileName;
	StartUp();
}

Tutorial4::~Tutorial4 ()
{

}

void Tutorial4::StartUp()
{
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	unsigned char* data = stbi_load(m_fileName,	&imageWidth, &imageHeight, &imageFormat, STBI_default);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	stbi_image_free(data);

	const char* vsSource = "#version 410\n \
	layout(location=0) in vec4 Position; \
	layout(location=1) in vec2 TexCoord; \
	out vec2 vTexCoord; \
	uniform mat4 ProjectionView; \
	void main() { \
	vTexCoord = TexCoord; \
	gl_Position= ProjectionView * Position;\
	}";
	const char* fsSource = "#version 410\n \
	in vec2 vTexCoord; \
	out vec4 FragColor; \
	uniform sampler2D diffuse; \
	void main() { \
	FragColor = texture(diffuse,vTexCoord);\
	}";
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);
	m_program = glCreateProgram();
	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragmentShader);
	glLinkProgram(m_program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertexData[] = {
	-5, 0, 5, 1, 0, 1,
	5, 0, 5, 1, 1, 1,
	5, 0, -5, 1, 1, 0,
	-5, 0, -5, 1, 0, 0,
	};
	unsigned int indexData[] = {
	0, 1, 2,
	0, 2, 3,
	};
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4,
	vertexData, GL_STATIC_DRAW);
	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6,
	indexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
	sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
	sizeof(float) * 6, ((char*)0)+16);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Tutorial4::Update(float _dt)
{

}

void Tutorial4::Draw(Camera* _pCamera)
{
	// use our texture program
	glUseProgram(m_program);
	// bind the camera
	int loc = glGetUniformLocation(m_program, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE,
	&(_pCamera->GetProjectionView()[0][0]));
	// set texture slot
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	// tell the shader where it is
	loc = glGetUniformLocation(m_program, "diffuse");
	glUniform1i(loc, 0);
	// draw
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

