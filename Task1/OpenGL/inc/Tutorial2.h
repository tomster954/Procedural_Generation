//-----------------------------------------------------------------------------
//Author: Tommas Solarino
//Description: Creating Vertex Buffers, Index Buffers and Vertex Array objects.
//-----------------------------------------------------------------------------

#ifndef TUTORIAL2
#define TUTORIAL2

class Camera;

#include <glm/glm.hpp>
#include <glm/ext.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;

class Tutorial2
{

public:
	//contructor
	Tutorial2();

	//destructor
	~Tutorial2();

	void Update( float _dt);
	void Draw(Camera* pCamera);

	void StartUp();

	void GenerateGrid ( unsigned int rows, unsigned int cols);

private:

	// our vertex and index buffers
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;

	unsigned int m_programID;

};

struct Vertex 
{
	vec4 position;
	vec4 colour;
 };


#endif