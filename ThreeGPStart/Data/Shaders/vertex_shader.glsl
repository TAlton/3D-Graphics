#version 330

// TODO
uniform mat4 model_xform;
uniform mat4 combined_xform;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv;

out vec3 v_normal;
out vec3 v_position;
out vec2 TexCoord;

void main(void) {	

	TexCoord = vertex_uv;
	v_normal = mat3(model_xform) * vertex_normal;
	v_position = mat4x3(model_xform) * vec4(vertex_position, 1.0);
	gl_Position = combined_xform * model_xform * vec4(vertex_position, 1.0);

}