#include "Terrain.h"

Terrain::Terrain(const std::string& ModelFilename, const std::string& HeightmapFilename, GLuint& VAO, GLint x, GLint y) {

	m_ilImage.Load(ModelFilename);
	m_ilHeightmap.Load(HeightmapFilename);

	m_unNumCellsX = m_ilImage.Width();
	m_unNumCellsZ = m_ilImage.Height();
	
	m_unNumVertsX = m_unNumCellsX + 1;
	m_unNumVertsZ = m_unNumCellsZ + 1;
	m_unNumVerts  = m_unNumCellsX * m_unNumCellsZ; //check if this is right or if it should be vertsX * vertsZ

	m_fVertXToImage = static_cast<GLfloat>(m_ilHeightmap.Width()) / m_unNumVertsX;
	m_fVertZToImage = static_cast<GLfloat>(m_ilHeightmap.Height()) / m_unNumVertsZ;

	m_pbImageData = m_ilHeightmap.GetData();

	Initialise();

}

Terrain::~Terrain()
{
}

GLboolean Terrain::SetNormals() {

		for (auto& x : m_vecunElements) m_vecnNormals.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

	return TRUE;

}

GLboolean Terrain::SetPositions() {

		for (GLuint z{ 0 }; z < m_unNumVertsZ; z++) {

			for (GLuint x{ 0 }; x < m_unNumVertsX; x++) {

				m_vecv3Positions.push_back(glm::vec3(x, 0, -(static_cast<GLint>(z))));

			}

		}

	if (Helpers::CheckForGLError()) return FALSE;

	return TRUE;

}

GLboolean Terrain::SetUV() {

	const GLfloat cfStepX = (1.0f / (m_unNumCellsX - 1)),
		cfStepZ = (1.0f / (m_unNumCellsZ - 1));

	for (GLuint z{ 0 }; z < m_unNumCellsZ; z++) {

		for (GLuint x{ 0 }; x < m_unNumCellsX; x++) {

			m_vecv2UV.push_back(glm::vec2(x * cfStepX, z * cfStepZ));

		}

	}

	if (Helpers::CheckForGLError()) return FALSE;

	return TRUE;

}

void Terrain::CreateMesh() {

	for (GLuint cellZ = 0; cellZ < m_unNumCellsZ; cellZ++) {

		for (GLuint cellX = 0; cellX < m_unNumCellsX; cellX++) {

			GLint vertexIndex = cellZ * m_unNumVertsX + cellX;

			if (m_bInvertPattern) {

				m_vecunElements.push_back(vertexIndex);
				m_vecunElements.push_back(vertexIndex + 1);
				m_vecunElements.push_back(vertexIndex + m_unNumVertsX);

				m_vecunElements.push_back(vertexIndex + 1);
				m_vecunElements.push_back(vertexIndex + m_unNumVertsX + 1);
				m_vecunElements.push_back(vertexIndex + m_unNumVertsX);

			}
			else {

				m_vecunElements.push_back(vertexIndex);
				m_vecunElements.push_back(vertexIndex + 1);
				m_vecunElements.push_back(vertexIndex + m_unNumVertsX + 1);

				m_vecunElements.push_back(vertexIndex + m_unNumVertsX + 1);
				m_vecunElements.push_back(vertexIndex + m_unNumVertsX);
				m_vecunElements.push_back(vertexIndex);

			}

			m_bInvertPattern = !m_bInvertPattern;

		}

		m_bInvertPattern = !m_bInvertPattern;

	}


}

void Terrain::ApplyHeightmap() {

	for (GLuint z{ 0 }; z < m_unNumVertsZ; z++) {
		for (GLuint x{ 0 }; x < m_unNumVertsX; x++) {
			GLfloat imageX = m_fVertXToImage * x;
			GLfloat imageZ = m_fVertZToImage * z;

		

			GLint64 offset = ((GLint64)imageX + (GLint64)imageZ * m_ilHeightmap.Width()) * 4;
			
			GLubyte h = m_pbImageData[offset];

			m_vecv3Positions[m_unCurrentVertex].y = (GLfloat)-h / 2;
			m_unCurrentVertex++;
		}
	}

}

void Terrain::SmoothNormals() {

	for (GLuint64 i{ 0 }; i < m_vecunElements.size(); i += 3) {

		GLuint i1 = m_vecunElements[i];
		GLuint i2 = m_vecunElements[i + 1];
		GLuint i3 = m_vecunElements[i + 2];
		glm::vec3 v0 = m_vecv3Positions[i1];
		glm::vec3 v1 = m_vecv3Positions[i2];
		glm::vec3 v2 = m_vecv3Positions[i3];

		glm::vec3 normal = glm::cross((v1 - v0), (v2 - v0));

		m_vecnNormals[i1] += normal;
		m_vecnNormals[i2] += normal;
		m_vecnNormals[i3] += normal;

	}

	for (GLint i{ 0 }; i < m_vecnNormals.size(); i++) {

		glm::normalize(m_vecnNormals[i]);

	}

}

GLboolean Terrain::Initialise() {

	CreateMesh();
	m_stNumElements = m_vecunElements.size();
	SetNormals();
	SetPositions();
	ApplyHeightmap();
	SetUV();
	SmoothNormals();
	SetBuffers();
	SetTextureBuffer();
	BindVao();
	SetBufferPointers();

	if (Helpers::CheckForGLError()) return FALSE;

	return TRUE;

}

GLboolean Terrain::SetTextureBuffer() {

	glGenTextures(1, &m_unTexture);
	glBindTexture(GL_TEXTURE_2D, m_unTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_ilImage.Width(), m_ilImage.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_ilImage.GetData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);

	if (Helpers::CheckForGLError()) return FALSE;

	return TRUE;

}
