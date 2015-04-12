//-----------------------------------------------------------------------------
//Author: Tommas Solarino
//Description: loads FBX files
//-----------------------------------------------------------------------------

#ifndef FBX_LOADER
#define FBX_LOADER

#include <glm/glm.hpp>

class Camera;
class FBXFile;
class FBXMeshNode;

class FBX_Loader
{
public:
	FBX_Loader(const char *_FBX_FileName);
	~FBX_Loader();

	void StartUp(const char* _FBX_FileName);

	void Draw(Camera* pCamera); 
	void Update(float _dt);

	void createOpenGLBuffers(FBXFile* fbx);
	void cleanupOpenGLBuffers(FBXFile* fbx);

	void SetLightColour(glm::vec3 _colour) { m_lightCol = _colour; }
	void SetLightDir(glm::vec3 _lightDir) { m_lightDir = _lightDir; }
	void SetSecPower(float _specPower) { m_specPow = _specPower; }

	void SetPosition(glm::vec4 _pos){ m_position = _pos; }
private:
	FBXFile* m_fbx;
	FBXMeshNode* m_mesh;

	glm::vec3 m_lightDir;
	glm::vec3 m_lightCol;
	
	glm::vec4 m_position;

	float m_specPow;
	float m_timer;

	unsigned int m_programID;

	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	unsigned int m_texture;

	unsigned int m_vertCount;
	unsigned int m_indexCount;
};
#endif