#pragma once

#include "ExternalLibraryHeaders.h"
#include "Camera.h"

class Renderer;
struct GLFWwindow;

// Simulation class to handle input, updating of the simulation and calling the renderer

class Simulation
{
private:

	// A simple camera
	std::shared_ptr<Helpers::Camera> m_camera;

	// The renderer
	std::shared_ptr<Renderer> m_renderer;

	// Remember last update time so we can calculate delta time
	float m_lastTime{ 0 };

	//bool for remembering which polygon mode is in use
	GLboolean m_bPolyModeToggle{ false };

	// Handle any user input. Return false if program should close.
	bool HandleInput(GLFWwindow* window);

	//set as GL_LINE
	void SetPolyModeWireframe() const;
	//set as GL_FILL
	void SetPolyModeFill() const;
	//Switches between GL_LINE and GL_FILL
	void TogglePolyMode();

public:
	// Initialise this as well as the renderer, returns false on error
	bool Initialise();	

	// Update the simulation (and render) returns false if program should clse
	bool Update(GLFWwindow* window);
};

