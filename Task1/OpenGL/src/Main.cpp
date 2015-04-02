#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "TutorialManager.h"

#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;


int main() 
{
	TutorialManager *pTut = new TutorialManager(3, vec4(0.25f, 0.25f, 0.25f, 1.0f));
	pTut->RunGame();
	delete pTut;

	return 0;
}

// Tutorial 1: open gl tutorial with planets
// Tutorial 2: Creating Vertex Buffers, Index Buffers and Vertex Array objects.
// Tutorial 3: Geometry including obj loading and loading textures in to OpenGL and adding them to Fragment Shaders.