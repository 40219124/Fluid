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

	//Particle p1;
	//parts.push_back(&p1);
	//p1.CreateDefault();
	//meshes.push_back(&p1.GetMesh());
	//p1.SetPos(glm::vec3(0.0f, 10.0f, 0.0f));

	//Particle p2;
	//parts.push_back(&p2);
	//p2.CreateDefault();
	//meshes.push_back(&p2.GetMesh());
	//p2.SetPos(glm::vec3(0.25f, 12.0f, 0.0f));

	int cubeSide = 3;

	for (int i = 0; i < cubeSide * cubeSide * cubeSide; ++i) {
		Particle* ptemp = new Particle();
		ptemp->CreateDefault();
		meshes.push_back(&ptemp->GetMesh());
		parts.push_back(ptemp);
		ptemp->SetPos(glm::vec3(0.5 * (float)(i % cubeSide),
			0.5 * (float)((i / cubeSide) % cubeSide),
			0.5 * (float)((i / cubeSide) / cubeSide)));
		ptemp->Translate(glm::vec3((float)i * 0.01));
		ptemp->Translate(glm::vec3(0.0f, 5.0f, 0.0f));
	}

	// create ground plane
	Mesh plane = Mesh::Mesh(Mesh::QUAD);
	// scale it up x5
	plane.Scale(glm::vec3(5.0f, 5.0f, 5.0f));
	Shader lambert = Shader("resources/shaders/physics.vert", "resources/shaders/physics.frag");
	plane.SetShader(lambert);
	meshes.push_back(&plane);


	//// create particle
	//Mesh particle1 = Mesh::Mesh(Mesh::QUAD);
	////scale it down (x.1), translate it up by 2.5 and rotate it by 90 degrees around the x axis
	//particle1.Translate(glm::vec3(0.0f, 2.5f, 0.0f));
	//particle1.Scale(glm::vec3(.1f, .1f, .1f));
	//particle1.Rotate((GLfloat)M_PI_2, glm::vec3(1.0f, 0.0f, 0.0f));
	//particle1.SetShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
	//meshes.push_back(&particle1);

	//// create demo objects (a cube and a sphere)
	//Mesh sphere = Mesh::Mesh(Mesh::CUBE);
	//sphere.Translate(glm::vec3(-1.0f, 1.0f, 0.0f));
	//sphere.SetShader(lambert);
	//meshes.push_back(&sphere);
	//Mesh cube = Mesh::Mesh(Mesh::CUBE);
	//cube.Translate(glm::vec3(1.0f, .5f, 0.0f));
	//cube.SetShader(lambert);
	//meshes.push_back(&cube);

	// time
	GLfloat firstFrame = (GLfloat)glfwGetTime();
	GLfloat accumulator = 0.0f;
	GLfloat fixedStep = 1.0f / 30.0f;

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


		/*
		**	START FIXED STEP LOOP
		*/

		accumulator += deltaTime;
		for (accumulator; accumulator > fixedStep; accumulator -= fixedStep) {
			// Calculate acceleration
			for (Particle* p : parts) {
				p->SetAcc(p->ApplyForces(fixedStep, p->GetPos()));
			}
			// Calculate velocity and translation
			for (Particle* p : parts) {
				p->SetVel(p->GetVel() + p->GetAcc() * fixedStep);
				p->Translate(p->GetVel() * fixedStep);
			}
			// Remove broken cohesive forces
			for (int coI = 0; coI < bonds.size(); ++coI) {
				if (bonds[coI]->ToDestroy()) {
					pair<Body*, Body*> oldPair = bonds[coI]->GetPair();
					delete bonds[coI];
					bonds.erase(bonds.begin() + coI);
					bondPairs.erase(remove(bondPairs.begin(), bondPairs.end(), oldPair));
					--coI;
				}
			}
			// Calculate collisions/proximities
			for (int p = 0; p < parts.size(); ++p) {
				// If particle below the plane, do collision floor response
				if (parts[p]->GetPos().y < plane.GetPos().y) {
					// Reverse vel, and dampen
					glm::vec3 revVel = parts[p]->GetVel();
					revVel.y *= -0.1;
					parts[p]->SetVel(revVel);
					// Account for travelling into ground
					parts[p]->Translate(glm::vec3(0.0f, plane.GetPos().y - parts[p]->GetPos().y, 0.0f));
				}
				// For particles further in the list than this particle
				for (int fore = p + 1; fore < parts.size(); ++fore) {
					// Make pair of particles
					pair<Body*, Body*> thisPair = pair<Body*, Body*>(parts[p], parts[fore]);
					// Check if bond with this pair exists
					if (find(bondPairs.begin(), bondPairs.end(), thisPair) != bondPairs.end()) {
						continue;
					}
					// Get the vector between the two particles
					glm::vec3 diff = parts[fore]->GetPos() - parts[p]->GetPos();
					// Squared length of vector between particles
					float diffLen2 = glm::dot(diff, diff);
					// If lenght is < 2 (sqlen < 4)
					if (diffLen2 < 4.0f) {
						// Temporary cohesive force
						Cohesive* tc = new Cohesive(parts[p], parts[fore]);
						// Add force to both particles
						parts[p]->AddForce(tc);
						parts[fore]->AddForce(tc);
						// Add force to cohesive list
						bonds.push_back(tc);
						// Add pair to bond pair list
						bondPairs.push_back(thisPair);
					}
				}
			}

		}
		cout << bonds.size() << ", ";
		cout << bondPairs.size() << endl;
		/*
		**  END FIXED STEP LOOP
		*/

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

	return EXIT_SUCCESS;
}

