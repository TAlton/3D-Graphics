#include "Model.h"

Model::Model(const std::string& ModelFilename, const std::string& TextureFilename, GLuint& VAO, GLint ParentIndex, glm::vec3 Translation, glm::vec3 Axis, GLfloat Degrees) {

	this->m_strFilename = ModelFilename;
	this->m_strImageFilename = TextureFilename;
	this->m_unVAO = VAO;
	this->m_unParentID = ParentIndex;
	this->m_v3Translation = Translation;
	this->m_v3RotationAxis = Axis;
	this->m_fDegrees = Degrees;

}

Model::~Model()
{
}

void Model::Initialise() {



}
