#version 400
 
in vec3 vertex;
in vec2 texcoord;

smooth out vec2 UV;
uniform mat4 MVP;

void main()
{
   gl_Position = MVP*vec4(vertex,1);

   UV = texcoord;
}