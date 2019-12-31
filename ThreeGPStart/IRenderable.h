#pragma once

#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "ImageLoader.h"
#include "Mesh.h"

class IRenderable
{
public:

	IRenderable(const std::string& Filename) : m_strFilename(Filename) {}

	GLuint64EXT GetNumElements() const { return this->m_stNumElements; }
	GLuint GetVAO() const { return this->m_unVAO; }
	GLuint GetTextureID() const { return this->m_unTexture; }

protected:

	std::string m_strFilename{ '\0' },
		m_strImageFilename{ "Data\\Textures\\default.png" };

	GLuint m_unNormalsVBO{ 0 },
		m_unVerticesVBO{ 0 },
		m_unElementsEBO{ 0 },
		m_unUVVBO{ 0 },
		m_unTexture{ 1 },
		m_unVAO{ 0 };

	glm::vec3 m_v3Translation{ 0.0f, 0.0f, 0.0f },
		m_v3CombinedTransform{ 0.0f, 0.0f, 0.0f },
		m_v3RotationAxis{ 0.0f, 0.0f, 0.0f };
	
	glm::mat4 m_m4CombinedTransform{1};

	GLuint64EXT m_stNumElements{ 0 };

	std::vector<glm::vec3> m_vecv3Positions,
		m_vecnNormals;
	std::vector<glm::vec2> m_vecv2UV;
	std::vector<GLuint> m_vecunElements;

	Helpers::ImageLoader m_ilTexture;

	virtual GLboolean Initialise() = 0;

	virtual GLboolean SetBuffers();
	virtual GLboolean BindVao();
	virtual GLboolean SetBufferPointers();
	virtual GLboolean SetTexture();

private:

	virtual GLboolean SetTextureBuffer();
	virtual GLboolean LoadTexture();

};

