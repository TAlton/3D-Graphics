#include "Model.h"

Model::Model(const std::string& ModelFilename, const std::string& TextureFilename, GLuint& VAO, GLint ParentIndex, glm::vec3 Translation, glm::vec3 Axis, GLfloat Degrees) {

	this->m_strFilename = ModelFilename;
	this->m_strImageFilename = TextureFilename;
	this->m_unVAO = VAO;
	this->m_unParentID = ParentIndex;
	this->m_v3Translation = Translation;
	this->m_v3RotationAxis = Axis;
	this->m_fDegrees = Degrees;

	try {

		m_mlModel.LoadFromFile(m_strFilename);

	}
	catch (std::runtime_error& e) {

		throw e;

	}
	catch (std::invalid_argument& e) {

		std::cerr << e.what() << std::endl;
		throw e;

	}

}

Model::~Model()
{
}

void Model::Translate(glm::vec3 xform) {

	this->m_v3Translation += xform;

}

void Model::Translate(GLint x, GLint y, GLint z) {

	this->m_v3Translation.x += x;
	this->m_v3Translation.y += y;
	this->m_v3Translation.z += z;

}

GLboolean Model::Initialise() {

	/*
	
	load mesh
	set xfrom
	set vectors
	set buffers 
	bind vao
	set buffer pointers

	*/

}

GLboolean Model::SetVectors() {
	
	m_vecv3Positions = m_mlModel.GetMeshVector()[0].vertices;
	m_vecnNormals = m_mlModel.GetMeshVector()[0].normals;
	m_vecv2UV = m_mlModel.GetMeshVector()[0].uvCoords;
	m_vecunElements = m_mlModel.GetMeshVector()[0].elements;

	m_unNumElements = m_vecunElements.size();

	if (0 == m_vecv3Positions.size()) return FALSE;
	if (0 == m_vecnNormals.size()) return FALSE;
	if (0 == m_vecv2UV.size()) return FALSE;
	if (0 == m_vecunElements.size()) return FALSE;

	return TRUE;

}
