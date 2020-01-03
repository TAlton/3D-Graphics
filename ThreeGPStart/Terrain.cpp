#include "Terrain.h"

Terrain::Terrain(const std::string& ModelFilename, const std::string& HeightmapFilename, GLuint VAO, GLint x, GLint y, GLint TexID) : IRenderable(ModelFilename) {

	m_ilHeightmap.Load(HeightmapFilename);

	m_strImageFilename = ModelFilename;

	m_unNumCellsX = x;
	m_unNumCellsZ = y;
	
	m_unNumVertsX = m_unNumCellsX + 1;
	m_unNumVertsZ = m_unNumCellsZ + 1;
	m_unNumVerts  = m_unNumCellsX * m_unNumCellsZ; //check if this is right or if it should be vertsX * vertsZ

	m_fVertXToImage = static_cast<GLfloat>(m_ilHeightmap.Width()) / m_unNumVertsX;
	m_fVertZToImage = static_cast<GLfloat>(m_ilHeightmap.Height()) / m_unNumVertsZ;

	m_unTexture = TexID;	

	m_pcImageData = m_ilHeightmap.GetData();

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

		return !Helpers::CheckForGLError();

}

GLboolean Terrain::SetUV() {

	const GLfloat cfStepX = (32.0f / (m_unNumCellsX - 1)),
		cfStepZ = (32.0f / (m_unNumCellsZ - 1));

	for (GLuint z{ 0 }; z < m_unNumCellsZ; z++) {

		for (GLuint x{ 0 }; x < m_unNumCellsX; x++) {

			m_vecv2UV.push_back(glm::vec2(x * cfStepX, z * cfStepZ));

		}

	}

	return !Helpers::CheckForGLError();

}

GLvoid Terrain::CreateMesh() {

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

GLvoid Terrain::ApplyHeightmap() {

	for (GLuint z{ 0 }; z < m_unNumVertsZ; z++) {

		for (GLuint x{ 0 }; x < m_unNumVertsX; x++) {

			GLfloat imageX = m_fVertXToImage * x;
			GLfloat imageZ = m_fVertZToImage * z;

			GLint64 offset = ((GLint64)imageX + (GLint64)imageZ * m_ilHeightmap.Width()) * 4;		

			GLubyte h = m_pcImageData[offset];

			m_vecv3Positions[m_unCurrentVertex].y = (GLfloat)h / 4;
			m_unCurrentVertex++;

		}

	}

}

GLvoid Terrain::SmoothNormals() {

	for (GLuint64 i{ 0 }; i < m_vecunElements.size(); i += 3) {

		GLuint i1 = m_vecunElements[i];
		GLuint i2 = m_vecunElements[i + 1];
		GLuint i3 = m_vecunElements[i + 2];
		glm::vec3 v0 = m_vecv3Positions[i1];
		glm::vec3 v1 = m_vecv3Positions[i2];
		glm::vec3 v2 = m_vecv3Positions[i3];

		glm::vec3 normal = glm::normalize(glm::cross((v1 - v0), (v2 - v0)));

		m_vecnNormals[i1] += normal;
		m_vecnNormals[i2] += normal;
		m_vecnNormals[i3] += normal;

	}

	for (auto& n : m_vecnNormals) n = glm::normalize(n);

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
	SetTexture();
	BindVao();
	SetBufferPointers();

	return !Helpers::CheckForGLError();

}
