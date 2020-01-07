#pragma once

#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "ImageLoader.h"
#include "Mesh.h"

class Skybox {

public:

	Skybox(GLuint VAO, GLuint TexID) : m_unVAO(VAO), m_unTexID(TexID) { CreateSkybox(); }

	GLuint GetVAO() const { return this->m_unVAO; }
	GLuint GetTextureID() const { return this->m_unTexID; }

private:

	GLuint m_unVAO{ 0 },
		m_unTexID{ 0 };

	GLboolean CreateSkybox();

	std::vector<std::string> m_vecstrTextures{

		"Data\\Sky\\Sky\\right.jpg",
		"Data\\Sky\\Sky\\left.jpg",
		"Data\\Sky\\Sky\\top.jpg",
		"Data\\Sky\\Sky\\bottom.jpg",
		"Data\\Sky\\Sky\\front.jpg",
		"Data\\Sky\\Sky\\back.jpg"

	};

};

