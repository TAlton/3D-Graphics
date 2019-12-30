#include "Model.h"

Model::Model(const std::string& ModelFilename, GLuint VAO, GLint ParentIndex, glm::vec3 Translation, glm::vec3 Axis, GLfloat Degrees) : IRenderable(ModelFilename) {

	m_strFilename = ModelFilename;
	m_unVAO = VAO;
	m_nParentID = ParentIndex;
	m_v3Translation = Translation;
	m_v3RotationAxis = Axis;
	m_fDegrees = Degrees;

	Initialise();

}

Model::~Model()
{
}

void Model::SetTransform() {

	this->m_v3CombinedTransform += m_v3Translation;

}

GLint Model::GetParent() const {

	return this->m_nParentID;

}

void Model::Translate(glm::vec3 xform) {

	this->m_v3CombinedTransform += xform;

}

void Model::Translate(GLint x, GLint y, GLint z) {

	this->m_v3CombinedTransform += glm::vec3(x, y, z);

}

GLboolean Model::Initialise() {

	LoadMesh();
	SetTransform();
	SetVectors();
	SetBuffers();
	BindVao();
	SetBufferPointers();

	return !Helpers::CheckForGLError();

}

GLboolean Model::SetVectors() {
	
	m_vecv3Positions = m_mlModel.GetMeshVector()[0].vertices;
	m_vecnNormals = m_mlModel.GetMeshVector()[0].normals;
	m_vecv2UV = m_mlModel.GetMeshVector()[0].uvCoords;
	m_vecunElements = m_mlModel.GetMeshVector()[0].elements;

	m_stNumElements = m_vecunElements.size();

	if (0 == m_vecv3Positions.size()) return FALSE;
	if (0 == m_vecnNormals.size()) return FALSE;
	if (0 == m_vecv2UV.size()) return FALSE;
	if (0 == m_vecunElements.size()) return FALSE;

	return TRUE;

}

GLboolean Model::LoadMesh() {

	return m_mlModel.LoadFromFile(m_strFilename);

}
