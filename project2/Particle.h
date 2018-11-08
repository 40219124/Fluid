#pragma once
#include "Body.h"

class Particle : public Body
{
public:
	Particle();
	~Particle();
	// Create default particle
	void CreateDefault();
};

