#include "stdafx.h"
#include"Header/Resources/Resource_Material.h"
#include "..\..\Header\Resources\Resource_Material.h"


ButiEngine::Resource_Material::Resource_Material(const MaterialVariable& arg_var)
{
	materialVar = arg_var;
}

const std::string& ButiEngine::Resource_Material::GetMaterialName()
{
	return materialName;
}

const std::string& ButiEngine::Resource_Material::GetMaterialNameEng()
{
	return materialNameEng;
}

const std::string& ButiEngine::Resource_Material::GetMaterialComment()
{
	return comment;
}

void ButiEngine::Resource_Material::SetMaterialName(const std::string& arg_name)
{
	materialName = arg_name;
}

void ButiEngine::Resource_Material::SetMaterialNameEng(const std::string& arg_name)
{
	materialNameEng = arg_name;
}

void ButiEngine::Resource_Material::SetComment(const std::string& arg_comment)
{
	comment = arg_comment;
}

std::shared_ptr<ButiEngine::IResource_Texture> ButiEngine::Resource_Material::GetTexture(const int index)
{
	return std::shared_ptr<IResource_Texture>();
}

std::shared_ptr<ButiEngine::IResource_Texture> ButiEngine::Resource_Material::GetSphereTexture()
{
	return std::shared_ptr<IResource_Texture>();
}

ButiEngine::MaterialVariable ButiEngine::Resource_Material::GetMaterialVariable() const
{
	return materialVar;
}