#include "stdafx.h"
#include"Header/Device/DescriptorHeapManager.h"
#include"Header/Device/GraphicResourceUtil_Dx12.h"

ButiEngine::DescriptorHeapManager::DescriptorHeapManager( std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice,const UINT arg_max , const UINT arg_addUint )
{
	wkp_graphicDevice= arg_wkp_graphicDevice;
	maxCbv = 50;//arg_max;
	addUnit = arg_addUint;
}

void ButiEngine::DescriptorHeapManager::Initialize(ID3D12Device& device)
{
	cbvSrvUavDescriptorHeap = DescriptorHeapHelper::CreateCbvSrvUavHeap(DescriptorHeapSize,device);
	samplerDescriptorHeap = DescriptorHeapHelper::CreateSamplerHeap(8,device);
	cbvSrvDescriptorHandleIncrementSize
		=device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	samplerDescriptorHandleIncrementSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	

	auto SamplerDescriptorHandle = samplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	auto samplerSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
	SamplerHelper::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle, device);
	SamplerDescriptorHandle.ptr += samplerSize;
	SamplerHelper::CreateSampler(SamplerState::LinearWrap, SamplerDescriptorHandle, device);
	SamplerDescriptorHandle.ptr += samplerSize;
	SamplerHelper::CreateSampler(SamplerState::SamplerNone, SamplerDescriptorHandle, device);
	SamplerDescriptorHandle.ptr += samplerSize;
	SamplerHelper::CreateSampler(SamplerState::PointWrap, SamplerDescriptorHandle, device);
	SamplerDescriptorHandle.ptr += samplerSize;
	SamplerHelper::CreateSampler(SamplerState::PointClamp, SamplerDescriptorHandle, device);
	SamplerDescriptorHandle.ptr += samplerSize;
	SamplerHelper::CreateSampler(SamplerState::AnisotropicWrap, SamplerDescriptorHandle, device);
	SamplerDescriptorHandle.ptr += samplerSize;
	SamplerHelper::CreateSampler(SamplerState::AnisotropicClamp, SamplerDescriptorHandle, device);
	SamplerDescriptorHandle.ptr += samplerSize;
	SamplerHelper::CreateSampler(SamplerState::ComparisonLinear, SamplerDescriptorHandle, device);

	cbvSize =((UINT64) maxCbv) * 0x100;

	D3D12_HEAP_PROPERTIES prop = { D3D12_HEAP_TYPE_UPLOAD, D3D12_CPU_PAGE_PROPERTY_UNKNOWN, D3D12_MEMORY_POOL_UNKNOWN, 1, 1 };
	D3D12_RESOURCE_DESC desc = { D3D12_RESOURCE_DIMENSION_BUFFER, 0, (UINT64)cbvSize, 1, 1, 1, DXGI_FORMAT_UNKNOWN, { 1, 0 }, D3D12_TEXTURE_LAYOUT_ROW_MAJOR, D3D12_RESOURCE_FLAG_NONE };
	device.CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constantBufferUploadHeap));

	D3D12_RANGE readRange = {};
	HRESULT hr = constantBufferUploadHeap->Map(0, &readRange, (void**)&mappedConstantBuffer);


}



ButiEngine::HandleInformation ButiEngine::DescriptorHeapManager::GetSamplerHandle(const UINT key)
{
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(
		samplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
		key,
		samplerDescriptorHandleIncrementSize
	); 
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle(
		samplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
		key,
		samplerDescriptorHandleIncrementSize
	);
	HandleInformation out{ gpuHandle,cpuHandle,key };
	return out;
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ButiEngine::DescriptorHeapManager::GetDescriptorHeap()
{
	return cbvSrvUavDescriptorHeap;
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ButiEngine::DescriptorHeapManager::GetSamplerHeap()
{
	return samplerDescriptorHeap;
}

Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::DescriptorHeapManager::GetConstantBuffer()
{
	return constantBufferUploadHeap;
}

ButiEngine::HandleInformation ButiEngine::DescriptorHeapManager::CreateConstantBufferView(void* p_value, const bool isKeep, const int size )
{
	UINT sizeAligned = (size + 0xff) & ~0xff;
	UINT numRequired = sizeAligned / 0x100;


	UINT top;
	bool isUseSpace = false;
	if (vec_space.size()) {
		auto itr = vec_space.begin();
		for (; itr != vec_space.end(); itr++) {
			if (itr->size >= numRequired) {
				isUseSpace = true;

				top = itr->index;
				itr->index += numRequired;
				itr->size -= numRequired;
				if (itr->size == 0) {
					vec_space.erase(itr);
				}

				break;

			}
		}
	}

	if ((!isUseSpace) && index + numRequired > maxCbv) {
		AddHeapRange();
		top = index;
		index += numRequired;
		//throw ButiException(L"", L"", L"");
	}
	else if ((!isUseSpace)) {
		top = index;
		index += numRequired;
	}


	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(
		cbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
		top,
		cbvSrvDescriptorHandleIncrementSize
	);
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle(
		cbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
		top,
		cbvSrvDescriptorHandleIncrementSize
	);
	HandleInformation out{ gpuHandle,cpuHandle,top };


	memcpy(mappedConstantBuffer + top, p_value, size);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};

	BackUpConstantBufferData backUpData = {};

	cbvDesc.BufferLocation = constantBufferUploadHeap->GetGPUVirtualAddress() + ((UINT64)top * 0x100);

	cbvDesc.SizeInBytes = sizeAligned;


	assert((cbvDesc.SizeInBytes & 0xff) == 0);

	wkp_graphicDevice.lock()
		->GetDevice().CreateConstantBufferView(&cbvDesc, out.CPUHandle);
	auto hr = wkp_graphicDevice.lock()->GetDevice().GetDeviceRemovedReason();

	//if (hr != S_OK) {
	//	std::cout << hr << std::endl;
	//}/*
	//else {
	//	std::cout << "OK!" << std::endl;
	//}*/

	vec_cbBackUpData.push_back({ top,sizeAligned,out.CPUHandle });

	return out;
}

void ButiEngine::DescriptorHeapManager::ConstantBufferUpdate(void* p_value, const UINT index, const int size)
{
	memcpy(mappedConstantBuffer + index, p_value, size);
}


ButiEngine::HandleInformation ButiEngine::DescriptorHeapManager::GetNowHandle()
{
	UINT sizeAligned = 256;
	UINT numRequired = sizeAligned / 0x100;


	UINT top;
	bool isUseSpace = false;
	if (vec_space.size()) {
		auto itr = vec_space.begin();
		for (; itr != vec_space.end(); itr++) {
			if (itr->size >= numRequired) {
				isUseSpace = true;

				top = itr->index;
				itr->index + numRequired;
				itr->size -= numRequired;
				if (itr->size == 0) {
					vec_space.erase(itr);
				}

				break;

			}
		}
	}

	if ((!isUseSpace) && index + numRequired > maxCbv) {
		AddHeapRange();
		top = index;
		index += numRequired;
		//throw ButiException(L"", L"", L"");
	}
	else if ((!isUseSpace)) {
		top = index;
		index += numRequired;
	}


	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle(
		cbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
		top,
		cbvSrvDescriptorHandleIncrementSize
	);
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle(
		cbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
		top,
		cbvSrvDescriptorHandleIncrementSize
	);
	HandleInformation out{ gpuHandle,cpuHandle,top };
	return out;
}

UINT ButiEngine::DescriptorHeapManager::GetDescriptorHandleIncrementSize()
{
	return cbvSrvDescriptorHandleIncrementSize;
}

UINT ButiEngine::DescriptorHeapManager::GetIndex()
{
	return index;
}

void ButiEngine::DescriptorHeapManager::Release(const BlankSpace& arg_releaseSpace)
{
	vec_space.push_back(arg_releaseSpace);
}

void ButiEngine::DescriptorHeapManager::ReCreateConstantBuffer()
{
	std::reverse(vec_cbBackUpData.begin(), vec_cbBackUpData.end());
	std::map<UINT, std::string> map_check;
	for (auto itr = vec_cbBackUpData.begin();itr!= vec_cbBackUpData.end(); ) {
		if (map_check.count(itr->top)) {
			itr = vec_cbBackUpData.erase(itr);
			continue;
		}
		else {
			map_check.emplace(itr->top, "");
		}

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};

		//BackUpConstantBufferData backUpData;

		cbvDesc.BufferLocation = constantBufferUploadHeap->GetGPUVirtualAddress() + ((UINT64)itr->top * 0x100);

		cbvDesc.SizeInBytes = itr->sizeAligned;


		assert((cbvDesc.SizeInBytes & 0xff) == 0);

		wkp_graphicDevice.lock()
			->GetDevice().CreateConstantBufferView(&cbvDesc, itr->cpuHandle);
		itr++;
	}
}

void ButiEngine::DescriptorHeapManager::AddHeapRange()
{
	std::cout << "AddHeapRange" << std::endl;
	maxCbv += addUnit;
	if (maxCbv > DescriptorHeapSize)
		throw ButiException(L"", L"", L"");

	UINT64 size = ((UINT64)maxCbv) * 0x100;

	
	D3D12_HEAP_PROPERTIES prop = { D3D12_HEAP_TYPE_UPLOAD, D3D12_CPU_PAGE_PROPERTY_UNKNOWN, D3D12_MEMORY_POOL_UNKNOWN, 1, 1 };
	D3D12_RESOURCE_DESC desc = constantBufferUploadHeap->GetDesc();
	desc.Width = size;

	wkp_graphicDevice.lock()->GetDevice().CreateCommittedResource(&prop, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constantBufferUploadHeap));
	buffer* mapped ;
	D3D12_RANGE readRange = {};
	HRESULT hr = constantBufferUploadHeap->Map(0, &readRange, (void**)&mapped);

	//memcpy(mapped, mappedConstantBuffer, cbvSize);

	 

	mappedConstantBuffer = mapped;

	auto container = wkp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer();
	if(container)
	container->MaterialUpdate();

	ReCreateConstantBuffer();
	cbvSize = size;

}
