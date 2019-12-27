#pragma once
#include "IRenderable.h"

class Model :
	public IRenderable {

public:

	Model(const std::string ModelFilename, GLuint& VAO, GLint ParentIndex = -1, glm::vec3 Translation = glm::vec3(0.0f), glm::vec3 Axis = glm::vec3(0.0f), GLfloat Degrees = 0.0f);
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

