#include "Force.h"
#include "Body.h"

// FORCE ---------------

Force::Force() {}


Force::~Force() {}

// LINEAR FORCE ----------

LinearForce::LinearForce() {}
LinearForce::~LinearForce() {}

glm::vec3 LinearForce::CalculateForce(double dt, glm::vec3 bodyPos) {
	return m_linearForce;
}

// COHESIVE FORCE ------------

Cohesive::Cohesive(Body* b1, Body* b2) : m_body1(b1), m_body2(b2) {}
Cohesive::~Cohesive() {}

glm::vec3 Cohesive::CalculateForce(double dt, glm::vec3 bodyPos) {
	glm::vec3 outF = glm::vec3();
	glm::vec3 dif = m_body2->GetPos() - m_body1->GetPos();
	float difLen2 = glm::dot(dif, dif);
	if (difLen2 == 0) {
		return outF;
	}
	if (difLen2 < 1.0f) {
		outF = glm::normalize(dif) * 20.0f * (1 - difLen2);
		if (bodyPos == m_body1->GetPos())
		{
			outF *= -1.0f;
		}
	}
	else if (difLen2 > 1.0f) {
		outF = glm::normalize(dif) * difLen2/sqrtf(difLen2);
		if (bodyPos == m_body2->GetPos())
		{
			outF *= -1.0f;
		}
	}
	return outF;
}