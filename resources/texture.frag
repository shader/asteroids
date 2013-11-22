#version 400

smooth in vec2 UV;

uniform sampler2D tex;

out vec4 color;

void main(void)
{
	color = texture(tex, UV);
}