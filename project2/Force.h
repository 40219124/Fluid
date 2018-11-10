#pragma once
#include <glm/glm.hpp>
#include <utility>

class Body;

class Force
{
public:
	Force();
	virtual ~Force();

	virtual glm::vec3 CalculateForce(double dt, glm::vec3 bodyPos) { return glm::vec3(0); }

	bool ToDestroy() { return m_toDestroy; }

protected:
	bool m_toDestroy = false;
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

	std::pair<Body*, Body*> GetPair() { return std::pair<Body*, Body*>(m_body1, m_body2); }

private:
	Body* m_body1;
	Body* m_body2;
	float limit = 3.0f;
};

