//-----------------------------------------------------------------------------
//Author:Tommas Solarino
//Description: This is the mirror tutorial
//-----------------------------------------------------------------------------

#ifndef RENDER_TARGETS
#define RENDER_TARGETS

class Camera;
class ShaderLoader;

class RenderTargets
{
public:
	RenderTargets();
	~RenderTargets();

	void StartUp();

	void ShaderProg();

	void Draw(Camera* _pCamera);

	void SetAsActiveRenderTarget();
	void ClearAsActiveRenderTarget();

private:
	unsigned int m_fbo;
	unsigned int m_fboTexture;
	unsigned int m_fboDepth;

	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ibo;

	unsigned int m_program;
protected:

};


#endif