#version 400

smooth in vec3 px_normal;
smooth in vec4 half_vec;

in Light {
	vec4 color;
	vec4 dir;
} light;

uniform Material {
	vec4 color;
	float shininess;
	float specular;
	float diffuse;
} material;

out vec4 frag_color;

void main(void)
{
	vec3 normal = normalize(px_normal);
	float specular = pow(max(dot(normal, normalize(half_vec).xyz), 0), material.shininess) * material.specular;
	float diffuse = max(dot(normal, light.dir.xyz), 0) * material.diffuse;
	frag_color = vec4(light.color.rgb * (specular + diffuse), 1) * material.color;
}