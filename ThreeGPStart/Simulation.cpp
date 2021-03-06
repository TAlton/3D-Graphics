#include "Simulation.h"
#include "Renderer.h"

// Initialise this as well as the renderer, returns false on error
bool Simulation::Initialise()
{
	// Set up camera
	m_camera = std::make_shared<Helpers::Camera>();
	//m_camera->Initialise(glm::vec3(0, 200, 900), glm::vec3(0)); // Jeep
	//m_camera->Initialise(glm::vec3(-13.82f, 5.0f, 1.886f), glm::vec3(0.25f, 1.5f, 0), 30.0f,0.8f); // Aqua pig
	m_camera->Initialise(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	// Set up renderer
	m_renderer = std::make_shared<Renderer>();
	return m_renderer->InitialiseGeometry();
}

// Handle any user input. Return false if program should close.
bool Simulation::HandleInput(GLFWwindow* window)
{
	// You can get keypresses like this:
	// if (glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS) // means W key pressed

	// You can get mouse button input, returned state is GLFW_PRESS or GLFW_RELEASE
	// int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

	// Use this to get the mouse position:
	// double xpos, ypos;
	// glfwGetCursorPos(window, &xpos, &ypos);

	// To prevent the mouse leaving the window (and to hide the arrow) you can call:
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// To reenable it use GLFW_CURSOR_NORMAL

	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_F1)) SetPolyModeFill();
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_F2)) SetPolyModeWireframe();
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_Y)) MoveObject(0, 0, 1);
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_G)) MoveObject(1);
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_H)) MoveObject(0, 0, -1);
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_J)) MoveObject(-1);
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_T)) MoveObject(0, 1, 0);
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_U)) MoveObject(0, -1, 0);


	//if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_Y)) MoveObject();

	// To see an example of input using GLFW see the camera.cpp file.
	return true;
}

void Simulation::SetPolyModeWireframe() const {

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
}

void Simulation::SetPolyModeFill() const {

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void Simulation::MoveObject(GLint x, GLint y, GLint z) {

	if (m_fTime < 100) {
		m_fTime = 0;
		return;
	}

	m_renderer->MoveBoat(glm::vec3(x, y, z));

}

// Update the simulation (and render) returns false if program should close
bool Simulation::Update(GLFWwindow* window)
{
	// Deal with any input
	if (!HandleInput(window))
		return false;

	// Calculate delta time since last called
	// We pass the delta time to the camera and renderer
	float timeNow = (float)glfwGetTime();
	float deltaTime{ timeNow - m_lastTime };
	m_lastTime = timeNow;
	m_fTime += m_lastTime; //for input handling

	// The camera needs updating to handle user input internally
	m_camera->Update(window, deltaTime);

	// Render the scene
	m_renderer->Render(*m_camera, deltaTime);

	return true;
}
