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

	//calculates view and projection matricies
	ComputeViewport();
	ComputeProjectionTransform();
	//loads all renderables
	LoadModels();
	LoadTerrain();

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

void Renderer::SetHierarchy() { //possible not needed

	for (auto& x : vecModel) {

		if (-1 == x->GetParent()) continue;

		x->Translate(vecModel[x->GetParent()]->GetTransform());
		
	}

}

GLboolean Renderer::LoadModels() {

	Model* m_mHull = new Model("Data\\Models\\AquaPig\\hull.obj", m_VAO,  -1, glm::vec3(0.0f, 0.0f, 0.0f));
	Model* m_mWingRight = new Model("Data\\Models\\AquaPig\\wing_right.obj", m_VAO, 0, glm::vec3(-2.231f, 0.272f, -2.663f));
	Model* m_mWingLeft = new Model("Data\\Models\\AquaPig\\wing_left.obj", m_VAO,  0, glm::vec3(2.231f, 0.272f, -2.663f));
	Model* m_mPropeller = new Model("Data\\Models\\AquaPig\\propeller.obj", m_VAO,  0, glm::vec3(0.0f, 1.395f, -3.616f), glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
	Model* m_mGunBase = new Model("Data\\Models\\AquaPig\\gun_base.obj", m_VAO,  0, glm::vec3(0.0f, 0.569f, -1.866f));
	Model* m_mGun = new Model("Data\\Models\\AquaPig\\gun.obj", m_VAO, 4,  glm::vec3(0.0f, 1.506f, 0.644f));
	Model* m_mJeep = new Model("Data\\Models\\Jeep\\jeep.obj", m_VAO, "Data\\Models\\Jeep\\jeep_army.jpg", m_unTexID);
	Model* m_mSun = new Model("Data\\Models\\earth.obj", m_VAO, -1, glm::vec3(0, 100, 0));

	vecModel.insert(vecModel.end(), { m_mHull, m_mWingRight, m_mWingLeft, m_mPropeller, m_mGunBase, m_mGun, m_mJeep, m_mSun });

	return Helpers::CheckForGLError();

}

GLboolean Renderer::LoadTerrain() {

	Terrain* m_tGrass = new Terrain("Data\\Textures\\grass11.bmp", "Data\\Textures\\curvy.gif", m_VAO, 256, 256, m_unTexID);

	vecTerrain.push_back(m_tGrass);

	return Helpers::CheckForGLError();

}

void Renderer::SetModelTransform(Model& m) { //need to clean this up

	glm::mat4 model_xform = glm::mat4(1);

	model_xform = glm::translate(model_xform, m.GetTransform());

	if (m.GetDegrees() > 0.1f) { //TRS
		
		deg += 2.5f;
		if (deg > 358.0f) deg = 0.0f;
		model_xform = glm::rotate(model_xform, glm::radians(m.GetDegrees()), m.GetAxis());
		model_xform = glm::rotate(model_xform, glm::radians(deg), glm::vec3(0, 1, 0));

	}

	GLuint model_xform_id = glGetUniformLocation(m_program, "model_xform");

	glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));

}

void Renderer::SetTerrainTransform() {

	glm::mat4 model_xform = glm::mat4(1);

	GLuint model_xform_id = glGetUniformLocation(m_program, "model_xform");

	glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));

}

void Renderer::BindTexture(GLuint i) {

	if (-1 == i) return;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, i);
	glUniform1i(glGetUniformLocation(m_program, "sampler_tex"), 0);

}

// Load / create geometry into OpenGL buffers	
bool Renderer::InitialiseGeometry()
{
	// Load and compile shaders into m_program
	if (!CreateProgram())
		return false;

	SetHierarchy();

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

	//Use our program. Doing this enables the shaders we attached
	glUseProgram(m_program);

	GLuint combined_xform_id = glGetUniformLocation(m_program, "combined_xform"); //need a way to put this in a function
	glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(m_m4CombinedTransform));

	for (auto& x : vecTerrain) { //draws all terrain
		//move draw into a function
		BindTexture(x->GetTextureID());
		SetTerrainTransform();
		glBindVertexArray(x->GetVAO());
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(x->GetNumElements()), GL_UNSIGNED_INT, (void*)0);

	}

	for (auto& x : vecModel) { //draws all models

		BindTexture(x->GetTextureID());
		SetModelTransform(*x);
		glBindVertexArray(x->GetVAO());
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(x->GetNumElements()), GL_UNSIGNED_INT, (void*)0);

	}

	// Always a good idea, when debugging at least, to check for GL errors
	Helpers::CheckForGLError();
}

void Renderer::MoveBoat(glm::vec3 v) {

	for (auto& x : vecModel) {

		x->Translate(v);

	}

}

/*

todo

skybox
---create and draw square
---make square follow camera
------draw texture on inside of the cube

multiple lights
---create light struct
------pass lights into

perlin noise

headlights (this is just a spot light, that is placed on the model and rotates and moves with it

*/