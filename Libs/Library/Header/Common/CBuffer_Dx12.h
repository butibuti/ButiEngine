#pragma once
#include"stdafx.h"
#include"../Device/DescriptorHeapManager.h"
namespace ButiEngine {
	template <class T>
	class CBuffer_Dx12 :public CBuffer<T>,public Resource
	{
	public:
	CBuffer_Dx12(const UINT arg_slot) {
		size = 0;
		this->slot = arg_slot;
	}
	CBuffer_Dx12(){}
	~CBuffer_Dx12() {
		if (wkp_graphicDevice.lock()) {
			wkp_heapManager.lock()->Release(BlankSpace{ index,size / 0x100 });
			this->instance = nullptr;
		}
	}
	T& Get() const override
	{
		return *instance.get();
	}
	void Initialize() {
		size = (sizeof(T) + 255) & ~255;
		if(!this->instance)
			this->instance=(std::make_shared<T>());
		
	}
	void CreateBuffer(const bool arg_mapKeep=true)override {
		mapKeep = arg_mapKeep;
		auto out= wkp_heapManager.lock()->CreateConstantBufferView(instance.get(), mapKeep, size);
		index= out.index;
		gpuDescriptorHandle = out.GPUHandle;
	}
	void SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice) override{
		wkp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
		wkp_heapManager = wkp_graphicDevice.lock()->GetDescriptorHeapManager();
	}
	void PreInitialize() {

	}
	void Attach()const override {
		wkp_graphicDevice.lock()->GetCommandList().SetGraphicsRootDescriptorTable(TextureTableRegion + SamplerTableRegion + this->slot, gpuDescriptorHandle);
	}
	void Update()const override {

		wkp_heapManager.lock()->ConstantBufferUpdate(instance.get(), index, size);
	}

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(this->slot);
		archive(size);
		archive(index);
		archive(mapKeep);
		archive(this->exName);
		archive(instance);
	}
	bool OnShowUI()override {
		return instance->ShowUI();
	}
	std::shared_ptr<ICBuffer> Clone()override {
		auto output = ObjectFactory::Create<CBuffer_Dx12<T>>(this->slot);

		
		*(output->instance) = (*instance);

		return output;
	}
private:
	void  UpdateResourceRelease()override {

	}
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescriptorHandle;
	std::weak_ptr<GraphicDevice_Dx12> wkp_graphicDevice;
	std::weak_ptr<DescriptorHeapManager> wkp_heapManager;
	std::shared_ptr<T> instance ;
	UINT size;
	UINT index = 0;
	bool mapKeep=true;
	};

}