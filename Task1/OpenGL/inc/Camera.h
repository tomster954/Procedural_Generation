//-----------------------------------------------------------------------------
//Author: Tommas Solarino	
//Description: This is the camera class
//-----------------------------------------------------------------------------

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

struct GLFWwindow;

class Camera
{
public:
	//Constructor
	Camera(GLFWwindow *_pWindow);
	//Destructor
	~Camera();

	void Update(float dt);
	void Draw();

	glm::mat4 GetProjectionView()
	{
		return m_projection * m_view;
	}

	void SetPosition(glm::vec3 pos)
	{
		m_worldTransform[3].xyz = pos;
		m_view = glm::inverse(m_worldTransform);
	}

	glm::vec3 GetPosition()
	{
		return m_worldTransform[3].xyz;

	}

	glm::mat4 GetTransform() 
	{
		return m_worldTransform; 
	}

	void SetCameraSpeed(float _camerSpeed) { m_cameraSpeed = _camerSpeed; }

protected:

private:
	glm::mat4 m_view;
	glm::mat4 m_projection;

	float m_cameraSpeed;

	double m_prevMouseXpos, m_prevMouseYpos;
	double m_currMouseXpos, m_currMouseYpos;

	GLFWwindow *m_pWindow;

	float m_cameraX;
	float m_cameraY;
	float m_cameraZ;

	glm::mat4 m_worldTransform;
};

#endif
