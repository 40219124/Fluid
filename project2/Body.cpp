#include "Body.h"

using namespace std;

Body::Body() {}


Body::~Body() {}

glm::vec3 Body::ApplyForces(double dt, glm::vec3 bodyPos) {
	// Output vector
	glm::vec3 out = glm::vec3(0.0f);
	// Sum force accelerations
	for (int i = 0; i < m_forces.size(); ++i) {
		out = out + m_forces[i]->CalculateForce(dt, bodyPos);
		// If force is to be destroyed remove it from list
		if (m_forces[i]->ToDestroy()) {
			m_forces.erase(m_forces.begin() + i);
			i--;
		}
	}
	// Divide acceleration by mass
	out /= (m_mass != 0 ? m_mass : 1.0f);
	// Check for large acceleration values
	if (glm::dot(out, out) > 10000) {
		out = 100.0f * glm::normalize(out);
	}
	// Return vector
	return out;
}
