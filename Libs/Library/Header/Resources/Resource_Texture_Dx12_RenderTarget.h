#pragma once
#include<d3dx12.h>
#include"Resource_Texture_Dx12.h"
namespace ButiEngine {
	class GraphicDevice_Dx12;


	class Resource_Texture_Dx12_RenderTarget :public Resource_Texture_Dx12,public IRenderTarget
	{
	public:
		Resource_Texture_Dx12_RenderTarget(int width, int height, std::shared_ptr<GraphicDevice> arg_graphicDevice);
		void SetRenderTarget(Vector4& arg_clearColor)override;
		void SetRenderTargetWithoutDepth(Vector4& arg_clearColor)override;
		void CreateTextureUploadHeap()override;
		void Initialize()override;
		void EndRenderTarget()override;
		void UpdateResourceRelease()override;
		void ResourceUpdate() override;
		void Attach(int slot)override;
	private:

		D3D12_RECT     scissorRect;
		Microsoft::WRL::ComPtr<ID3D12Resource> depthStencil;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> depthStencilDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> renderTargetDescriptorHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
		//void UpdateResourceRelease()override;
		//void ResourceUpdate() override;

	};
}