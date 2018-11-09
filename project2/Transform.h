#pragma once
#include "Mesh.h"
#include "glm/glm.hpp"

class Transform
{
public:
	Transform();
	~Transform();
	// Get methods
	glm::vec3 GetPos() { return glm::vec3(m_pos[3]); }
	glm::mat4 GetRot() { return m_rot; }
	glm::mat4 GetScale() { return m_scale; }
	glm::mat4 GetTransform() { return m_model; }
	Mesh& GetMesh() { return m_mesh; }
	// Set methods
	void SetPos(const glm::vec3& p);
	void SetRot(const glm::mat4& r);
	void SetScale(const glm::mat4& s);
	void SetTransform(const glm::mat4& t);
	void SetMesh(const Mesh& m) { m_mesh = m; }
	// Transform methods
	void Translate(const glm::vec3& t);
	void Rotate(float value, const glm::vec3& axis);
	void Scale(const glm::vec3& scale);

protected:
	void updateTransform();

	Mesh m_mesh;

	glm::mat4 m_pos = glm::mat4();
	glm::mat4 m_rot = glm::mat4();
	glm::mat4 m_scale = glm::mat4();

	glm::mat4 m_model = glm::mat4();
};

