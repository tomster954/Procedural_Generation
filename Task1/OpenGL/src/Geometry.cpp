#include "Geometry.h"
#include "FBX_Loader.h"


#include "GUI_Bar.h"
#include "Camera.h"
#include "ParticleEmitter.h"
#include "GPUParticleEmitter.h"
#include "RenderTargets.h"
#include "Proc_Gen.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "FBXFile.h"

#include "PostProcessing.h"

Geometry::Geometry(GLFWwindow *_pWindow)
{
	m_pWindow = _pWindow;
	StartUp();
}

Geometry::~Geometry()
{
	delete m_particleEmitter;
	delete m_GPUParticleEmitter;
	delete m_myBar;
	delete m_particlesBar;
	
	delete m_mirror;
	delete m_postProcessingEffects;
	delete m_terrain;

	delete m_Pyro;
	delete m_other;
}

void Geometry::StartUp()
{
	m_Camera = nullptr;

	m_terrain = new ProcGen(300, 300, m_pWindow);

	//m_postProcessingEffects = new PostProcessing();
	m_mirror = new RenderTargets();

	//GUIs
	//------------------------------------
	m_myBar = new GUI_Bar(m_pWindow);
	//------------------------------------

	//CPU Particles
	//------------------------------------
	m_particleEmitter = new ParticleEmitter();
	m_particleEmitter->initalise(1000, 500, 0.1f, 1.0f,	1, 5, 1, 0.1f, glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));
	//------------------------------------

	//GPU Particles
	//------------------------------------
	m_GPUParticleEmitter = new GPUParticleEmitter();
	m_GPUParticleEmitter->initalise(100000, 0.1f, 5.0f, 5, 20, 1, 0.1f, glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));
	//------------------------------------

	m_Pyro = new FBX_Loader("./data/characters/Pyro/pyro.fbx");
	
	m_other = new FBX_Loader("./data/characters/Marksman/Marksman.fbx");

}

void Geometry::Update(float _dt)
{
	m_Pyro->Update(_dt);
	m_other->Update(_dt);

	m_terrain->Update(_dt);

	if (m_Camera != nullptr)
		m_particleEmitter->Update(_dt, m_Camera->GetTransform());
}

void Geometry::Draw(Camera *_pCamera)
{
	//setting camera and its speed
	m_Camera = _pCamera;
	m_Camera->SetCameraSpeed(m_myBar->GetCameraSpeed());

	//m_postProcessingEffects->Draw(_pCamera);
	m_mirror->SetAsActiveRenderTarget();

	//do everything here that was want to draw on the mirror

	//Drawing and setting FBX modles
	DrawFBXModles(_pCamera);

	// Drawing and setting CPUparticles
	DrawCPUParticles(_pCamera);

	// drawing GPUparticles
	DrawGPUParticles(_pCamera);

	//Draw Terrain
	DrawTerrain(_pCamera);
	
	
	
	//Clear As Active Render Target
	m_mirror->ClearAsActiveRenderTarget();
	m_mirror->Draw(_pCamera);
	
	//Drawing GUIs
	m_myBar->Draw();
}

void Geometry::DrawCPUParticles(Camera *_pCamera)
{
	m_particleEmitter->Draw(m_Camera);

	m_particleEmitter->SetLifeSpanMin(m_myBar->GetLifeSpanMin());
	m_particleEmitter->SetLifeSpanMax(m_myBar->GetLifeSpanMax());

	m_particleEmitter->SetVelocityMin(m_myBar->GetVelocityMin());
	m_particleEmitter->SetVelocityMax(m_myBar->GetVelocityMax());

	m_particleEmitter->SetStartSize(m_myBar->GetStartSize());
	m_particleEmitter->SetEndSize(m_myBar->GetEndSize()) ;

	m_particleEmitter->SetMaxParticles(m_myBar->GetMaxParticles());

	m_particleEmitter->SetStartColour(m_myBar->GetStartColour());
	m_particleEmitter->SetEndColour(m_myBar->GetEndColour()) ;
}

void Geometry::DrawGPUParticles(Camera *_pCamera)
{
	m_GPUParticleEmitter->Draw((float)glfwGetTime(), m_Camera->GetTransform(), m_Camera->GetProjectionView());
}

void Geometry::DrawFBXModles(Camera *_pCamera)
{
	//Drawing the pyro
	//---------------------------------
		m_Pyro->Draw(m_Camera);
		m_Pyro->SetLightDir(m_myBar->GetLightDir());
		m_Pyro->SetLightColour(m_myBar->GetLightCol());
		m_Pyro->SetSecPower(m_myBar->GetSpecPower());
	//---------------------------------
	
	//Drawing the pyro
	//---------------------------------
		m_other->Draw(m_Camera);
		m_other->SetLightDir(m_myBar->GetLightDir());
		m_other->SetLightColour(m_myBar->GetLightCol());
		m_other->SetSecPower(m_myBar->GetSpecPower());
	//---------------------------------

}

void Geometry::DrawTerrain(Camera *_pCamera)
{
	m_terrain->Draw(_pCamera);
}