#pragma once
#include "IRenderable.h"

class Model :
	public IRenderable {

public:

	Model(const std::string& ModelFilename, GLuint VAO, GLint ParentIndex, glm::vec3 Translation = { 0, 0, 0 }, glm::vec3 Axis = { 0, 0, 0 }, GLfloat Degrees = 0.0f);
	~Model();



	GLint GetParent() const;

	void Translate(glm::vec3 xform);
	void Translate(GLint x = 0, GLint y = 0, GLint z = 0);

	glm::vec3 GetTransform() { return this->m_v3CombinedTransform; }


private:

	GLint m_nParentID{ -1 };

	GLfloat m_fDegrees{ 0.0f };

	Helpers::ModelLoader m_mlModel;
	void SetTransform();

	GLboolean Initialise() override final;
	GLboolean SetVectors();
	GLboolean LoadMesh();

};

