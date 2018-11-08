#pragma once
#include <glm\glm.hpp>
#include <vector>
#include "Transform.h"
#include "Force.h"

class Body : public Transform
{
public:
	Body();
	virtual ~Body();
	// Get methods
	glm::vec3& GetAcc() { return m_acc; }
	glm::vec3& GetVel() { return m_vel; }
	float GetMass() { return m_mass; }
	// Set methods
	void SetAcc(glm::vec3& a) { m_acc = a; }
	void SetVel(glm::vec3& v) { m_vel = v; }
	void SetMass(float m) { m_mass = m; }
	// Force methods
	void AddForce(Force* f) { m_forces.push_back(std::move(f)); }
	glm::vec3 ApplyForces(double dt, glm::vec3 bodyPos);

protected:
	glm::vec3 m_acc;
	glm::vec3 m_vel;
	float m_mass;
	std::vector<Force*> m_forces;
};

