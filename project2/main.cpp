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

#include <iostream>

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
	Application::camera.setCameraPosition(glm::vec3(0.0f, 5.0f, 20.0f));


	Transform test;
	test.SetPos(vec3(1.0f, 2.0f, 3.0f));
	test.SetRot(mat3(1.0f));
	test.SetScale(mat3(1.0f));
	// create ground plane
	Mesh plane = Mesh::Mesh(Mesh::QUAD);
	// scale it up x5
	plane.Scale(glm::vec3(5.0f, 5.0f, 5.0f));
	Shader lambert = Shader("resources/shaders/physics.vert", "resources/shaders/physics.frag");
	plane.SetShader(lambert);


	// create particle
	Mesh particle1 = Mesh::Mesh(Mesh::QUAD);
	//scale it down (x.1), translate it up by 2.5 and rotate it by 90 degrees around the x axis
	particle1.Translate(glm::vec3(0.0f, 2.5f, 0.0f));
	particle1.Scale(glm::vec3(.1f, .1f, .1f));
	particle1.Rotate((GLfloat)M_PI_2, glm::vec3(1.0f, 0.0f, 0.0f));
	particle1.SetShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));

	// create demo objects (a cube and a sphere)
	Mesh sphere = Mesh::Mesh(Mesh::CUBE);
	sphere.Translate(glm::vec3(-1.0f, 1.0f, 0.0f));
	sphere.SetShader(lambert);
	Mesh cube = Mesh::Mesh(Mesh::CUBE);
	cube.Translate(glm::vec3(1.0f, .5f, 0.0f));
	cube.SetShader(lambert);

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
			particle1.Translate(glm::vec3(1.0f, 0.0f, 0.0f) * fixedStep);




		}

		/*
		**  END FIXED STEP LOOP
		*/

		/*
		**	RENDER
		*/
		// clear buffer
		app.clear();
		// draw groud plane
		app.draw(plane);
		// draw particles
		app.draw(particle1);

		// draw demo objects
		app.draw(cube);
		app.draw(sphere);

		app.display();
	}

	app.terminate();

	return EXIT_SUCCESS;
}

