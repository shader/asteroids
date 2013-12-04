#version 400

in vec3 vertex;
out vec4 color;

uniform mat4 MVP;
uniform float age;
uniform vec4 explosion_color;

void main()
{
   gl_Position = MVP*vec4(vertex,1);
   gl_PointSize = -vertex.z;
   color = (explosion_color - (explosion_color * vertex.z / 3)) / pow(age,2);
}