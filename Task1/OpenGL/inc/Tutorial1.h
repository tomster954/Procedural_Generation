//-----------------------------------------------------------------------------
//Author: Tommas Solarino
//Descriptio: OpenGL Tutorial planets and stuff
//-----------------------------------------------------------------------------

#ifndef TUTORIAL1
#define TUTORIAL1

#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;

class Tutorial1
{
public:
	//Constructor
	Tutorial1();
	//Destructor
	~Tutorial1();

	void Update(float dt);
	void Draw();

protected:

private:

	float m_rotation;
};
#endif