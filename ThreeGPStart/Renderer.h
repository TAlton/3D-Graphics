#pragma once

#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"
#include "Terrain.h"
#include "Model.h"

class Renderer
{
private:
	// Program object - to host shaders
	GLuint m_program{ 0 };

	// Vertex Array Object to wrap all render settings
	GLuint m_VAO{ 0 };

	// Number of elments to use when rendering
	GLuint m_numElements{ 0 };

	GLfloat m_fAspectRatio{ 0 };

	glm::mat4 m_m4ProjectionTransform{ 0 },
		m_m4ViewTransform{ 0 },
		m_m4CombinedTransform{ 0 };

	std::vector<GLint>

	bool CreateProgram();
	//clears the screen to a colour
	bool ClearScreen() const;
	//computes the viewport size for use in projection matricies
	void ComputeViewport();
	//Computes the projection matrix;
	void ComputeProjectionTransform(GLfloat fov = 45.0f, GLfloat near = 1.0f, GLfloat far = 2000.0f);
	//calculates the view transform and the combined transform from the cameras position and where it is looking
	void UpdateViewTransform(glm::vec3 pos, glm::vec3 look, glm::vec3 up);

	void GenerateUniformLocations();

public:

	Renderer()=default;
	~Renderer();

	// Create and / or load geometry, this is like 'level load'
	bool InitialiseGeometry();

	// Render the scene
	void Render(const Helpers::Camera& camera, float deltaTime);
};

