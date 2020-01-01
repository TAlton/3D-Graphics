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

	vec3 ambient_light = vec3(0.05, 0.05, 0.0);
	
	vec3 light_position = vec3(0, 25, 0);

	//

	float light_distance = distance(light_position, v_position);
	float light_range = 200.0f;
	float attenuation = 1.0 - smoothstep(0, light_range, light_distance);

	//

	//vec3 light_direction = vec3(-0.9, -0.1, 0); //spot light

	vec3 P = v_position;
	vec3 light_direction = P - light_position; //point light

	vec3 tex_colour = texture(sampler_tex, TexCoord).rgb;
	vec3 L = normalize(light_direction);
	vec3 N = normalize(v_normal);

	float diffuse_intensity = max(0, dot(L,N));

	vec3 light_colour = vec3(1.0, 0.8, 0.6);


	vec3 final_colour = (tex_colour + ambient_light) * diffuse_intensity * light_colour * attenuation;
	//vec3 final_colour = tex_colour * ambient_light + light_colour * diffuse_intensity * attenuation;
	//ambient colour + texcolour * (diffuse + specular) * attenuation

	//tex_colour *= -(v_normal.y);
	fragment_colour = vec4(final_colour, 1.0);

	

}

// attenuated light uses attenuation to multiply diffuse intensity to give a range for the light