#version 330
smooth in vec4 smooth_color;

out vec4 frag_color;

void main(void)
{
   frag_color = smooth_color;
}