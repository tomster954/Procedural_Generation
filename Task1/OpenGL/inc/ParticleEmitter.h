//-----------------------------------------------------------------------------
//Author: Tommas Solarino
//Description: this is a particle emitters
//-----------------------------------------------------------------------------

#ifndef PARTICLES
#define PARTICLES

#include <glm/glm.hpp>

class Camera;

struct Particle 
{
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 colour;
	float size;
	float lifetime;
	float lifespan;
};

struct ParticleVertex 
{
	glm::vec4 position;
	glm::vec4 colour;
};

class ParticleEmitter
{
public:
	ParticleEmitter();

	~ParticleEmitter();

	void ParticleEmitter::initalise
	(
		unsigned int a_maxParticles, unsigned int a_emitRate, float a_lifetimeMin, 
		float a_lifetimeMax, float a_velocityMin, float a_velocityMax, float a_startSize, 
		float a_endSize, const glm::vec4& a_startColour, const glm::vec4& a_endColour
	);
	void ParticleEmitter::emit(); 

	void StartUp();

	void Update(float _dt, const glm::mat4& a_cameraTransform);
	void Draw(Camera *_pCamera);

	//GUI should uses these
	//----------------------------------------------------------------------------
	void SetLifeSpanMin(float _minLifeSpan ){ m_lifespanMin = _minLifeSpan; }
	void SetLifeSpanMax(float _maxLifeSpan ){ m_lifespanMax = _maxLifeSpan; }
						 
	void SetVelocityMin(float _minVelocity ){ m_velocityMin = _minVelocity; }
	void SetVelocityMax(float _maxVelocity ){ m_velocityMax = _maxVelocity; }

	void SetStartSize( float _startSize ) { m_startSize = _startSize; }
	void SetEndSize(float _endSize ) { m_endSize = _endSize;}

	void SetMaxParticles(unsigned int _maxParticles) { m_maxParticles = _maxParticles; }

	void SetStartColour(glm::vec4 _startColour) { m_startColour = _startColour; }
	void SetEndColour(glm::vec4 _endColour ) { m_endColour = _endColour; }
	//----------------------------------------------------------------------------

private:
	Particle* m_particles;
	
	unsigned int m_firstDead;
	unsigned int m_maxParticles;
	
	unsigned int m_vao, m_vbo, m_ibo;
	
	ParticleVertex* m_vertexData;
	
	unsigned int m_programID;

	glm::vec3 m_position;
	float m_emitTimer;
	float m_emitRate;
	float m_lifespanMin;
	float m_lifespanMax;
	float m_velocityMin;
	float m_velocityMax;
	float m_startSize;
	float m_endSize;
	
	glm::vec4 m_startColour;
	glm::vec4 m_endColour;

};


#endif