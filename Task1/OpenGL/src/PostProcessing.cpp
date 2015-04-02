#include "PostProcessing.h"

#include "Camera.h"

#include <gl_core_4_4.h>
#include <glm/glm.hpp>

#include <Gizmos.h>

PostProcessing::PostProcessing()
{
	StartUp();
}

PostProcessing::~PostProcessing()
{

}

void PostProcessing::StartUp()
{
	// setup framebuffer
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glGenTextures(1, &m_fboTexture);
	glBindTexture(GL_TEXTURE_2D, m_fboTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 1280, 720);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		m_fboTexture, 0);
	glGenRenderbuffers(1, &m_fboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_fboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
		1280, 720);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, m_fboDepth);
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// fullscreen quad
	glm::vec2 halfTexel = 1.0f / glm::vec2(1280, 720) * 0.5f;
	float vertexData[] = {
		-1, -1, 0, 1, halfTexel.x, halfTexel.y,
		1, 1, 0, 1, 1 - halfTexel.x, 1 - halfTexel.y,
		-1, 1, 0, 1, halfTexel.x, 1 - halfTexel.y,
		-1, -1, 0, 1, halfTexel.x, halfTexel.y,
		1, -1, 0, 1, 1 - halfTexel.x, halfTexel.y,
		1, 1, 0, 1, 1 - halfTexel.x, 1 - halfTexel.y,
	};
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6,
		vertexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
		sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		sizeof(float) * 6, ((char*)0) + 16);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	ShaderProg();
}

void PostProcessing::ShaderProg()
{
	const char* vsSource = "#version 410\n								\
							layout (location = 0) in vec4 position;		\
							layout (location = 1) in vec2 texCoord;		\
							out vec2 fTexCoord;							\
							void main()									\
							{											\
								gl_Position = position;					\
								fTexCoord = texCoord;					\
							}";

	const char* fsSource = "#version 410\n									\
							in vec2 fTexCoord;								\
							out vec4 FragColour;							\
							uniform sampler2D target;						\
							// just sample the target and return the colour	\
							vec4 Simple() {									\
							return texture(target, fTexCoord);\
							}\
							vec4 BoxBlur() {\
								vec2 texel = 1.0f / textureSize(target, 0).xy;\
								// 9-tap box kernel\
								vec4 colour = texture(target, fTexCoord);\
								colour += texture(target, fTexCoord + vec2(-texel.x, texel.y));\
								colour += texture(target, fTexCoord + vec2(-texel.x, 0));\
								colour += texture(target, fTexCoord + vec2(-texel.x, -texel.y));\
								colour += texture(target, fTexCoord + vec2(0, texel.y));\
								colour += texture(target, fTexCoord + vec2(0, -texel.y));\
								colour += texture(target, fTexCoord + vec2(texel.x, texel.y));\
								colour += texture(target, fTexCoord + vec2(texel.x, 0));\
								colour += texture(target, fTexCoord + vec2(texel.x, -texel.y));\
								return colour / 9;\
							}\
							void main() \
							{\
								FragColour = BoxBlur();\
							}";
}

void PostProcessing::Draw(Camera* _pCamera)
{
	// bind our target
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0, 0, 1280, 720);
	// clear the target
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw our 3D scene
	// gizmos for now
	Gizmos::draw(_pCamera->GetProjectionView());
	// bind the back-buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1280, 720);
	// just clear the back-buffer depth as
	// each pixel will be filled
	glClear(GL_DEPTH_BUFFER_BIT);
	// draw out full-screen quad
	glUseProgram(m_program);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_fboTexture);
	int loc = glGetUniformLocation(m_program, "target");
	glUniform1i(loc, 0);
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}