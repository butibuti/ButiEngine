#include "stdafx.h"
#include"Header/Device/ResourceFileCreater.h"
#include"Header/GameParts/ResourceContainer.h"

void ButiEngine::ResourceFileCreater::CreateMaterialFile(const  MaterialTag& arg_tag, const std::string& arg_filePath, ResourceContainer& arg_resourceContainer, const std::string& arg_fileDirectory)
{
	BinaryWriter b3mWriter;
	b3mWriter.WriteStart(GlobalSettings::GetResourceDirectory() + arg_fileDirectory + arg_filePath + ".bma");

	b3mWriter.WriteCharactor("ButiMaterialData");

	b3mWriter.WriteVariable(1.0f);
	b3mWriter.WriteVariable<char>(1);

	auto material =arg_resourceContainer. GetMaterial(arg_tag).lock();

	b3mWriter.WriteVariable<int>(material->GetMaterialName().size());
	b3mWriter.WriteCharactor(material->GetMaterialName());
	b3mWriter.WriteVariable<int>(material->GetMaterialNameEng().size());
	b3mWriter.WriteCharactor(material->GetMaterialNameEng());


	if (material->GetSphereTextureTag().IsEmpty())
		b3mWriter.WriteVariable<char>(1);
	else
		b3mWriter.WriteVariable<char>(2);


	std::string textureName = arg_resourceContainer. GetTexture(material->GetTextureTag()).lock()->GetTexturePath();

	while (!Util::CheckFileExistence(GlobalSettings::GetResourceDirectory() + arg_fileDirectory + textureName))
	{
		textureName = "../" + textureName;
	}

	b3mWriter.WriteVariable<int>(textureName.size());
	b3mWriter.WriteCharactor(textureName);

	if (!material->GetSphereTextureTag().IsEmpty()) {
		std::string textureName = arg_resourceContainer. GetTexture(material->GetSphereTextureTag()).lock()->GetTexturePath();
		b3mWriter.WriteVariable<int>(textureName.size());
		b3mWriter.WriteCharactor(textureName);
	}

	b3mWriter.WriteVariable<Vector4>(material->GetMaterialVariable().diffuse);
	b3mWriter.WriteVariable<Vector4>(material->GetMaterialVariable().specular);
	b3mWriter.WriteVariable<Vector4>(material->GetMaterialVariable().ambient);
	b3mWriter.WriteVariable<Vector4>(material->GetMaterialVariable().emissive);

	b3mWriter.WriteVariable<char>(0);
	b3mWriter.WriteVariable<char>(0);

	std::string comment = material->GetMaterialComment();

	b3mWriter.WriteVariable<int>(comment.size());
	b3mWriter.WriteCharactor(comment);

	b3mWriter.WriteEnd();

}

void ButiEngine::ResourceFileCreater::CreateModelFile(const  ModelTag& arg_tag, const std::string& arg_filePath, const std::string& arg_fileDirectory)
{
}