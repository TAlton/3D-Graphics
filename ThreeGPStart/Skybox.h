#pragma once

#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "ImageLoader.h"
#include "Mesh.h"

class Skybox {

	Skybox(GLuint VAO) : m_unVAO(VAO) {}

private:

	GLuint m_unVAO{ 0 };

	GLboolean CreateCube();



};

