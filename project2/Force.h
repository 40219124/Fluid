#pragma once
#include <glm/glm.hpp>

class Body;

class Force
{
public:
	Force();
	virtual ~Force();

	virtual glm::vec3 CalculateForce(double dt, glm::vec3 bodyPos) { return glm::vec3(0); }
};

class LinearForce : public Force {
public:
	LinearForce();
	LinearForce(glm::vec3 dir) { SetForce(dir); }
	~LinearForce();

	glm::vec3 CalculateForce(double dt, glm::vec3 bodyPos);

	glm::vec3 GetForce() { return m_linearForce; }
	void SetForce(glm::vec3 v) { m_linearForce = v; }

private:
	glm::vec3 m_linearForce;
};

class Cohesive : public Force {
public:
	Cohesive(Body* b1, Body* b2);
	~Cohesive();

	glm::vec3 CalculateForce(double dt, glm::vec3 bodyPos);

	bool Broken() { return m_broken; }

private:
	Body* m_body1;
	Body* m_body2;

	bool m_broken = false;
};

