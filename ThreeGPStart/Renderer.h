#pragma once

#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "ImageLoader.h"
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

	bool CreateProgram();
	//clears the screen to a colour
	GLboolean ClearScreen() const;
	//computes the viewport size for use in projection matricies
	void ComputeViewport();
	//Computes the projection matrix;
	void ComputeProjectionTransform(GLfloat fov = 45.0f, GLfloat near = 1.0f, GLfloat far = 2000.0f);
	//calculates the view transform and the combined transform from the cameras position and where it is looking
	void UpdateViewTransform(glm::vec3 pos, glm::vec3 look, glm::vec3 up);
	//creates information for all uniform variables

	Model m_Hull{ "Data\\Models\\AquaPig\\hull.obj", m_VAO, -1, glm::vec3(0.0f, 0.0f, 0.0f) },
		m_mWingRight{ "Data\\Models\\AquaPig\\wing_right.obj", m_VAO, 0, glm::vec3(-2.231f, 0.272f, -2.663f) },
		m_mWingLeft{ "Data\\Models\\AquaPig\\wing_left.obj", m_VAO, 0, glm::vec3(2.231f, 0.272f, -2.663f) },
		m_mPropeller{ "Data\\Models\\AquaPig\\propeller.obj", m_VAO, 0, glm::vec3(0.0f, 0.272f, -2.663f), 90.f, glm::vec3(1.0f, 0.0f, 0.0f) },
		m_mGunBase{ "Data\\Models\\AquaPig\\gun_base.obj", m_VAO, 0, glm::vec3(0.0f, 0.569f, -1.866f) },
		m_mGun{ "Data\\Models\\AquaPig\\gun.obj", m_VAO, 4, glm::vec3(0.0f, 1.506f, 0.644f) };

	Terrain m_tGrass{ "Data\\Textures\\grass11.bmp", "Data\\Textures\\curvy.gif", m_VAO, 2, 2 };

	std::vector<IRenderable*> vecRenderables{ m_tGrass, m_Hull, m_mWingRight, m_mWingLeft, m_mPropeller, m_mGunBase, m_mGun }

public:

	Renderer()=default;
	~Renderer();

	// Create and / or load geometry, this is like 'level load'
	bool InitialiseGeometry();

	// Render the scene
	void Render(const Helpers::Camera& camera, float deltaTime);
};

