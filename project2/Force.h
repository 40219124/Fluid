#pragma once
#include <glm/glm.hpp>

class Force
{
public:
	Force();
	virtual ~Force();

	virtual glm::vec3 ApplyForce(double dt) { return glm::vec3(0); }
};

class LinearForce : public Force {
public:
	LinearForce();
	~LinearForce();

	glm::vec3 ApplyForce(double dt);

	glm::vec3 GetForce() { return linearForce; }
	void SetForce(glm::vec3 v) { linearForce = v; }

private:
	glm::vec3 linearForce;
};

