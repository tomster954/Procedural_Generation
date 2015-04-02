//-----------------------------------------------------------------------------
//Author: Tommas Solarino
//Descriptio: Will manage the different tutorials
//-----------------------------------------------------------------------------

#ifndef TUTORIAL_MANAGER
#define TUTORIAL_MANAGER

//Includes


#include <glm/glm.hpp>
#include <glm/ext.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;

struct GLFWwindow;

class Tutorial1;
class Tutorial2;
class Geometry;

class Camera;
class TutorialManager
{
public:
	//Constructor
	TutorialManager( int _tutNumber, vec4 _backgroundColour);
	//Destructor
	~TutorialManager();

	void RunGame();

	void Update();
	void Draw();

protected:

private:
	GLFWwindow *m_pWindow;

	Tutorial1	*m_tut1;
	Tutorial2	*m_tut2;
	Geometry	*m_tut3;

	Camera *m_camera;

	int m_tutNumber;

	bool m_gameOver;

	float	m_lastTime;
	float	m_currentTime;
	float	m_deltaTime;
};

#endif
