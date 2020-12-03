#pragma once
#include"stdafx.h"
#include"../../Resources/DrawData/IDrawData.h"
#include"Header/Common/CBuffer_Dx12.h"
#include"Header/Common/CArrayBuffer_Dx12.h"


namespace ButiEngine {


	class MeshDrawComponent :public GameComponent
	{
	public:
		MeshDrawComponent(const MeshTag& arg_meshTag, const ShaderTag& arg_shaderTag, const MaterialTag& arg_materialTag, std::shared_ptr< DrawInformation >arg_shp_drawInfo=nullptr, const UINT arg_layer = 0, std::shared_ptr<Transform> arg_shp_transform = nullptr);
		MeshDrawComponent(const MeshTag& arg_meshTag, const ShaderTag& arg_shaderTag, const std::vector< MaterialTag>& arg_materialTag, std::shared_ptr< DrawInformation >arg_shp_drawInfo = nullptr, const UINT arg_layer=0, std::shared_ptr<Transform> arg_shp_transform = nullptr);
		MeshDrawComponent(const ModelTag& arg_modelTag, const ShaderTag& arg_shaderTag, std::shared_ptr< DrawInformation >arg_shp_drawInfo = nullptr, const UINT arg_layer = 0, std::shared_ptr<Transform> arg_shp_transform = nullptr);
		MeshDrawComponent() {}
		std::string GetGameComponentName() {
			return "MeshDraw";
		}
		void OnUpdate()override;
		void ChangeFillMode(const bool arg_isFill);
		void SwitchFillMode(const bool arg_isFill);
		void ChangeCullMode(const CullMode& arg_cullMode);
		void OnSet()override;
		void OnRemove() override;
		void SetBlendMode(const BlendMode& arg_blendMode);
		void SetMaterialTag(MaterialTag  arg_materialTag);
		std::shared_ptr<GameComponent> Clone()override;
		void Regist();
		void ReRegist();
		void UnRegist();
		std::shared_ptr< DrawInformation > GetDrawInformation();
		void OnShowUI()override;

		template <class T>
		std::shared_ptr<CBuffer< T>> CreateCBuffer(const std::string& arg_cBufferName, const UINT arg_slot,  std::weak_ptr<GraphicDevice> arg_wkp_graphicDevice) {
			std::shared_ptr<CBuffer< T>> out = nullptr;
			out = ObjectFactory::Create<CBuffer_Dx12<T>>(arg_slot);
			
			out->SetGraphicDevice(arg_wkp_graphicDevice.lock());
			out->CreateBuffer();
			out->SetExName(arg_cBufferName);
			data->GetThis<DrawData>()->AddICBuffer(out);

			return out;
		}
		template <class T>
		std::shared_ptr<CArrayBuffer< T>> CreateCArrayBuffer(const std::string& arg_cBufferName, const UINT arg_slot, const UINT arg_size, std::weak_ptr<GraphicDevice> arg_wkp_graphicDevice) {
			std::shared_ptr<CArrayBuffer< T>> out = nullptr;
			out = ObjectFactory::Create<CArrayBuffer_Dx12<T>>(arg_slot);
			
			out->SetGraphicDevice(arg_wkp_graphicDevice.lock());
			out->CreateInstance(arg_size);
			out->CreateBuffer();
			out->SetExName(arg_cBufferName);
			data->GetThis<DrawData>()->AddICBuffer(out);

			return out;
		}
		template <class T>
		std::shared_ptr<CBuffer<T>> GetCBuffer(const std::string& arg_cBufferName) {
			return data->GetThis<DrawData>()->GetCBuffer<T>(arg_cBufferName);
		}

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(layer);
			archive(meshTag);
			archive(shaderTag);
			archive(modelTag);
			archive(materialTag);
			archive(isActive);
			archive(shp_transform);
			archive(shp_drawInfo);
		}
	protected:
		std::shared_ptr< MeshDrawData > data;
		virtual void CreateData();
		UINT* index;
		UINT layer; 
		MeshTag meshTag;
		ShaderTag shaderTag ;
		ModelTag modelTag;
		std::shared_ptr<Transform> shp_transform;
		std::shared_ptr< DrawInformation >shp_drawInfo = nullptr;
		std::vector<MaterialTag> materialTag;

		bool isCereal = true;
	};
}
