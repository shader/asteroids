#version 330
 
in vec3 color;
in vec3 vertex;
in vec3 normal;

smooth out vec4 smooth_color;

uniform mat4 MVP;
void main()
{
   gl_Position = MVP*vec4(vertex,1);
   smooth_color = vec4(normal,1);
   gl_PointSize = 1;
}