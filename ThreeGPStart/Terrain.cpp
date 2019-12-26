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

void Terrain::Initialise() {



}
