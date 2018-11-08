#include "Body.h"

using namespace std;

Body::Body() {}


Body::~Body() {}

glm::vec3 Body::ApplyForces(double dt, glm::vec3 bodyPos) {
	// Output vector
	glm::vec3 out = glm::vec3(0.0f);
	// Sum force accelerations
	for (Force* f : m_forces) {
		out = out + f->CalculateForce(dt, bodyPos);
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
