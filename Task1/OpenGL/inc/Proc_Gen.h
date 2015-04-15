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
	glm::vec3 normal;
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

	void SetSeed(float _seed) { m_seed = _seed; }
	void SetSpecularPower(float _specPow) { m_specPow = _specPow; }
	void SetLightDirection( glm::vec3 _lightDir ) { m_lightDir = -_lightDir; }
	void SetLightColour( glm::vec3 _lightCol ) { m_lightCol = _lightCol; }
	void SetAmbientColour( glm:: vec3 _ambientColour) {m_ambientColour = _ambientColour; }

	void SetPersistence( float _persistence ) { m_persistence = _persistence; }
	void SetAmplitude( float _amplitude ) { m_amplitude = _amplitude; }

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

	glm::vec3 m_ambientColour;

	float m_seed;

	float m_specPow;

	float m_amplitudeCurr;
	float m_persistenceCurr;

	float m_amplitude;
	float m_persistence;

	glm::vec3 m_lightDir;
	glm::vec3 m_lightCol;

	GLFWwindow *m_pWindow;
};


#endif