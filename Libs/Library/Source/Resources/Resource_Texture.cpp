#include"stdafx.h"

#include"Header/Resources/Resource_Texture.h"

void ButiEngine::Resource_Texture::SetFilePath(const std::string & arg_textureFilePath)
{
	textureFilePath = arg_textureFilePath;
}

const std::string & ButiEngine::Resource_Texture::GetTexturePath()
{
	return textureFilePath;
}
