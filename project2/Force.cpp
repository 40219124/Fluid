#include "Force.h"

// FORCE ---------------

Force::Force()
{
}


Force::~Force()
{
}

// LINEAR FORCE ----------

LinearForce::LinearForce() {}
LinearForce::~LinearForce() {}

glm::vec3 LinearForce::CalculateForce(double dt, glm::vec3 bodyPos)
{
	return linearForce * (float)dt;
}