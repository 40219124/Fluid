#include "Particle.h"



Particle::Particle() {}


Particle::~Particle() {}

void Particle::CreateDefault() {
	m_mesh = Mesh::Mesh(Mesh::MeshType::QUAD);
	m_acc = glm::vec3(0.0f);
	m_vel = glm::vec3(0.0f);
	m_mass = 1.0f;
	Scale(glm::vec3(0.25f));
	Rotate(glm::half_pi<GLfloat>(), glm::vec3(1.0f, 0.0f, 0.0f));
	m_forces.push_back(new LinearForce(glm::vec3(0.0f, -9.8f, 0.0f)));
	m_mesh.SetShader(Shader("resources/shaders/solid.vert", "resources/shaders/solid_blue.frag"));
}
