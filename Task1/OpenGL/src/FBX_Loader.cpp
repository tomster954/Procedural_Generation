#include "FBX_Loader.h"
#include "camera.h"

#include "FBXFile.h"
#include <stb-master/stb_image.h>

#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>

#include "ShaderLoader.h"

using namespace std;

FBX_Loader::FBX_Loader(const char* _FBX_FileName)
{
	StartUp(_FBX_FileName);
	m_timer = 0.0f;
}

FBX_Loader::~FBX_Loader()
{
	cleanupOpenGLBuffers(m_fbx);
	glDeleteProgram(m_programID);
}

void FBX_Loader::StartUp(const char* _FBX_FileName)
{
	m_position = glm::vec4(0, 50, 0, 0);

	m_lightCol = glm::vec3(1, 1, 1);
	m_lightDir = glm::vec3(0, 1, 0);
	m_specPow = 5;

	m_fbx = new FBXFile();
	m_fbx->load(_FBX_FileName);
	m_fbx->initialiseOpenGLTextures();
	createOpenGLBuffers(m_fbx);

	ifstream inFileName(_FBX_FileName, ios::in);
	ifstream inTextName(_FBX_FileName, ios::in);
	if (!inFileName || !inTextName ) 
	{
		cout << "Could Not load fbx file \n";
	}

	unsigned int vertexShader = LoadShaderProgram(GL_VERTEX_SHADER,		"./shaderProg/FBX_Loader.vert");
	unsigned int fragmentShader = LoadShaderProgram(GL_FRAGMENT_SHADER,	"./shaderProg/FBX_Loader.frag");

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);
	int success = GL_FALSE;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) 
	{
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n",infoLog);
		delete[] infoLog;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void FBX_Loader::Draw(Camera* pCamera)
{
	//Gizmos::addSphere(glm::vec3(0, 0, 0), 5.0f, 100, 100, glm::vec4(1, 0, 0, 0));
	glUseProgram(m_programID);
	// bind the camera
	int loc = glGetUniformLocation(m_programID, "ProjectionView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(pCamera->GetProjectionView()[0][0]));

	// set texture slot
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	FBXSkeleton* skeleton = m_fbx->getSkeletonByIndex(0);

	// tell the shader where it is
	loc = glGetUniformLocation(m_programID, "diffuse");
	glUniform1i(loc, 0);

	//setting up the shader values
	//----------------------------------------------------------------------------
		//set bonoes
		int bones_location = glGetUniformLocation(m_programID, "bones");
		glUniformMatrix4fv(bones_location, skeleton->m_boneCount, GL_FALSE,	(float*)skeleton->m_bones);

		//set colour and direction of light
		unsigned int lightDirection = glGetUniformLocation(m_programID, "LightDir");
		glUniform3f(lightDirection, m_lightDir.x, m_lightDir.y, m_lightDir.z);

		//set specpower
		unsigned int specPower = glGetUniformLocation(m_programID, "SpecPow");
		glUniform1f(specPower, m_specPow);

		//set light colour
		unsigned int lightColour = glGetUniformLocation(m_programID, "LightColour");
		glUniform3f(lightColour, m_lightCol.x, m_lightCol.y, m_lightCol.z);

		//set position
		//unsigned int position = glGetUniformLocation(m_programID, "Position");
		//glUniform3f(position, m_position.x, m_position.y, m_position.z);

		//set camera pos
		unsigned int cameraPosLocation = glGetUniformLocation(m_programID, "CameraPos");
		glm::vec3 camLocation = pCamera->GetPosition();
		glUniform3fv(cameraPosLocation, 1, glm::value_ptr(camLocation));
	//----------------------------------------------------------------------------

	// bind our vertex array object and draw the mesh
	for (unsigned int i = 0; i < m_fbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = m_fbx->getMeshByIndex(i);
		unsigned int* glData = (unsigned int*)mesh->m_userData;

		unsigned int global_location = glGetUniformLocation(m_programID, "global");
		glUniformMatrix4fv(global_location, 1, GL_FALSE, &(mesh->m_globalTransform[0][0]));


		glBindTexture(GL_TEXTURE_2D, mesh->m_material->textures[FBXMaterial::DiffuseTexture]->handle);

		glBindVertexArray(glData[0]);
		glDrawElements(GL_TRIANGLES, (unsigned int)mesh->m_indices.size(), GL_UNSIGNED_INT, 0);

	}

	glUseProgram(0);
}

void FBX_Loader::Update(float _dt)
{
	m_timer += _dt;
	// Grab the skeleton and animation we want to use
	FBXSkeleton* skeleton = m_fbx->getSkeletonByIndex(0);
	FBXAnimation* animation = m_fbx->getAnimationByIndex(0);

	skeleton->updateBones();
	
	// evaluate the animation to update bones
	skeleton->evaluate(animation, m_timer);

	for( unsigned int bone_index = 0; bone_index < skeleton->m_boneCount; ++bone_index)
	{
		skeleton->m_nodes[bone_index]->updateGlobalTransform();	
	}

}
void FBX_Loader::createOpenGLBuffers(FBXFile* fbx)
{
	// create the GL VAO/VBO/IBO data for each mesh
	for (unsigned int i = 0; i < fbx->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = fbx->getMeshByIndex(i);

		// storage for the opengl data in 3 unsigned int
		unsigned int* glData = new unsigned int[3];

		glGenVertexArrays(1, &glData[0]);
		glBindVertexArray(glData[0]);

		glGenBuffers(1, &glData[1]);
		glGenBuffers(1, &glData[2]);
		glBindBuffer(GL_ARRAY_BUFFER, glData[1]);
		glBufferData(GL_ARRAY_BUFFER, mesh->m_vertices.size() * sizeof(FBXVertex), mesh->m_vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glData[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->m_indices.size() * sizeof(unsigned int), mesh->m_indices.data(), GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0); //position
		glEnableVertexAttribArray(1); //normals
		glEnableVertexAttribArray(2); //tangents
		glEnableVertexAttribArray(3); //texcoords
		glEnableVertexAttribArray(4); //weights
		glEnableVertexAttribArray(5); //indices

		//glEnableVertexAttribArray(6); //transform

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,	sizeof(FBXVertex),	(void*)FBXVertex::PositionOffset);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE,	sizeof(FBXVertex),	(void*)FBXVertex::NormalOffset);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE,	sizeof(FBXVertex),	(void*)FBXVertex::TangentOffset);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex),	(void*)FBXVertex::TexCoord1Offset);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex),	(void*)FBXVertex::WeightsOffset);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex),	(void*)FBXVertex::IndicesOffset);
		
		//glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(m_position),	0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		mesh->m_userData = glData;
	}
}

void FBX_Loader::cleanupOpenGLBuffers(FBXFile* fbx) 
{
	// clean up the vertex data attached to each mesh
	for (unsigned int i = 0; i < fbx->getMeshCount(); ++i) 
	{
		m_mesh = fbx->getMeshByIndex(i);
		unsigned int* glData = (unsigned int*)m_mesh->m_userData;
		glDeleteVertexArrays(1, &glData[0]);
		glDeleteBuffers(1, &glData[1]);
		glDeleteBuffers(1, &glData[2]);
		delete[] glData;
	}
}