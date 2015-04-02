//-----------------------------------------------------------------------------
//Author: Tommas Solarino
//Desctiption:  loading textures in to OpenGL and adding them to Fragment Shaders.
//-----------------------------------------------------------------------------

#ifndef TUTORIAL4
#define TUTORIAL4
class Camera;
class Tutorial4 
{
public:
	Tutorial4 (const char *fileName);
	~Tutorial4 ();

	void Update(float _dt);
	void Draw(Camera* _pCamera);

	void StartUp();

private:
	const char *m_fileName;

	unsigned int m_texture;
	unsigned int m_program;

	unsigned int m_vao; //Vertex Array Object
	unsigned int m_vbo;	//Vertex Buffer Object
	unsigned int m_ibo;	//Index Buffer Object
};
#endif