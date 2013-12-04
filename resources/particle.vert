#version 400

in vec3 vertex;
out vec4 color;

uniform mat4 MVP;
uniform ParticleSystem {
	vec4 start_color;
	vec4 end_color;
	float age;
	float duration;
} system;

void main()
{
   gl_Position = MVP*vec4(vertex,1);
   gl_PointSize = 2-vertex.z/2;
   color = ((system.duration-system.age)/system.duration) * system.start_color + (system.age/system.duration) * system.end_color;
}