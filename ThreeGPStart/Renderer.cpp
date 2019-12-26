#include "Renderer.h"
#include "ImageLoader.h"

// On exit must clean up any OpenGL resources e.g. the program, the buffers
Renderer::~Renderer()
{
	glDeleteProgram(m_program);	
	glDeleteBuffers(1, &m_VAO);
}

// Load, compile and link the shaders and create a program object to host them
bool Renderer::CreateProgram()
{
	// Create a new program (returns a unqiue id)
	m_program = glCreateProgram();

	ComputeViewport();
	ComputeProjectionTransform();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/vertex_shader.glsl") };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/fragment_shader.glsl") };
	if (vertex_shader == 0 || fragment_shader == 0)
		return false;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(m_program, vertex_shader);

	// The attibute 0 maps to the input stream "vertex_position" in the vertex shader
	// Not needed if you use (location=0) in the vertex shader itself
	//glBindAttribLocation(m_program, 0, "vertex_position");

	// Attach the fragment shader (copies it)
	glAttachShader(m_program, fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(m_program))
		return false;

	return !Helpers::CheckForGLError();
}

GLboolean Renderer::ClearScreen() const {

	// Clear buffers from previous frame
	glClearColor(0.0f, 0.0f, 0.0f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return true;
	
}

void Renderer::ComputeViewport() {

	GLint viewportSize[4];
	glGetIntegerv(GL_VIEWPORT, viewportSize);
	m_fAspectRatio = viewportSize[2] / (GLfloat)viewportSize[3];
	
}

void Renderer::ComputeProjectionTransform(GLfloat fov, GLfloat near, GLfloat far) {

	
	m_m4ProjectionTransform = (glm::mat4(
								glm::perspective(
									glm::radians(fov),
									m_fAspectRatio,
									near,
									far)));

}

void Renderer::UpdateViewTransform(glm::vec3 pos, glm::vec3 look, glm::vec3 up) {

	//calculates the view xform from the cameras properties
	m_m4ViewTransform = glm::lookAt(pos, pos + look, up);
	//calculates the combined transform also reducing amount of functions
	m_m4CombinedTransform = m_m4ProjectionTransform * m_m4ViewTransform;

}

// Load / create geometry into OpenGL buffers	
bool Renderer::InitialiseGeometry()
{
	// Load and compile shaders into m_program
	if (!CreateProgram())
		return false;

	// TODO - load mesh using the Helpers::ModelLoader class

	// Good idea to check for an error now:	
	Helpers::CheckForGLError();

	// Clear VAO binding
	glBindVertexArray(0);

	return true;
}

// Render the scene. Passed the delta time since last called.
void Renderer::Render(const Helpers::Camera& camera, float deltaTime)
{		
	// Configure pipeline settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	ClearScreen();
	UpdateViewTransform(camera.GetPosition(), camera.GetLookVector(), camera.GetUpVector());

	//Use oour program. Doing this enables the shaders we attached
	glUseProgram(m_program);

	GLuint combined_xform_id = glGetUniformLocation(m_program, "combined_xform"); //need a way to put this in a function
	glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(m_m4CombinedTransform));

	// Always a good idea, when debugging at least, to check for GL errors
	Helpers::CheckForGLError();
}
