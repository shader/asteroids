#version 400

in vec3 vertex;
out vec4 color;

uniform mat4 MVP;
uniform float age;
uniform vec4 explosion_color;

void main()
{
   gl_Position = MVP*vec4(vertex,1);
   gl_PointSize = 2;
   color = (explosion_color - vec4(0.5) * vertex.z) / age;
}