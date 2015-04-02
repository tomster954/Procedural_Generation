//-----------------------------------------------------------------------------
//Author: Tommas Solarino
//Description: GPU paticle emiiter
//-----------------------------------------------------------------------------

#ifndef GPU_PARTICLE_EMITTER
#define GPU_PARTICLE_EMITTER

#include <gl_core_4_4.h>
#include <glm/glm.hpp>

struct GPUParticle 
{
	GPUParticle() : lifetime(1), lifespan(0) {}
	glm::vec3 position;
	glm::vec3 velocity;
	float lifetime;
	float lifespan;
};

class GPUParticleEmitter
{
public:
	GPUParticleEmitter();
	virtual ~GPUParticleEmitter();
	
	void initalise(	unsigned int a_maxParticles,
					float a_lifespanMin, float a_lifespanMax,
					float a_velocityMin, float a_velocityMax,
					float a_startSize, float a_endSize,
					const glm::vec4& a_startColour,
					const glm::vec4& a_endColour
				  );
					
	void Draw(	float time, const glm::mat4& a_cameraTransform,
				const glm::mat4& a_projectionView
			 );

	//loads shader program
	unsigned int loadShader(unsigned int type, const char* path);

private:

protected:
	void createBuffers();
	void createUpdateShader();
	void createDrawShader();

	GPUParticle* m_particles;

	unsigned int m_maxParticles;

	glm::vec3 m_position;

	float m_lifespanMin;
	float m_lifespanMax;

	float m_velocityMin;
	float m_velocityMax;

	float m_startSize;
	float m_endSize;

	glm::vec4 m_startColour;
	glm::vec4 m_endColour;

	unsigned int m_activeBuffer;
	unsigned int m_vao[2];
	unsigned int m_vbo[2];

	unsigned int m_drawShader;
	unsigned int m_updateShader;

	float m_lastDrawTime;
};

#endif