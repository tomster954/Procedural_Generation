#include "TutorialManager.h"
#include "Tutorial1.h"
#include "Tutorial2.h"
#include "Geometry.h"

#include "Camera.h"

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;

//Constructor
TutorialManager::TutorialManager( int _tutNumber, vec4 _backgroundColour)
{
	m_tutNumber = _tutNumber;

	if (glfwInit() == false)
		return;

	m_pWindow = glfwCreateWindow(1280, 720, "Computer Graphics", nullptr, nullptr);

	m_camera = new Camera(m_pWindow);

	if (m_pWindow == nullptr) 
	{
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_pWindow);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) 
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
		return;
	}
	//Init different tutorials
	//------------------------------------
	m_tut1 = new Tutorial1();
	m_tut2 = new Tutorial2();
	m_tut3 = new Geometry(m_pWindow);
	//------------------------------------

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	glClearColor(_backgroundColour.r, _backgroundColour.g, _backgroundColour.b, _backgroundColour.a);
	glEnable(GL_DEPTH_TEST); // enables the depth buffer

	m_currentTime	= 0.0f;
	m_deltaTime		= 0.0f;
	m_lastTime		= 0.0f;

}
//Destructor
TutorialManager::~TutorialManager()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void TutorialManager::RunGame()
{
	while (glfwWindowShouldClose(m_pWindow) == false &&
	glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS) 
	{
		Update();
		Draw();

		glfwSwapBuffers(m_pWindow);
		glfwPollEvents();
	}
}

void TutorialManager::Update()
{
	//m_lastTime		= m_currentTime;
	//m_currentTime	= glfwGetTime();
	//m_deltaTime		= m_currentTime - m_lastTime;

	m_currentTime	= (float)glfwGetTime();
	m_deltaTime		= m_currentTime - m_lastTime; // prev of last frame
	m_lastTime		= m_currentTime;

	m_camera->Update(m_deltaTime);


	switch (m_tutNumber)
	{
		case 1: 
		{
			m_tut1->Update(m_deltaTime);
		}break;
		case 2: 
		{
			m_tut2->Update(m_deltaTime);
		}break;
		case 3: 
		{
			m_tut3->Update(m_deltaTime);
		}break;
		default: 
		{
			std::cout << "Wrong Tutorial Number\n";
		}break;
	}
	
}

void TutorialManager::Draw()
{
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	m_camera->Draw();

	switch (m_tutNumber)
	{
		case 1: 
		{
			m_tut1->Draw();	
		}break;
		case 2: 
		{
			m_tut2->Draw(m_camera);	
		}break;
		case 3: 
		{
			m_tut3->Draw(m_camera);	
		}break;
		default: 
		{
			std::cout << "Wrong Tutorial Number \n";
		}break;
	}
}