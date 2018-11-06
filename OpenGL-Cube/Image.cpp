#include "Image.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

Image::Image(const string &filename)
{
	m_data = stbi_load(filename.c_str(), &m_width, &m_height, &m_nrChannels, 0);
}

Image::Image(const string &filename, bool mirrored)
{
	stbi_set_flip_vertically_on_load(mirrored);
	m_data = stbi_load(filename.c_str(), &m_width, &m_height, &m_nrChannels, 0);
}

Image::~Image()
{
	stbi_image_free(m_data);
}