#include"stdafx.h"
#include"Header/GameParts/GraphicDevice_Dx12.h"
#include "..\..\Header\Resources\Resource_Texture_Dx12_DepthStencil.h"

#include"Header/Device/DescriptorHeapManager.h"

ButiEngine::Resource_Texture_Dx12_DepthStencil::Resource_Texture_Dx12_DepthStencil(int width, int height, std::shared_ptr<GraphicDevice> arg_graphicDevice)
{
	wkp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	image.width = width;
	image.height = height;


	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = 1;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		// 深度ステンシルビュー用ディスクリプタヒープの設定.
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

		// 深度ステンシルビュー用ディスクリプタヒープを生成.

		auto hr = wkp_graphicDevice.lock()->GetDevice().CreateDescriptorHeap(&desc, IID_PPV_ARGS(depthStencilDescriptorHeap.GetAddressOf()));
		if (FAILED(hr))
		{
		}
	}

	// ヒーププロパティの設定.
	D3D12_HEAP_PROPERTIES prop;
	prop.Type = D3D12_HEAP_TYPE_DEFAULT;
	prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask = 1;
	prop.VisibleNodeMask = 1;

	// リソースの設定.
	D3D12_RESOURCE_DESC desc;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = width;
	desc.Height = height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 0;
	desc.Format = DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	// クリア値の設定.
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = DXGI_FORMAT_D32_FLOAT;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;
	// リソースを生成.
	auto hr = wkp_graphicDevice.lock()->GetDevice().CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(texture.GetAddressOf()));
	if (FAILED(hr))
	{
		int a = 0;
	}
	// 深度ステンシルビューの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

	// 深度ステンシルビューを生成.
	wkp_graphicDevice.lock()->GetDevice().CreateDepthStencilView(texture.Get(), &dsvDesc, depthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart());



	auto srvHandleInfo = wkp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->GetNowHandle();
	handle = srvHandleInfo.GPUHandle;
	//テクスチャハンドルを作成
	auto cpuHandle = srvHandleInfo.CPUHandle;
	D3D12_SHADER_RESOURCE_VIEW_DESC resourct_view_desc{};
	resourct_view_desc.Format = DXGI_FORMAT_R32_FLOAT;
	resourct_view_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	resourct_view_desc.Texture2D.MipLevels = 1;
	resourct_view_desc.Texture2D.MostDetailedMip = 0;
	resourct_view_desc.Texture2D.PlaneSlice = 0;
	resourct_view_desc.Texture2D.ResourceMinLODClamp = 0.0F;
	resourct_view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;


	wkp_graphicDevice.lock()->GetDevice().CreateShaderResourceView(
		texture.Get(),
		&resourct_view_desc,
		cpuHandle);

	//デプスステンシルビューのハンドルを取得
	dsvHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		depthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart()
	);


	scissorRect.left = 0;
	scissorRect.right = width;
	scissorRect.top = 0;
	scissorRect.bottom = height;
}

void ButiEngine::Resource_Texture_Dx12_DepthStencil::CreateTextureUploadHeap()
{

	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(texture.Get(), 0, 1);
	auto hr = wkp_graphicDevice.lock()->GetDevice().GetDeviceRemovedReason();
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);
	auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	hr = wkp_graphicDevice.lock()->GetDevice().CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		nullptr,
		IID_PPV_ARGS(&textureUploadHeap));
}

void ButiEngine::Resource_Texture_Dx12_DepthStencil::Initialize()
{
	wkp_graphicDevice.lock()->AddResource(&(*GetThis<Resource_Texture_Dx12>()));
}

void ButiEngine::Resource_Texture_Dx12_DepthStencil::UpdateResourceRelease()
{

	textureUploadHeap = nullptr;
}

void ButiEngine::Resource_Texture_Dx12_DepthStencil::ResourceUpdate()
{
	if (!dataRefresh) {
		return;
	}

	dataRefresh = false;
}

void ButiEngine::Resource_Texture_Dx12_DepthStencil::Attach(int slot)
{
	wkp_graphicDevice.lock()->GetCommandList().SetGraphicsRootDescriptorTable(slot, handle);
}

void ButiEngine::Resource_Texture_Dx12_DepthStencil::SetIsCleared(bool arg_isClear)
{
	isCleared = arg_isClear;
}

ButiEngine::Vector2 ButiEngine::Resource_Texture_Dx12_DepthStencil::GetSize()
{
    return Vector2(image.width,image.height);
}

void ButiEngine::Resource_Texture_Dx12_DepthStencil::SetDepthStencil()
{
	if (!isCleared) {
		isCleared = true;
		wkp_graphicDevice.lock()->GetCommandList().ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}
	wkp_graphicDevice.lock()->SetDepthStencil(&dsvHandle);
}

void ButiEngine::Resource_Texture_Dx12_DepthStencil::DisSetDepthStencil()
{
	wkp_graphicDevice.lock()->DisSetDepthStencil();
}
