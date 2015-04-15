#include "GUI_Bar.h"

#include <GLFW/glfw3.h>

GUI_Bar::GUI_Bar(GLFWwindow *_pWindow)
{
	StartUp(_pWindow);
}

GUI_Bar::~GUI_Bar()
{
	TwDeleteAllBars();
	TwTerminate();
}


void GUI_Bar::StartUp(GLFWwindow *_pWindow)
{
	m_amplitude = 1.f;
	m_persistence = 0.2f;

	m_clearColour = glm::vec4(1, 1, 1, 1);
	m_light = glm::vec3(1, 1, 0);
	m_specPower = 1.0f;
	m_lightColour = glm::vec3(1, 1, 1);
	m_cameraSpeed = 20.0f;

	m_terrainSeed = 1234.5f;

	//Particles
	//------------------------------------
	m_lifespanMin	= 30.0f;
	m_lifespanMax	= 40.0f;
	m_SpeedMin		= 10.0f;
	m_SpeedMax		= 20.0f;
	m_startSize		= 0.5f;
	m_endSize		= 0.5f;
	
	m_startColour	= glm::vec4 (0.5f, 1, 1, 1);
	m_endColour		= glm::vec4 (0.5f, 1, 1, 1);

	m_ambientColour	= glm::vec3 (0.25, 0.25, 0.25);

	m_maxParticles = 10000;
	//------------------------------------

	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(1280, 720);

	glfwSetMouseButtonCallback(_pWindow, OnMouseButton);
	glfwSetCursorPosCallback(_pWindow, OnMousePosition);
	glfwSetScrollCallback(_pWindow, OnMouseScroll);
	glfwSetKeyCallback(_pWindow, OnKey);
	glfwSetCharCallback(_pWindow, OnChar);
	glfwSetWindowSizeCallback(_pWindow, OnWindowResize);

	m_bar = TwNewBar("my bar");

	TwAddButton(m_bar, "Run", NULL, NULL, " label='Press 'R' to regen terrain' ");
	
	//Group : Terrain
	//------------------------------------
		TwAddVarRW(m_bar, "Seed", TW_TYPE_FLOAT, &m_terrainSeed, "group=Terrain");

		TwAddVarRW(m_bar, "Amplitude", TW_TYPE_FLOAT, &m_amplitude, "group=Terrain");

		TwAddVarRW(m_bar, "Persistence", TW_TYPE_FLOAT, &m_persistence, "group=Terrain");
	//------------------------------------

	//Group : Scene
	//------------------------------------
		TwAddVarRW(m_bar, "light colour",
		TW_TYPE_COLOR3F, &m_lightColour[0], "group=Scene");

		TwAddVarRW(m_bar, "Ambient Light",
		TW_TYPE_COLOR3F, &m_ambientColour[0], "group=Scene");

		TwAddVarRW(m_bar, "light direction",
		TW_TYPE_DIR3F, &m_light[0], "group=Scene");

		TwAddVarRW(m_bar, "Specular Power",
		TW_TYPE_FLOAT, &m_specPower, "group=Scene");
	//------------------------------------

	TwAddVarRW(m_bar, "Camera Speed",
	TW_TYPE_FLOAT, &m_cameraSpeed, "group=Camera");

	//Particles
	//-------------------------------------------------------------------------
	//m_startColour
	TwAddVarRW(m_bar, "Start colour",
	TW_TYPE_COLOR4F, &m_startColour[0], "group=Particles");
	//m_endColour
	TwAddVarRW(m_bar, "End colour",
	TW_TYPE_COLOR4F, &m_endColour[0], "group=Particles");
	
	//Start size
	TwAddVarRW(m_bar, "Start size",
	TW_TYPE_FLOAT, &m_startSize, "group=Particles");
	//End size
	TwAddVarRW(m_bar, "End size",
	TW_TYPE_FLOAT, &m_endSize, "group=Particles");
	
	//Velocity min
	TwAddVarRW(m_bar, "Speed",
	TW_TYPE_FLOAT, &m_SpeedMin, "group=Particles");
	////Velocity max
	//TwAddVarRW(m_bar, "Speed max",
	//TW_TYPE_FLOAT, &m_SpeedMax, "group=Particles");
	
	//Lifespan min
	TwAddVarRW(m_bar, "Lifespan min",
	TW_TYPE_FLOAT, &m_lifespanMin, "group=Particles");
	//Lifespan max
	TwAddVarRW(m_bar, "Lifespan max",
	TW_TYPE_FLOAT, &m_lifespanMax, "group=Particles");
	
	//Max particles
	TwAddVarRW(m_bar, "Max particles",
	TW_TYPE_UINT32 , &m_maxParticles, "group=Particles");

}

void GUI_Bar::Draw()
{
	TwDraw();
	glClearColor(m_clearColour.x, m_clearColour.y, m_clearColour.z, 1);
}