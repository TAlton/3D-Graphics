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

GLboolean Model::Initialise() {



}
