#include "Terrain.h"

Terrain::Terrain(const std::string& ModelFilename, const std::string& HeightmapFilename, GLuint& VAO, GLint x, GLint y) {

	m_ilImage.Load(ModelFilename);
	m_ilHeightmap.Load(HeightmapFilename);

	m_unNumCellsX = m_ilImage.Width();
	m_unNumCellsZ = m_ilImage.Height();
	
	m_unNumVertsX = m_unNumCellsX + 1;
	m_unNumVertsZ = m_unNumCellsZ + 1;
	m_unNumVerts  = m_unNumCellsX * m_unNumCellsZ; //check if this is right or if it should be vertsX * vertsZ

	m_fVertXToImage = static_cast<float>(m_ilHeightmap.Width()) / m_unNumVertsX;
	m_fVertZToImage = static_cast<float>(m_ilHeightmap.Height()) / m_unNumVertsZ;

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

	for (int z{ 0 }; z < m_unNumVertsZ; z++) {

		for (int x{ 0 }; x < m_unNumVertsX; x++) {

			m_vecv3Positions.push_back(glm::vec3(x, 0, -z));

		}

	}

	return TRUE;

}

GLboolean Terrain::SetUV() {

	const GLfloat cfStepX = (1.0f / (m_unNumCellsX - 1)),
		cfStepZ = (1.0f / (m_unNumCellsZ - 1));

	for (int z{ 0 }; z < m_unNumCellsZ; z++) {

		for (int x{ 0 }; x < m_unNumCellsX; x++) {

			m_vecv2UV.push_back(glm::vec2(x * cfStepX, z * cfStepZ));

		}

	}

}

GLboolean Terrain::Initialise() {

	/*
	
	create mesh
	set num elements
	set normals
	set positions
	apply heightmap
	smooth normals
	set buffers
	bind vao
	set buffer pointers

	*/
	SetNormals();
	SetPositions();
	SetUV();

}
