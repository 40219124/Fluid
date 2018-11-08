#pragma once
#include "Mesh.h"
#include "glm/glm.hpp"

class Transform
{
public:
	Transform();
	~Transform();
	// Get methods
	glm::vec3& GetPos() { return m_pos; }
	glm::mat3& GetRot() { return m_rot; }
	glm::mat3& GetScale() { return m_scale; }
	glm::mat4& GetTransform() { return m_trans; }
	Mesh& GetMesh() { return m_mesh; }
	// Set methods
	void SetPos(const glm::vec3& p);
	void SetRot(const glm::mat3& r);
	void SetScale(const glm::mat3& s);
	void SetTransform(const glm::mat4& t);
	void SetMesh(const Mesh& m) { m_mesh = m; }

private:
	void updateTransform();

	Mesh m_mesh = Mesh();

	glm::vec3 m_pos = glm::vec3();
	glm::mat3 m_rot = glm::mat3();
	glm::mat3 m_scale = glm::mat3();

	glm::mat4 m_trans = glm::mat4();
};

