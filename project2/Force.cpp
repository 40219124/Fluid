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

glm::vec3 LinearForce::ApplyForce(double dt)
{
	return linearForce * (float)dt;
}