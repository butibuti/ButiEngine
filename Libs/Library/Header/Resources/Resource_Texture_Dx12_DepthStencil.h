#pragma once
#include<d3dx12.h>
#include"Resource_Texture_Dx12.h"
namespace ButiEngine {
	class GraphicDevice_Dx12;


	class Resource_Texture_Dx12_DepthStencil :public Resource_Texture_Dx12, public IDepthStencil
	{
	public:
		Resource_Texture_Dx12_DepthStencil(int width, int height, std::shared_ptr<GraphicDevice> arg_wkp_graphicDevice);
		void CreateTextureUploadHeap()override;
		void Initialize()override;
		void UpdateResourceRelease()override;
		void ResourceUpdate() override;
		void Attach(int slot)override;
		void SetIsCleared(bool arg_isClear)override;
		Vector2 GetSize()override;
		void SetDepthStencil() override;
		void DisSetDepthStencil() override;
	private:
		D3D12_RECT     scissorRect;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> depthStencilDescriptorHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
		bool isCleared = false;
	};
}