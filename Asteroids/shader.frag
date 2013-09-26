#version 410

smooth in vec3 px_normal;
smooth in vec4 half_vec;
in vec4 light_color;
in vec4 light_dir;

out vec4 frag_color;

void main(void)
{
	vec3 normal = normalize(px_normal);
	float specular = pow(max(dot(normal, normalize(half_vec).xyz), 0), 32);
	float diffuse = max(dot(normal, light_dir.xyz), 0);
	frag_color = light_color * (specular + diffuse);
}