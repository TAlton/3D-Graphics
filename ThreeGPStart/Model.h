#pragma once
#include "IRenderable.h"
class Model :
	public IRenderable {

public:

	Model(const std::string& ModelFilename, const std::string& TextureFilename, GLuint& VAO, GLint ParentIndex = -1, glm::vec3 Translation = glm::vec3(0.0f), glm::vec3 Axis = glm::vec3(0.0f), GLfloat Degrees = 0.0f);
	~Model();

private:

	GLint m_unParentID{ -1 };

	glm::vec3 m_v3Translation{ 0.0f, 0.0f, 0.0f },
		m_v3CombinedTransform{ 0.0f, 0.0f, 0.0f },
		m_v3RotationAxis{ 0.0f, 0.0f, 0.0f };

	GLfloat m_fDegrees{ 0.0f };

	GLboolean Initialise() override final;

};

