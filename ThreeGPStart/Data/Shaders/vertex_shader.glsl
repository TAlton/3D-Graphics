#version 330

// TODO
uniform mat4 model_xform;
uniform mat4 combined_xform;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv;

out vec3 v_normal;

out vec2 TexCoord;

void main(void)
{	
	v_normal = vertex_normal;
	TexCoord = vertex_uv;
	gl_Position = combined_xform * model_xform * vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1.0);

}