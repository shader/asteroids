#version 330
 
in vec3 color;
in vec3 vertex;
in vec3 normal;

smooth out vec4 px_normal;
smooth out vec4 half_vec;
out vec4 light_color;
out vec4 light_dir;

uniform mat4 MVP;
uniform mat4 model;
uniform mat4 normal_matrix;
uniform mat4 inv_model;
uniform mat4 inv_view;

void main()
{
   gl_Position = MVP*vec4(vertex,1);

   px_normal = normal_matrix*vec4(normal,1);
   light_dir = vec4(1);
   light_color = vec4(1);
   vec4 eye = -normalize(inv_view*vec4(0,0,10,1) + model*vec4(vertex,1));
   half_vec = normalize(light_dir - eye);

   gl_PointSize = 1;
}