#pragma once
#include"stdafx.h"
#include"Resource_Material.h"
#include"../Common/CBuffer_Dx12.h"
namespace ButiEngine {
	class GraphicDevice_Dx12;
	class Resource_Material_Dx12:public Resource_Material
	{
	public:
		Resource_Material_Dx12(const MaterialVariable& arg_var, const TextureTag& arg_textureTag, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice);
		Resource_Material_Dx12(const MaterialVariable& arg_var, const std::vector< TextureTag>& arg_textureTag, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice);
		void Initialize()override ;
		void Attach(const UINT slotOffset, std::shared_ptr<IRenderer> arg_shp_renderer)override;
		void Update()override;
		UINT GetTextureCount()const override;
		bool OnShowUI() ;
		void SetTextureTag(const UINT index, TextureTag arg_tag) ;
		void SetMaterialVariable(const MaterialVariable& arg_var);
		MaterialVariable& GetMaterialVariable();
	private:
		std::weak_ptr<GraphicDevice_Dx12> wkp_graphicDevice;
		std::shared_ptr< CBuffer_Dx12<MaterialVariable>> materialBuffer;
	};

}