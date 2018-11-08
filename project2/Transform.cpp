#include "Transform.h"



Transform::Transform() {}


Transform::~Transform() {}
// Set new pos and update transorm matrix
void Transform::SetPos(const glm::vec3& p) {
	m_pos = p;
	updateTransform();
	m_mesh.SetPos(m_pos);
}
// Set new rotation and update transorm matrix
void Transform::SetRot(const glm::mat3& r) {
	m_rot = r;
	updateTransform();

	// Calculate rotation and apply to mesh

}
// Set new scale and update transorm matrix
void Transform::SetScale(const glm::mat3& s) {
	// Calculate relative scaling for mesh
	glm::vec3 scale;
	for (int i = 0; i < 3; ++i) {
		scale[i] = s[i][i] / m_scale[i][i];
	}
	m_mesh.Scale(scale);
	// Assign scale matrix
	m_scale = s;
	updateTransform();
}
void Transform::SetTransform(const glm::mat4& t) {
	// Set object transform
	m_trans = t;
	// Set object position
	SetPos(m_trans[3]);
	// Set object scale
	glm::mat3 newScale;
	for (int i = 0; i < 3; ++i) {
		newScale[i][i] = glm::length(glm::vec3(m_trans[0]));
	}
	SetScale(newScale);
	// Create 1/m_rot
	glm::mat3 iScale = glm::mat3();
	for (int i = 0; i < 3; ++i) {
		// Avoid dividing by 0
		if (m_scale[i][i] != 0) {
			iScale[i][i] = 1.0f / m_scale[i][i];
		}
	}
	// Set object rotation
	SetRot(glm::mat3(m_trans) * iScale);
}
// Calculate new transform matrix with updated p||r||s
void Transform::updateTransform() {
	m_trans = glm::mat4(m_rot) * glm::mat4(m_scale);
	m_trans[3] = glm::vec4(m_pos, 1.0f);
}

