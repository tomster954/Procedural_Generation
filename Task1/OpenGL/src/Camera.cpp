#include "Camera.h"

#include <GLFW/glfw3.h>

#include <glm/ext.hpp>

#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;
//Constructor
Camera::Camera(GLFWwindow *_pWindow)
{
	m_pWindow = _pWindow;

	m_cameraSpeed = 200.0f;

	m_worldTransform[3] = glm::vec4(0, 1, 10, 1);

	m_view = glm::lookAt(m_worldTransform[3].xyz(), vec3(0), vec3(0,1,0));
	m_projection = glm::perspective(glm::pi<float>() * 0.25f, 16/9.f, 0.1f, 10000.f);

	m_currMouseXpos = 0;
}
//Destructor
Camera::~Camera()
{

}

void Camera::Update(float dt)
{
	glm::vec3 vRight = m_worldTransform[0].xyz();
	glm::vec3 vUp = m_worldTransform[1].xyz();
	glm::vec3 vForward = m_worldTransform[2].xyz();

	//IF key pressed
	//	m_cameraX += 10 * dt;
	if(glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_worldTransform[3] += m_cameraSpeed * dt * glm::vec4(vForward, 0);
	}

	if(glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_worldTransform[3] -= m_cameraSpeed * dt * glm::vec4(vForward, 0);
	}

	if(glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_worldTransform[3] -= m_cameraSpeed * dt * glm::vec4(vRight, 0);
	}
	
	if(glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_worldTransform[3] += m_cameraSpeed * dt * glm::vec4(vRight, 0);
	}
 
	double xpos, ypos;
	
	glfwGetCursorPos(m_pWindow, &xpos, &ypos);
	
	if (glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS && m_prevMouseXpos > 0 && m_prevMouseYpos > 0)
	{
		float dx = m_prevMouseXpos - xpos;
		float dy = m_prevMouseYpos - ypos;
	
		glm::mat4 xRot = glm::rotate(dx * 0.5f * dt,	glm::vec3(0, 1, 0));
		glm::mat4 yRot = glm::rotate(dy * 0.5f * dt,	glm::vec3(1, 0, 0));
	
		m_worldTransform = m_worldTransform * xRot * yRot;
	
		//m_view = glm::inverse(m_worldTransform);
	
	}
	
	glm::mat4 oldTrans = m_worldTransform;

	glm::mat4 trans;
	glm::vec3 worldUp = glm::vec3(0, 1, 0);

	//right
	trans[0] = glm::normalize( glm::vec4(glm::cross(worldUp, oldTrans[2].xyz()), 0));
	//up
	trans[1] = glm::normalize( glm::vec4(glm::cross(oldTrans[2].xyz(), trans[0].xyz()), 0));
	//forwar
	trans[2] = glm::normalize(oldTrans[2]);
	//position 
	trans[3] = oldTrans[3];

	m_view = glm::inverse(trans);

	m_prevMouseXpos = xpos;
	m_prevMouseYpos = ypos;

}

void Camera::Draw()
{
	Gizmos::draw(m_projection * m_view);
}