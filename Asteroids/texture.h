#pragma once
#include "FreeImage.h"
#include <string>
#include "GL/glew.h"
using namespace std;

class Texture {
public:
	GLuint sampler, texture;
	bool load2D(string path, bool generate_mipmaps = false);
	void Bind();

	void SetFiltering(GLuint magnification, GLuint minification);

	int GetMinification();
	int GetMagnification();

	Texture();
	~Texture();

private:
	int width, height, bpp; // Texture width, height, and bytes per pixel
	bool mipmaps_generated;

	GLuint minification, magnification;

	string path;
};

class SpriteFont : public Texture {
	SpriteFont();
};