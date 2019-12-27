#pragma once
#include "IRenderable.h"

class Model :
	public IRenderable {

public:

	Model(const std::string& ModelFilename, GLuint VAO, GLint ParentIndex, glm::vec3 Translation = { 0, 0, 0 }, glm::vec3 Axis = { 0, 0, 0 }, GLfloat Degrees = 0.0f);
	~Model();

private:

	GLint m_unParentID{ -1 };

	GLfloat m_fDegrees{ 0.0f };

	Helpers::ModelLoader m_mlModel;

	void Translate(glm::vec3 xform);
	void Translate(GLint x = 0, GLint y = 0, GLint z = 0);
	void SetTransform();

	GLboolean Initialise() override final;
	GLboolean SetVectors();
	GLboolean LoadMesh();

};

