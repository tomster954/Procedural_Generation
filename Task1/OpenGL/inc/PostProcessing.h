//-----------------------------------------------------------------------------
//Author: Tommas Solarino	
//Description: Post Processing effects tutorial
//-----------------------------------------------------------------------------

#ifndef POST_PROCESSING
#define POST_PROCESSING
class Camera;

class PostProcessing
{
public:
	PostProcessing();
	~PostProcessing();

	void StartUp();

	void ShaderProg();

	void Draw(Camera* _pCamera);

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

