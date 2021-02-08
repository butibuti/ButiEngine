#pragma once
#include"stdafx.h"
#include"IDrawData.h"
#include"../../Common/CBuffer_Dx12.h"
namespace ButiEngine {

	struct DrawData_Dx12 :public MeshDrawData {
		void Initialize(const UINT srvCount);

		void CreatePipeLineState(const UINT arg_exCBuffer,const UINT srvCount);
		void CommandExecute();
		void BufferUpdate();
		void CommandSet();
		std::shared_ptr<ICBuffer> AddICBuffer(std::shared_ptr<ICBuffer> arg_cbuffer)override;

		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;


		D3D12_GPU_DESCRIPTOR_HANDLE samplerBufferDescriptorHandle;

		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipeLineDesc;
		//パイプラインステート
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;

		///バンドルコマンドリスト
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;

		D3D12_RASTERIZER_DESC rasterizerStateDesc;
		D3D12_ROOT_SIGNATURE_DESC rootdesc;
		SamplerState samplerState;
		std::weak_ptr<GraphicDevice_Dx12> wkp_graphicDevice;
		int textureRegion;
	};

	struct MeshDrawData_Dx12 :public DrawData_Dx12 {
		
		MeshDrawData_Dx12(const MeshTag& arg_meshTag, const ShaderTag& arg_shader, const MaterialTag& arg_materialTag, std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice, std::shared_ptr< DrawInformation >arg_shp_drawInfo,std::shared_ptr<Transform> arg_shp_transform);
		
		MeshDrawData_Dx12(const MeshTag& arg_meshTag,const ShaderTag& arg_shader, const std::vector< MaterialTag>& arg_materialTag, std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice, std::shared_ptr< DrawInformation >arg_shp_drawInfo, std::shared_ptr<Transform> arg_shp_transform);
		
		MeshDrawData_Dx12(const ModelTag& arg_model, const ShaderTag& arg_shader, std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice, std::shared_ptr< DrawInformation >arg_shp_drawInfo, std::shared_ptr<Transform> arg_shp_transform);

		void DrawBefore()override;
		void Draw()override;

		void SetTransform(std::shared_ptr<Transform>& arg_transform)override {
			transform = arg_transform->GetMatrix();
		}

		inline float GetZ()override {
			return  GetMaxZ(wkp_graphicDevice.lock()->GetRawViewMatrix());
		}

		void Initialize()override;

		void PreInitialize()override {};


		
	};

}
