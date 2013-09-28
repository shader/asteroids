#include "texture.h"
using namespace std;

Texture::Texture() {
	glGenTextures(1, &texture);
	glGenSamplers(1, &sampler);
}

Texture::~Texture() {
	glDeleteTextures(1, &texture);
	glDeleteSamplers(1, &sampler);
}

void Texture::Bind() {
	glBindTexture(GL_TEXTURE_2D, texture);
}

bool Texture::load2D(string path, bool generate_mipmaps) {
	this->path = path;
	mipmaps_generated = generate_mipmaps;
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path.c_str(), 0);
	FIBITMAP* bitmap(0);

	if(format == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
		format = FreeImage_GetFIFFromFilename(path.c_str());

	if(format == FIF_UNKNOWN) // If still unkown, return failure
		return false;

	if(FreeImage_FIFSupportsReading(format)) // Check if the plugin has reading capabilities and load the file
		bitmap = FreeImage_Load(format, path.c_str());
	if(!bitmap)
		return false;
		
	BYTE* data = FreeImage_GetBits(bitmap); // Retrieve the image data

	width = FreeImage_GetWidth(bitmap); // Get the image width and height
	height = FreeImage_GetHeight(bitmap);
	bpp = FreeImage_GetBPP(bitmap);

	// If somehow one of these failed (they shouldn't), return failure
	if(data == NULL || width == 0 || height == 0)
		return false;

	int gl_format = bpp == 24 ? GL_BGR : bpp == 8 ? GL_LUMINANCE : 0;
	
	Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, gl_format, GL_UNSIGNED_BYTE, data);
	
	if(generate_mipmaps)
		glGenerateMipmap(GL_TEXTURE_2D);

	FreeImage_Unload(bitmap);
	return true;
}


void Texture::SetFiltering(GLuint magnification, GLuint minification)
{
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, magnification);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, minification);

	this->minification = minification;
	this->magnification = magnification;
}