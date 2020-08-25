#include "Texture.h"

Texture::Texture(const std::string& path)
	: m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0),m_Height(0)
{
}

Texture::~Texture()
{
}

void Texture::Bind(unsigned int slot) const
{
}

void Texture::Unbind() const
{
}
