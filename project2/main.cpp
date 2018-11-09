#pragma once
// Math constants
#define _USE_MATH_DEFINES
#include <cmath>  
#include <random>

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
	vector<Particle*> parts;

	Particle p1;
	parts.push_back(&p1);
	p1.CreateDefault();
	meshes.push_back(&p1.GetMesh());
	p1.SetPos(glm::vec3(0.0f, 10.0f, 0.0f));

	Particle p2;
	parts.push_back(&p2);
	p2.CreateDefault();
	meshes.push_back(&p2.GetMesh());
	p2.SetPos(glm::vec3(0.5f, 10.0f, 0.0f));

	Cohesive* coh = new Cohesive(&p1, &p2);
	p1.AddForce(coh);
	p2.AddForce(coh);


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
	GLfloat fixedStep = 1.0f / 60.0f;

	// Game loop
	while (!glfwWindowShouldClose(app.getWindow()))
	{
		// Set frame time
		GLfloat currentFrame = (GLfloat)glfwGetTime() - firstFrame;
		// the animation can be sped up or slowed down by multiplying currentFrame by a factor.
		currentFrame *= 1.5f;
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
			// Calculate collisions/proximities
			for (Particle* p : parts) {
				if (p->GetPos().y < plane.GetPos().y) {
					p->SetVel(-p->GetVel() * 0.4f);
					p->Translate(glm::vec3(0.0f, plane.GetPos().y - p->GetPos().y , 0.0f));
				}
			}



		}

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

