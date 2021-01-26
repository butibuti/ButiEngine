#pragma once
#include "stdafx.h"
namespace ButiEngine {
	struct Resource_Material :public IResource_Material
	{
		Resource_Material(const MaterialVariable& arg_var);
		void Initialize()override {};
		void PreInitialize()override {};
		const std::string& GetMaterialName();
		const std::string& GetMaterialNameEng();
		const std::string& GetMaterialComment();
		void SetMaterialName(const std::string& arg_name);
		void SetMaterialNameEng(const std::string& arg_name);
		void SetComment(const std::string& arg_comment);
		std::shared_ptr<IResource_Texture>GetTexture(const int index = 0);
		std::shared_ptr<IResource_Texture>GetSphereTexture();
		
		std::vector< TextureTag > GetTextureTags()override {
			return textureTag;
		}
		TextureTag GetSphereTextureTag() const{
			return sphereTextureTag;
		}
		TextureTag GetTextureTag(const int index=0)const {
			return textureTag[index];
		}
		std::vector< TextureTag >textureTag;
		TextureTag sphereTextureTag ;
	protected:
		MaterialVariable materialVar;
		std::string materialName;
		std::string materialNameEng;
		std::string comment;
	};

}