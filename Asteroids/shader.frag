#version 330

smooth in vec4 px_normal;
smooth in vec4 half_vec;
in vec4 light_color;
in vec4 light_dir;

out vec4 frag_color;

void main(void)
{
	//float specular = pow(max(dot(px_normal, half_vec), 0));
	float diffuse = max(dot(px_normal, light_dir), 0);
	frag_color = light_color * diffuse; //(specular + diffuse);
}