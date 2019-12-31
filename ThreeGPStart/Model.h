#pragma once
#include "IRenderable.h"

class Model :
	public IRenderable {

public:

	Model(const std::string& ModelFilename, GLuint VAO, GLint ParentIndex, glm::vec3 Translation = { 0, 0, 0 }, glm::vec3 Axis = { 0, 0, 0 }, GLfloat Degrees = 0.0f, const std::string& TextureFilename = "default", GLint TexID = 1);
	Model(const std::string& ModelFilename, GLuint VAO, const std::string& TextureFilename = "default", GLint TexID = 1);
	Model(const std::string& ModelFilename, GLuint VAO, GLint ParentIndex, const std::string& TextureFilename = "default", GLint TexID = 1);
	~Model();



	GLint GetParent() const;

	void Translate(glm::vec3 xform = glm::vec3(0, 0, 0));
	void Translate(GLint x = 0, GLint y = 0, GLint z = 0);

	glm::vec3 GetTransform() { return this->m_v3CombinedTransform; }
	GLfloat GetDegrees() { return this->m_fDegrees; }
	glm::vec3 GetAxis() { return this->m_v3RotationAxis; }

private:

	GLint m_nParentID{ -1 };

	GLfloat m_fDegrees{ 0.0f };

	Helpers::ModelLoader m_mlModel;


	GLboolean Initialise() override final;
	GLboolean SetVectors();
	GLboolean LoadMesh();
	void SetTransform();

};

