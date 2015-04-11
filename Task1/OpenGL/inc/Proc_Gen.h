//-----------------------------------------------------------------------------
//Author: Tommas Solarino
//Description: Proceduraly generates a terrain
//-----------------------------------------------------------------------------

#ifndef PROC_GEN
#define PROC_GEN

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Camera;
struct GLFWwindow;

struct ProcGenVertex 
{
	glm::vec4 position;
	glm::vec4 colour;
	glm::vec2 texcoord;
};

class ProcGen
{
public:
	ProcGen(unsigned int _rows, unsigned int _cols, GLFWwindow *_pWindow );
	~ProcGen();

	void StartUp();
	void GenerateGrid( unsigned int rows, unsigned int cols );
	void CreateShaders();

	void GenerateNoise();

	void Update(float _dt);
	void Draw(Camera* _pCamera);

	void LoadTextures();


private:
	// our vertex and index buffers
	unsigned int m_rows;
	unsigned int m_cols;
	
	unsigned int m_perlin_texture;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	
	unsigned int m_programID;

	unsigned int m_rockTexture;
	unsigned int m_dirtGrassTexture;
	unsigned int m_waterTexture;
	unsigned int m_snowTexture;

	GLFWwindow *m_pWindow;
};


#endif