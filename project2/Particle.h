#pragma once
#include "Body.h"

class Particle : public Body
{
public:
	Particle();
	~Particle();
	// Create default particle
	void CreateDefault();

	void SetForce(glm::vec3 f) { m_force = f; }
	void SetDensity(float rho) { m_density = rho; }
	void SetPressure(float p) { m_pressure = p; }

	glm::vec3 GetForce() { return m_force; }
	float GetDensity() { return m_density; }
	float GetPressure() { return m_pressure; }

private:
	glm::vec3 m_force = glm::vec3(0.0f);	// f
	float m_density = 0.0f;		// rho
	float m_pressure = 0.0f;	// p
};

