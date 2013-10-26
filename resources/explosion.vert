#version 400

in vec3 vertex;
out vec4 color;

uniform mat4 MVP;
uniform float age;

void main()
{
   gl_Position = MVP*vec4(vertex,1);
   gl_PointSize = 2;
   color = vec4(1 / age,0,0,1);
}