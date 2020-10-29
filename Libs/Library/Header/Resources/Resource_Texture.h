#pragma once
#include"stdafx.h"
namespace ButiEngine {
	class GraphicDevice;


	class Resource_Texture :public IResource_Texture, public Resource
	{
	public:
		void Initialize()override {}
		void PreInitialize()override {}
		virtual void SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice) = 0;
		virtual void Attach(int slot) = 0;
		UInt2 GetSize() const { return size; }
		void SetFilePath(const std::string& arg_textureFilePath);
		const std::string& GetTexturePath();
	protected:
		UInt2 size;
		std::string textureFilePath;
	};

}