//-----------------------------------------------------------------------------
//Author: Tommas Solarino
//Description: Handles loading of objects and textures
//-----------------------------------------------------------------------------

#ifndef GEOMETRY
#define GEOMETRY

class Camera;
class ObjLoader;
class GUI_Bar;
class ParticleEmitter;
class GPUParticleEmitter;
class FBX_Loader;
class RenderTargets;
class PostProcessing;
class ProcGen;

struct GLFWwindow;

class Geometry
{
public:
	Geometry(GLFWwindow *_pWindow);
	~Geometry();
	
	void StartUp();

	void Update(float _dt);
	void Draw(Camera *_pCamera);

	void DrawCPUParticles(Camera *_pCamera);
	void DrawGPUParticles(Camera *_pCamera);

	void DrawFBXModles(Camera *_pCamera);

	void DrawTerrain(Camera *_pCamera);
private:
	GLFWwindow *m_pWindow;
	Camera * m_Camera;
	//ObjLoader *m_objBeretta;
	//ObjLoader *m_objTalkie;

	ProcGen* m_terrain;

	FBX_Loader* m_fbx;

	ParticleEmitter* m_particleEmitter;

	GPUParticleEmitter* m_GPUParticleEmitter;

	GUI_Bar *m_myBar;
	GUI_Bar *m_particlesBar;

	RenderTargets *m_mirror;
	PostProcessing* m_postProcessingEffects;
};

#endif