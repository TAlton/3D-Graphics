#version 330

uniform sampler2D sampler_tex;

out vec4 fragment_colour;

in vec3 v_normal;
in vec3 v_position;
in vec2 TexCoord;

void main(void)
{
	//fragment_colour = vec4(N, 1.0f);

	//fragment_colour = vec4(255.0f, 0.0f, 0.0f, 1.0f);

	//float n = v_normal.x;
	//fragment_colour = vec4(n, n, n, 1.0f);
	vec3 light_position = vec3(0, 400, 0);

	//vec3 light_direction = vec3(-0.9, -0.1, 0); //spot light

	vec3 P = v_position;
	vec3 light_direction = P - light_position; //point light

	vec3 tex_colour = texture(sampler_tex, TexCoord).rgb;
	vec3 L = normalize(light_direction);
	vec3 N = normalize(v_normal);

	float diffuse_intensity = max(0, dot(L,N));

	vec3 final_colour = tex_colour * diffuse_intensity;

	//tex_colour *= -(v_normal.y);
	fragment_colour = vec4(final_colour, 1.0);

}