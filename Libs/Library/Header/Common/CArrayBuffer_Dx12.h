#pragma once
#include"stdafx.h"
#include"CArrayBuffer.h"
namespace ButiEngine {

	template <class T>
	class CArrayBuffer_Dx12:public CArrayBuffer<T>
	{
	public:
		CArrayBuffer_Dx12(const UINT arg_slot,const UINT count) {
			this->slot = arg_slot;
			size = (sizeof(T) + 255) & ~255;
			instanceSize = sizeof(T) * count;

			instance = (T*)malloc(instanceSize);
		}
		~CArrayBuffer_Dx12() {

			wkp_heapManager.lock()->Release(BlankSpace{ index,instanceSize / 0x100 });
			delete instance;


		}
		T& Get(const UINT arg_index) override
		{
			return instance[arg_index];
		}

		void Initialize() override{}


		void CreateBuffer(const bool arg_mapKeep = true) override{
			mapKeep = arg_mapKeep;
			auto out = wkp_heapManager.lock()->CreateConstantBufferView(instance, mapKeep, instanceSize);
			index = out.index;
			gpuDescriptorHandle = out.GPUHandle;
		}

		void SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice) {
			wkp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
			wkp_heapManager = wkp_graphicDevice.lock()->GetDescriptorHeapManager();
		}
		void PreInitialize() override{

		}
		void Attach()const override{
			wkp_graphicDevice.lock()->GetCommandList().SetGraphicsRootDescriptorTable(TextureTableRegion + SamplerTableRegion + this->slot, gpuDescriptorHandle);
		}
		void Update()const override{

			wkp_heapManager.lock()->ConstantBufferUpdate(instance, index, instanceSize);
		}

	private:
		std::weak_ptr<GraphicDevice_Dx12> wkp_graphicDevice;
		std::weak_ptr<DescriptorHeapManager> wkp_heapManager;
		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescriptorHandle;
		T* instance;
		UINT size;
		UINT index;
		UINT instanceSize;
		bool mapKeep = true;
	};

}