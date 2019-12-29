#version 330

//uniform sampler2D sampler_tex;

out vec4 fragment_colour;

in vec3 v_normal;

in vec2 TexCoord;

void main(void)
{
	vec3 N = normalize(v_normal);
	fragment_colour = vec4(N, 1.0f);

	//fragment_colour = vec4(255.0f, 0.0f, 0.0f, 1.0f);

	//float n = v_normal.x;
	//fragment_colour = vec4(n, n, n, 1.0f);

	//vec3 tex_colour = texture(sampler_tex, TexCoord).rgb;
	//tex_colour *= v_normal;
	//fragment_colour = vec4(tex_colour, 1.0f);

}