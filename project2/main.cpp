#pragma once
// Math constants
#define _USE_MATH_DEFINES
#include <cmath>  
#include <random>
#include <algorithm>

// Std. Includes
#include <string>
#include <time.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include "glm/ext.hpp"

// Other Libs
#include "SOIL2/SOIL2.h"

// project includes
#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
#include "Transform.h"
#include "Particle.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace glm;
// time
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// solver consts
//const static vec3 gravity(0.0f, -9.8f, 0.0f);
//const static float REST_DENSITY = 10.10000f;		// Resting density
//const static float GAS_CONST = 100.20000f;			// For equation of state
//const static float H = 0.5f;					// Particle radius
//const static float H2 = H * H;					// H squared
//const static float MASS = 0.065f;				// Particle mass
//const static float VISC = 02.025f;				// Viscosity 
const static vec3 gravity(0.0f, -9.8f, 0.0f);
const static float REST_DENSITY = 900.0f;		// Resting density
const static float GAS_CONST = 300.0f;			// For equation of state
const static float H = 0.5f;					// Particle radius
const static float H2 = H * H;					// H squared
const static float MASS = 6.0f;				// Particle mass
const static float VISC = 25.0f;				// Viscosity 

// Smoothing kernels defined in Muller and their gradients
const static float POLY6 = 315.0f / (64.0f * M_PI * pow(H, 9.0f));
const static float SPIKY_GRAD = -45.0f / (M_PI * pow(H, 6.0f));
const static float VISC_LAP = 45.0f / ( M_PI * pow(H, 6.0f));

// Sim parameters
const static int cubeSide = 6;
const static float boundWidth = H * ((float)cubeSide + 1.0f);
const static vec3 boundMin = vec3(-boundWidth, 0.0f, -boundWidth);
const static vec3 boundMax = vec3(boundWidth, 100.0f, boundWidth);
const static float EPS = H; // Boundary epsilon
const static float BOUND_DAMPING = -0.5f;


// main function
int main()
{
	// DON'T WORK WITH OPENGL BEFORE THIS
	// create application
	Application app = Application::Application();
	app.initRender();
	Application::camera.setCameraPosition(glm::vec3(0.0f, 5.0f, 10.0f));

	// Mesh list for rendering
	vector<Mesh*> meshes;
	// List of fluid particles
	vector<Particle*> parts;
	// List of particle bonds
	vector<Cohesive*> bonds;
	// List of bonded particle pairs
	vector<pair<Body*, Body*>> bondPairs;
	
	for (int i = 0; i < cubeSide * cubeSide * (cubeSide * 2); ++i) {
		Particle* ptemp = new Particle();
		ptemp->CreateDefault();
		meshes.push_back(&ptemp->GetMesh());
		parts.push_back(ptemp);
		ptemp->SetPos(glm::vec3(boundMin[0] + H + H * (float)(i % cubeSide),
			H + H * (float)((i / cubeSide) % cubeSide),
			boundMin[2] + H + H * (float)((i / cubeSide) / cubeSide)));
		ptemp->Translate(glm::vec3((float)i * 0.001));
		//ptemp->Translate(glm::vec3(0.0f, 5.0f, 0.0f));
	}

	// create ground plane
	Mesh plane = Mesh::Mesh(Mesh::QUAD);
	// scale it up x5
	plane.Scale(glm::vec3(boundWidth));
	Shader lambert = Shader("resources/shaders/physics.vert", "resources/shaders/physics.frag");
	plane.SetShader(lambert);
	meshes.push_back(&plane);

	// time
	GLfloat firstFrame = (GLfloat)glfwGetTime();
	GLfloat accumulator = 0.0f;
	GLfloat fixedStep = 1.0f / 200.0f;
	bool paused = true;
	bool keyHeld = false;



	// InitSPH
	// Update
	//// ComputeDensityPressure
	//// ComputeForces
	//// Integrate

	// Game loop
	while (!glfwWindowShouldClose(app.getWindow()))
	{
		// Set frame time
		GLfloat currentFrame = (GLfloat)glfwGetTime() - firstFrame;
		// the animation can be sped up or slowed down by multiplying currentFrame by a factor.
		currentFrame *= 1.0f;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/*
		**	INTERACTION
		*/
		// Manage interaction
		app.doMovement(deltaTime);
		if (!keyHeld && glfwGetKey(app.getWindow(), GLFW_KEY_P) == GLFW_PRESS) {
			paused = !paused;
			keyHeld = true;
		}
		else if (keyHeld && glfwGetKey(app.getWindow(), GLFW_KEY_P) == GLFW_RELEASE) {
			keyHeld = false;
		}




		/*
		**	START FIXED STEP LOOP
		*/
		if (!paused) {
			accumulator += deltaTime;
			for (accumulator; accumulator > fixedStep; accumulator -= fixedStep) {

				// ComputeDensityPressure-----------------------------------------------------
				for (Particle* p1 : parts) {
					p1->SetDensity(0.0f);
					for (Particle* p2 : parts) {
						vec3 diff = p2->GetPos() - p1->GetPos();
						float len2 = dot(diff, diff);

						if (len2 < H2) { // less than H^2, smoothing radius of support
							p1->SetDensity(p1->GetDensity() + MASS * POLY6 * powf(H2 - len2, 3.0f));
						}
					}
					p1->SetPressure(GAS_CONST * (p1->GetDensity() - REST_DENSITY));
				}

				// ComputeForces----------------------------------------------------------------

				for (Particle* p1 : parts) {
					vec3 fpress(0.0f);
					vec3 fvisc(0.0f);
					for (Particle* p2 : parts) {
						if (p1 == p2) {
							continue;
						}

						vec3 diff = p2->GetPos() - p1->GetPos();
						float len = sqrtf(dot(diff, diff));

						if (len < H) { // less than H
							// pressure force
							fpress += -glm::normalize(diff) * MASS * (p1->GetPressure() + p2->GetPressure()) / (2.0f * p2->GetDensity()) * SPIKY_GRAD * pow(H - len, 2.0f);
							// viscosity force
							fvisc += VISC * MASS * (p2->GetVel() - p1->GetVel()) / p2->GetDensity() * VISC_LAP * (H - len);
						}
					}
					vec3 fgrav = gravity * p1->GetDensity();
					p1->SetForce(fpress + fvisc + fgrav);
				}

				// Integrate--------------------------------------------------------------------

				for (Particle* p : parts) {
					// Forward Euler
					p->SetVel(p->GetVel() + fixedStep * p->GetForce() / p->GetDensity());
					p->Translate(fixedStep * p->GetVel());
					// Boundaries
					for (int i = 0; i < 3; i++) {
						if (p->GetPos()[i] < boundMin[i] + H) {
							vec3 velChange = p->GetVel();
							velChange[i] *= BOUND_DAMPING;
							p->SetVel(velChange);
							vec3 posChange = p->GetPos();
							posChange[i] = boundMin[i] + H;
							p->SetPos(posChange);
						}
						if (p->GetPos()[i] > boundMax[i] - H) {
							vec3 velChange = p->GetVel();
							velChange[i] *= BOUND_DAMPING;
							p->SetVel(velChange);
							vec3 posChange = p->GetPos();
							posChange[i] = boundMax[i] - H;
							p->SetPos(posChange);
						}
					}
				}
			}
		}

		/*
		**  END FIXED STEP LOOP
		*/
		static bool add = false;
		if (add && currentFrame >= 10.0f) {
			add = false;
			for (int i = 0; i < pow(3, 3); ++i) {
				Particle* ptemp = new Particle();
				ptemp->CreateDefault();
				meshes.push_back(&ptemp->GetMesh());
				parts.push_back(ptemp);
				ptemp->SetPos(glm::vec3(boundMin[0] + H + H * (float)(i % cubeSide),
					H * (float)((i / cubeSide) % cubeSide),
					boundMin[2] + H + H * (float)((i / cubeSide) / cubeSide)));
				ptemp->Translate(glm::vec3((float)i * 0.01));
				ptemp->Translate(glm::vec3(0.0f, 5.0f, 0.0f));
			}
		}

		/*
		**	RENDER
		*/
		// clear buffer
		app.clear();

		for (Mesh* m : meshes) {
			app.draw(*m);
		}

		app.display();
	}

	app.terminate();

	return 0;
}

