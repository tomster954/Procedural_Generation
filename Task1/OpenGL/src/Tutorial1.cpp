#include "Tutorial1.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;

Tutorial1::Tutorial1()
{
	Gizmos::create();
	
	m_rotation = 0.0f;
}
Tutorial1::~Tutorial1()
{
	Gizmos::destroy();
}

void Tutorial1::Update(float dt)
{
	// our game logic and update code goes here!
	// so does our render code!

	m_rotation += 0.009;

	Gizmos::clear();
	Gizmos::addTransform(glm::mat4(1));
	vec4 white	(1);
	vec4 green	(0,1,0,1);
	vec4 black	(0,0,0,1);
	vec4 blue	(0,0,1,1);

	for (int i = 0; i < 21; ++i) 
	{
		blue[0] += 1.0f/21.0f;

		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? green : blue);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? green : blue);
	}

	Gizmos::addSphere(vec3(0,0,0), 1, 20, 20, vec4(1,1,0,1));
	Gizmos::addSphere(vec3(4 * sin(m_rotation), 0, 3*cos(m_rotation) ), 0.5, 20, 20, vec4(1,0,0,1));

	Gizmos::addSphere(vec3(5 * sin(m_rotation), 0, 2*cos(m_rotation) ), 0.1, 10, 10, vec4(1,0,0,1));
}

void Tutorial1::Draw()
{
		
}