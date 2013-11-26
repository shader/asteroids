#version 400

smooth in vec3 px_normal;
smooth in vec4 half_vec;

in Light {
	vec4 color;
	vec4 dir;
} light;

layout (std140) uniform Material {
	vec4 color;
	vec4 emission;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
} material;

out vec4 frag_color;

void main(void)
{
	vec3 normal = normalize(px_normal);
	vec4 specular = pow(max(dot(normal, normalize(half_vec).xyz), 0), material.shininess) * material.specular;
	vec4 diffuse = max(dot(normal, light.dir.xyz), 0) * material.diffuse;
	vec4 ambient = material.ambient;
	frag_color = vec4(light.color.rgb * (specular + diffuse + ambient).rgb, 1) * material.color + material.emission;
}