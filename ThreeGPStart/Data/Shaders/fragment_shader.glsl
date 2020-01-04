#version 330

uniform sampler2D sampler_tex;

out vec4 fragment_colour;

in vec3 v_normal;
in vec3 v_position;
in vec2 TexCoord;

struct DirectionalLight {

	vec3 direction;

};

struct PointLight {
	
	vec3 position;
	vec3 light_colour;
	float light_range;

};

struct SpotLight {

	vec3 position;
	float angle;
	vec3 cone_direction;

};

uniform vec3 ambient_light;
uniform SpotLight sLight;
uniform PointLight pLight;
uniform DirectionalLight dLight;

float CalcDirectionalLight(vec3 N) {

	vec3 L = normalize(-dLight.direction);
	float diffuse_intensity = max(0, dot(L,N));

	return diffuse_intensity;

}

vec3 CalcPointLight() {

	vec3 light_direction = normalize(pLight.position - v_position);
	float diffuse = max(dot(v_normal, light_direction), 0.0);

	float light_distance = length(pLight.position - v_position);
	float attenuation = 1.0  - smoothstep(0, pLight.light_range, light_distance);


	return pLight.light_colour * (diffuse * attenuation);

}

vec3 CalcSpotLight() {

	vec3 lightDir = normalize(sLight.position - v_position);
	float theta = dot(lightDir, -normalize(sLight.cone_direction));
	float attenuation = 1.0  - smoothstep(0, 100, length(sLight.position - v_position));

	float fc = smoothstep(cos(0.5 * sLight.angle), 1, dot(-lightDir, sLight.cone_direction));

	if(theta < sLight.angle) {

	 return vec3(0,0,10) * attenuation;

	}

}

void main(void)
{
	//fragment_colour = vec4(N, 1.0f);

	//fragment_colour = vec4(255.0f, 0.0f, 0.0f, 1.0f);

	//float n = v_normal.x;
	//fragment_colour = vec4(n, n, n, 1.0f);
//
//	vec3 ambient_light = vec3(0.05, 0.05, 0.0);
//	
//	vec3 light_position = vec3(0, 25, 0);
//
//	//
//
//	float light_distance = distance(light_position, v_position);
//	float light_range = 200.0f;
//	float attenuation = 1.0;// - smoothstep(0, light_range, light_distance);
//
//	//
//
//	//vec3 light_direction = vec3(-0.9, -0.1, 0); //spot light
//
//	vec3 P = v_position;
//	vec3 light_direction = P - light_position; //point light
//
//	vec3 tex_colour = texture(sampler_tex, TexCoord).rgb;
//	vec3 L = normalize(-light_direction);
//	vec3 N = normalize(v_normal);
//
//	float diffuse_intensity = max(0, dot(L,N));
//
//	vec3 light_colour = vec3(1.0, 0.8, 0.6);
//
//
//	vec3 final_colour = (tex_colour + ambient_light) * diffuse_intensity * light_colour * attenuation;
//	//vec3 final_colour = tex_colour * ambient_light + light_colour * diffuse_intensity * attenuation;
//	//ambient colour + texcolour * (diffuse + specular) * attenuation
//
//	//tex_colour *= -(v_normal.y);
//	fragment_colour = vec4(final_colour, 1.0);

	vec3 tex_colour = texture(sampler_tex, TexCoord).rgb;
	vec3 N = normalize(v_normal);

	vec3 final_colour = (ambient_light + tex_colour) * (CalcDirectionalLight(N) + CalcPointLight() + CalcSpotLight());

	fragment_colour = vec4(final_colour, 1.0);

}
