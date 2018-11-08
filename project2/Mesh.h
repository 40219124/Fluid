#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "OBJloader.h"
#include "Shader.h"

/* 
** VERTEX CLASS 
*/
class Vertex {
public:
	Vertex() {
		m_coord = glm::vec3();
	}

	Vertex(const glm::vec3& coord) {
		this->m_coord = coord;
	}

	glm::vec3 GetCoord() const { return m_coord; }
	void SetCoord(const glm::vec3& coord) { m_coord = coord; }

protected:
private:
	glm::vec3 m_coord;
};

enum MeshBufferPositions
{
	POSITION_VB,
	TEXCOORD_VB,
	NORMAL_VB,
	INDEX_VB
};

/* 
** MESH CLASS 
*/
class Mesh
{
public:

	enum MeshType
	{
		TRIANGLE,
		QUAD,
		CUBE
	};
	/*
	** CONSTRUCTORS
	*/

	// default constructor creates a horizontal plane or dimensions 1 x 1 centered on the origin
	Mesh();
	// construct a mesh from a type
	Mesh(MeshType type);
	// create mesh from a .obj file
	Mesh(const std::string& fileName);
	// destructor
	virtual ~Mesh();

	/*
	** GET AND SET METHODS
	*/

	// getModel computes the model matrix any time it is required
	glm::vec3 GetPos() const { return GetTranslate()[3]; }
	glm::mat4 GetModel() const{ return GetTranslate() * GetRotate() * GetScale();}
	glm::mat4 GetTranslate() const{ return m_translate; }
	glm::mat4 GetRotate() const{ return m_rotate; }
	glm::mat4 GetScale() const{ return m_scale; }

	Shader GetShader() const { return m_shader; }
	unsigned int GetNumIndices() const{ return m_numIndices; }

	// set position of mesh center to specified 3D position vector
	void SetPos(const glm::vec3 &position) {
		m_translate[3][0] = position[0];
		m_translate[3][1] = position[1];
		m_translate[3][2] = position[2];
	}
	// set i_th coordinate of mesh center to float p (x: i=0, y: i=1, z: i=2)
	void SetPos(int i, float p) { m_translate[3][i] = p; }

	// allocate shader to mesh
	void SetShader(const Shader &shader) {
		m_shader = shader;
		m_shader.Use();
	}

	// get buffers and array references
	GLuint GetVertexArrayObject() const { return m_vertexArrayObject; }
	GLuint GetVertexBuffer() const { return m_vertexBuffer; }
	GLuint GetNormalBuffer() const { return m_normalBuffer; }

	/* 
	** INITIALISATION AND UTILITY METHODS
	*/

	// initialise transform matrices to identity
	void InitTransform();
	// create mesh from vertices
	void InitMesh(Vertex* vertices, glm::vec3* normals);
	// create mesh from model (typically loaded from a file)
	void InitMesh(const IndexedModel& model);


	// load .obj file
	void LoadOBJ(const char * path,
		std::vector < glm::vec3 > & out_vertices,
		std::vector < glm::vec2 > & out_uvs,
		std::vector < glm::vec3 > & out_normals
	);


	/*
	** TRANSFORMATION METHODS
	*/
	
	// translate mesh by a vector
	void Translate(const glm::vec3 &vect);
	// rotate mesh by a vector
	void Rotate(const float &angle, const glm::vec3 &vect);
	// scale mesh by a vector
	void Scale(const glm::vec3 &vect);

	GLuint m_vertexArrayObject;
	GLuint m_vertexBuffer;
	GLuint m_normalBuffer;

private:

	unsigned int m_numIndices;
	glm::mat4 m_translate;
	glm::mat4 m_rotate;
	glm::mat4 m_scale;

	Shader m_shader;
};


