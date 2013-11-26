#version 400

in vec3 color;
in vec3 vertex;
in vec3 normal;

smooth out vec3 px_normal;
smooth out vec4 half_vec;

out Light {
	vec4 color;
	vec4 dir;
} light_out;

uniform mat4 MVP;
uniform mat4 model_view;
uniform mat3 normal_matrix;
uniform mat4 inv_view;

void main()
{
   gl_Position = MVP*vec4(vertex,1);

   px_normal = normal_matrix*normal;
   
   light_out.dir = vec4(1,1,2,0);
   light_out.color = vec4(vec3(.75),1);

   vec4 eye = normalize(inv_view*vec4(0,0,0,1) - model_view*vec4(vertex,1));
   half_vec = normalize(light_out.dir - eye);
}