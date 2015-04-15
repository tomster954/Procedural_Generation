//-----------------------------------------------------------------------------
//Author: Tommas Solarino
//Description: GUI that will control different things in the world
//-----------------------------------------------------------------------------

#include "AntTweakBar.h"

#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#ifndef GUI_BAR
#define GUI_BAR

struct GLFWwindow;

class GUI_Bar
{
public:
	GUI_Bar(GLFWwindow *_pWindow);
	~GUI_Bar();

	void StartUp(GLFWwindow *_pWindow);
	void Draw();


	glm::vec3 GetLightDir(){ return -m_light; };
	glm::vec3 GetLightCol(){ return m_lightColour; }

	float GetSpecPower(){ return m_specPower; }

	float GetCameraSpeed(){ return m_cameraSpeed; }

	glm::vec3 GetAmbientColour(){ return m_ambientColour; }

	//Particle getters
	//------------------------------------
	float GetLifeSpanMin(){ return m_lifespanMin; }
	float GetLifeSpanMax(){ return m_lifespanMax; }

	float GetSpeedMin(){ return m_SpeedMin; }
	float GetSpeedMax(){ return m_SpeedMax; }

	float GetStartSize(){ return m_startSize; }
	float GetEndSize(){ return m_endSize; }

	unsigned int GetMaxParticles(){ return m_maxParticles; }

	glm::vec4 GetStartColour(){ return m_startColour; }
	glm::vec4 GetEndColour(){ return m_endColour; }
	//------------------------------------

	//Terrain getters
	//------------------------------------
	float GetTerrainSeed(){ return m_terrainSeed; }
	float GetAmplitude(){ return m_amplitude; }
	float GetPersistence(){ return m_persistence; }
	//------------------------------------

	static void OnMouseButton(GLFWwindow*, int b, int a, int m) 
	{
		TwEventMouseButtonGLFW(b, a);
	}
	static void OnMousePosition(GLFWwindow*, double x, double y) 
	{
		TwEventMousePosGLFW((int)x, (int)y);
	}
	static void OnMouseScroll(GLFWwindow*, double x, double y) 
	{
		TwEventMouseWheelGLFW((int)y);
	}
	static void OnKey(GLFWwindow*, int k, int s, int a, int m) 
	{
		TwEventKeyGLFW(k, a);
	}
	static void OnChar(GLFWwindow*, unsigned int c) 
	{
		TwEventCharGLFW(c, GLFW_PRESS);
	}
	static void OnWindowResize(GLFWwindow*, int w, int h) 
	{
		TwWindowSize(w, h);
		glViewport(0, 0, w, h);
	}

private:
	TwBar* m_bar;
	glm::vec4 m_clearColour;
	glm::vec3 m_light;
	glm::vec3 m_lightColour;

	float m_specPower;
	float m_cameraSpeed;
	void* clientdata;
	//Particles
	//------------------------------------
	unsigned int m_maxParticles;

	float m_lifespanMin;
	float m_lifespanMax;
	float m_SpeedMin;
	float m_SpeedMax;
	float m_startSize;
	float m_endSize;
	
	glm::vec3 m_ambientColour;

	float m_terrainSeed;
	float m_amplitude;
	float m_persistence;

	glm::vec4 m_startColour;
	glm::vec4 m_endColour;
	//------------------------------------

};
#endif