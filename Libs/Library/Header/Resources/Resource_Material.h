#pragma once
#include "stdafx.h"
namespace ButiEngine {
	struct Resource_Material :public IResource_Material
	{
		Resource_Material(const MaterialVariable& arg_var);
		void Initialize()override {};
		void PreInitialize()override {};
		virtual void Attach(std::shared_ptr<IRenderer> arg_shp_renderer) = 0;
		const std::string& GetMaterialName();
		const std::string& GetMaterialNameEng();
		const std::string& GetMaterialComment();
		void SetMaterialName(const std::string& arg_name);
		void SetMaterialNameEng(const std::string& arg_name);
		void SetComment(const std::string& arg_comment);
		std::shared_ptr<IResource_Texture>GetTexture();
		std::shared_ptr<IResource_Texture>GetSphereTexture();
		MaterialVariable GetMaterialVariable() const;

		TextureTag GetSphereTextureTag() {
			return sphereTextureTag;
		}
		TextureTag GetTextureTag() {
			return textureTag;
		}
		TextureTag textureTag;
		TextureTag sphereTextureTag ;
	protected:
		MaterialVariable materialVar;
		std::string materialName;
		std::string materialNameEng;
		std::string comment;
	};

}