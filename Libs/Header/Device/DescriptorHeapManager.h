#pragma once
#include<memory>
#include <commctrl.h> 
#include <D3dx12.h>
namespace ButiEngine {
	struct HandleInformation {
		CD3DX12_GPU_DESCRIPTOR_HANDLE GPUHandle;
		CD3DX12_CPU_DESCRIPTOR_HANDLE CPUHandle;
		UINT index;
	};
	struct BackUpConstantBufferData {
		UINT top;
		UINT sizeAligned;
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	};
	class DescriptorHeapManager
	{
		const UINT DescriptorHeapSize = 5000;
	public:
		DescriptorHeapManager(std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice,const UINT arg_max=100,const UINT arg_addUint= 100);
		void Initialize(ID3D12Device& device);

		HandleInformation GetSamplerHandle(const UINT key);

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap();
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetSamplerHeap();

		Microsoft::WRL::ComPtr<ID3D12Resource> GetConstantBuffer();

		void AddHeapRange();
		HandleInformation CreateConstantBufferView(void* p_value, const bool isKeep, const int size = 256);
		void ConstantBufferUpdate(void* p_value,const UINT index, const int size = 256);
		//HandleInformation CreateShaderResourceView(Microsoft::WRL::ComPtr<ID3D12Resource> resource ,const D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc,const int size );
		HandleInformation GetNowHandle();
		UINT GetDescriptorHandleIncrementSize();
		UINT GetIndex();
		void Release(const BlankSpace& arg_releaseSpace);
	private:
		void ReCreateConstantBuffer();
		UINT cbvSrvDescriptorHandleIncrementSize = 0;
		UINT samplerDescriptorHandleIncrementSize=0;
		UINT index = 0;
		UINT maxCbv;
		UINT addUnit;
		UINT ApplicationKeepRange = 10;
		UINT cbvSize;
		///デスクプリタヒープ
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> cbvSrvUavDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> samplerDescriptorHeap;
		std::vector<BlankSpace> vec_space;
		Microsoft::WRL::ComPtr<ID3D12Resource> constantBufferUploadHeap;
		struct buffer { char buf[0x100]; };
		buffer*  mappedConstantBuffer = nullptr;
		std::vector<BackUpConstantBufferData> vec_cbBackUpData;
		std::weak_ptr<GraphicDevice_Dx12> wkp_graphicDevice;
	};

}