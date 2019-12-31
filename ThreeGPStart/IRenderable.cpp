#include "IRenderable.h"

GLboolean IRenderable::SetBuffers() {

	glGenBuffers(1, &m_unVerticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_unVerticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vecv3Positions.size(), m_vecv3Positions.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_unNormalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_unNormalsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vecnNormals.size(), m_vecnNormals.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_unUVVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_unUVVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m_vecv2UV.size(), m_vecv2UV.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_unElementsEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unElementsEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_vecunElements.size(), m_vecunElements.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return !Helpers::CheckForGLError();

}

GLboolean IRenderable::BindVao() {

		glGenVertexArrays(1, &m_unVAO);
		glBindVertexArray(m_unVAO);

		return !Helpers::CheckForGLError();

}

GLboolean IRenderable::SetBufferPointers() {

		glBindBuffer(GL_ARRAY_BUFFER, m_unVerticesVBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glBindBuffer(GL_ARRAY_BUFFER, m_unNormalsVBO);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glBindBuffer(GL_ARRAY_BUFFER, m_unUVVBO);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(
			2,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unElementsEBO);
	Helpers::CheckForGLError();

	glBindVertexArray(0);

	return TRUE;

}

GLboolean IRenderable::SetTexture() {

	if (LoadTexture()) {

		return SetTextureBuffer();

	}

	return FALSE;

}

GLboolean IRenderable::SetTextureBuffer() {

	if (-1 == this->m_unTexture) return FALSE;

	glGenTextures(1, &m_unTexture);
	glBindTexture(GL_TEXTURE_2D, m_unTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_ilTexture.Width(), m_ilTexture.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_ilTexture.GetData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);

	return !Helpers::CheckForGLError();

}

GLboolean IRenderable::LoadTexture() {

	return m_ilTexture.Load(m_strImageFilename);

}
