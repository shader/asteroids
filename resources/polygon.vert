#version 400

in vec3 vertex;
out vec4 color;

uniform mat4 MVP;
uniform vec4 uni_color;
uniform int point_size = 1;

void main()
{
   gl_Position = MVP*vec4(vertex,1);
   gl_PointSize = point_size;
   color = uni_color;
}