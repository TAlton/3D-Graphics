#include "Renderer.h"
#include "ImageLoader.h"

// On exit must clean up any OpenGL resources e.g. the program, the buffers
Renderer::~Renderer()
{
	glDeleteProgram(m_program);
	glDeleteProgram(m_SkyboxProgram);
	glDeleteBuffers(1, &m_VAO);
}

// Load, compile and link the shaders and create a program object to host them
bool Renderer::CreateProgram()
{
	// Create a new program (returns a unqiue id)
	m_program = glCreateProgram();
	m_SkyboxProgram = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data\\Shaders\\vertex_shader.glsl") };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data\\Shaders\\fragment_shader.glsl") };
	GLuint skybox_vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data\\Shaders\\SkyboxVS.glsl") };
	GLuint skybox_fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data\\Shaders\\SkyboxFS.glsl") };

	if (vertex_shader == 0 || fragment_shader == 0 || 0 == skybox_vertex_shader || 0 == skybox_fragment_shader)
		return false;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(m_program, vertex_shader);
	// Attach the fragment shader (copies it)
	glAttachShader(m_program, fragment_shader);

	// The attibute 0 maps to the input stream "vertex_position" in the vertex shader
	// Not needed if you use (location=0) in the vertex shader itself
	//glBindAttribLocation(m_program, 0, "vertex_position");


	glAttachShader(m_SkyboxProgram, skybox_vertex_shader);
	glAttachShader(m_SkyboxProgram, skybox_fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteShader(skybox_vertex_shader);
	glDeleteShader(skybox_fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(m_program) || !Helpers::LinkProgramShaders(m_SkyboxProgram))
		return false;

	//calculates view and projection matricies
	ComputeViewport();
	ComputeProjectionTransform();
	//loads all renderables
	LoadModels();
	LoadTerrain();
	LoadSkybox();

	return !Helpers::CheckForGLError();
}

GLboolean Renderer::ClearScreen() const {

	// Clear buffers from previous frame
	glClearColor(0.0f, 0.0f, 0.0f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return true;
	
}

GLvoid Renderer::ComputeViewport() {

	GLint viewportSize[4];
	glGetIntegerv(GL_VIEWPORT, viewportSize);
	m_fAspectRatio = viewportSize[2] / (GLfloat)viewportSize[3];
	
}

GLvoid Renderer::ComputeProjectionTransform(GLfloat fov, GLfloat near, GLfloat far) {

	
	m_m4ProjectionTransform = (glm::mat4(
								glm::perspective(
									glm::radians(fov),
									m_fAspectRatio,
									near,
									far)));

}

GLvoid Renderer::UpdateViewTransform(glm::vec3 pos, glm::vec3 look, glm::vec3 up) {

	//calculates the view xform from the cameras properties
	m_m4ViewTransform = glm::lookAt(pos, pos + look, up);
	//calculates the combined transform also reducing amount of functions
	m_m4CombinedTransform = m_m4ProjectionTransform * m_m4ViewTransform;

}

GLvoid Renderer::SetHierarchy() { //possible not needed

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
	//Model* m_mSun = new Model("Data\\Models\\earth.obj", m_VAO, -1, glm::vec3(0, 100, 0));

	vecModel.insert(vecModel.end(), { m_mHull, m_mWingRight, m_mWingLeft, m_mPropeller, m_mGunBase, m_mGun, m_mJeep, /*m_mSun*/ });

	return Helpers::CheckForGLError();

}

GLboolean Renderer::LoadTerrain() {

	Terrain* m_tGrass = new Terrain("Data\\Textures\\grass11.bmp", "Data\\Textures\\curvy.gif", m_VAO, 256, 256, m_unTexID);

	vecTerrain.push_back(m_tGrass);

	return Helpers::CheckForGLError();

}

GLboolean Renderer::LoadSkybox() {
	
	m_Skybox = new Skybox(m_VAO, m_unTexID);

	return Helpers::CheckForGLError();

}

GLvoid Renderer::SetModelTransform(Model& m) { //need to clean this up

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

GLvoid Renderer::SetTerrainTransform() {

	glm::mat4 model_xform = glm::mat4(1);

	GLuint model_xform_id = glGetUniformLocation(m_program, "model_xform");

	glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));

}

GLvoid Renderer::BindTexture(GLuint i) {

	if (-1 == i) return;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, i);
	glUniform1i(glGetUniformLocation(m_program, "sampler_tex"), 0);

}

GLvoid Renderer::DrawSkybox(glm::vec3 pos) {

	glDepthFunc(GL_LEQUAL);

	glUseProgram(m_SkyboxProgram);

	GLuint proj = glGetUniformLocation(m_SkyboxProgram, "projection"); //need a way to put this in a function
	glUniformMatrix4fv(proj, 1, GL_FALSE, glm::value_ptr(m_m4ProjectionTransform));
	GLuint view = glGetUniformLocation(m_SkyboxProgram, "view");
	glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(m_m4ViewTransform));
	glm::mat4 temp = glm::mat4(1);
	temp = glm::translate(temp, pos);
	GLuint model = glGetUniformLocation(m_SkyboxProgram, "model");
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(temp));

	glBindVertexArray(m_Skybox->GetVAO());
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Skybox->GetTextureID());
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthFunc(GL_LESS);

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

	glUniform3f(glGetUniformLocation(m_program, "dLight.direction"), -0.9f, -0.1f, 0.0f);

	glUniform3f(glGetUniformLocation(m_program, "pLight.position"), 100, 100, -100);
	glUniform3f(glGetUniformLocation(m_program, "pLight.light_colour"), 10.0f, 5.0f, 0.0f);
	glUniform1f(glGetUniformLocation(m_program, "pLight.light_range"), 100.0f);

	//glUniform3f(glGetUniformLocation(m_program, "sLight.position"), vecModel[0]->GetTransform().x, vecModel[0]->GetTransform().y, vecModel[0]->GetTransform().z + 5);
	glUniform3f(glGetUniformLocation(m_program, "sLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
	glUniform1f(glGetUniformLocation(m_program, "sLight.inner_angle"), 10.0f);
	glUniform1f(glGetUniformLocation(m_program, "sLight.outer_angle"), 40.0f);

	glUniform3f(glGetUniformLocation(m_program, "sLight.cone_direction"), camera.GetLookVector().x, camera.GetLookVector().y, camera.GetLookVector().z);

	glUniform3f(glGetUniformLocation(m_program, "ambient_light"), 0.2f, 0.025f, 0.015f);

	glm::vec3 temp = camera.GetLookVector();

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

	DrawSkybox(camera.GetPosition());

	// Always a good idea, when debugging at least, to check for GL errors
	Helpers::CheckForGLError();

}

GLvoid Renderer::MoveBoat(glm::vec3 v) {

	for (auto& x : vecModel) {

		x->Translate(v);

	}

}

/*

todo

perlin noise

headlights (this is just a spot light, that is placed on the model and rotates and moves with it

*/