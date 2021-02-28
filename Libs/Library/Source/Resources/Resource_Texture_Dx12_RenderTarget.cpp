#include"stdafx.h"
#include"Header/GameParts/GraphicDevice_Dx12.h"
#include "..\..\Header\Resources\Resource_Texture_Dx12_RenderTarget.h"
#include"Header/Device/DescriptorHeapManager.h"

ButiEngine::Resource_Texture_Dx12_RenderTarget::Resource_Texture_Dx12_RenderTarget(int width, int height, std::shared_ptr<GraphicDevice> arg_graphicDevice) 
	//:Resource_Texture_Dx12(std::vector<BYTE>(width*height*4,255),width,height,arg_graphicDevice)
{
	wkp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	image.width = width;
	image.height = height;


	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.NumDescriptors = 1;
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;


		auto hr = wkp_graphicDevice.lock()->GetDevice().CreateDescriptorHeap(&desc, IID_PPV_ARGS(renderTargetDescriptorHeap.GetAddressOf()));
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
	


	auto rtdhHandle = renderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart();


	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	desc.Format =  DXGI_FORMAT_R8G8B8A8_UNORM;
	auto heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto hr= wkp_graphicDevice.lock()->GetDevice().CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(texture.GetAddressOf()));

	wkp_graphicDevice.lock()->GetDevice().CreateRenderTargetView(texture.Get(), &rtvDesc, rtdhHandle);


	auto srvHandleInfo = wkp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->GetNowHandle();
	handle = srvHandleInfo.GPUHandle;
	//テクスチャハンドルを作成
	auto cpuHandle = srvHandleInfo.CPUHandle;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Format = rtvDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	wkp_graphicDevice.lock()->GetDevice().CreateShaderResourceView(
		texture.Get(),
		&srvDesc,
		cpuHandle);


	//レンダーターゲットビューのハンドルを取得
	rtvHandle=CD3DX12_CPU_DESCRIPTOR_HANDLE (
		renderTargetDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), 0,
		wkp_graphicDevice.lock()->GetDevice().GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));



	scissorRect.left = 0;
	scissorRect.right = width;
	scissorRect.top = 0;
	scissorRect.bottom = height;

}

void ButiEngine::Resource_Texture_Dx12_RenderTarget::SetRenderTarget(Vector4& arg_clearColor)
{
	auto trans = CD3DX12_RESOURCE_BARRIER::Transition(texture.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	wkp_graphicDevice.lock()->GetCommandList().ResourceBarrier(1, &trans);
	if (!isCleared) {
		isCleared = true;
		wkp_graphicDevice.lock()->GetCommandList().ClearRenderTargetView(rtvHandle, arg_clearColor.GetData(), 0, nullptr);
	}
	wkp_graphicDevice.lock()->GetCommandList().RSSetScissorRects(1, &scissorRect);
	auto dsvHandle = wkp_graphicDevice.lock()->GetDepthStencil();
	wkp_graphicDevice.lock()->GetCommandList().OMSetRenderTargets(1, &rtvHandle, false, dsvHandle); 


}

void ButiEngine::Resource_Texture_Dx12_RenderTarget::SetRenderTargetWithoutDepth(Vector4& arg_clearColor)
{
	auto trans = CD3DX12_RESOURCE_BARRIER::Transition(texture.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	wkp_graphicDevice.lock()->GetCommandList().ResourceBarrier(1, &trans);
	wkp_graphicDevice.lock()->GetCommandList().RSSetScissorRects(1, &scissorRect);
	wkp_graphicDevice.lock()->GetCommandList().OMSetRenderTargets(1, &rtvHandle, false, nullptr);
	wkp_graphicDevice.lock()->GetCommandList().ClearRenderTargetView(rtvHandle, arg_clearColor.GetData(), 0, nullptr);
}

void ButiEngine::Resource_Texture_Dx12_RenderTarget::CreateTextureUploadHeap()
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

void ButiEngine::Resource_Texture_Dx12_RenderTarget::Initialize()
{
	wkp_graphicDevice.lock()->AddResource(&(*GetThis<Resource_Texture_Dx12>()));
}

void ButiEngine::Resource_Texture_Dx12_RenderTarget::DisSetRenderTarget()
{
	auto desc = CD3DX12_RESOURCE_BARRIER::Transition(texture.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	wkp_graphicDevice.lock()->GetCommandList().ResourceBarrier(1, &desc);
}


void ButiEngine::Resource_Texture_Dx12_RenderTarget::UpdateResourceRelease()
{

	textureUploadHeap = nullptr;
}

void ButiEngine::Resource_Texture_Dx12_RenderTarget::ResourceUpdate()
{

	if (!dataRefresh) {
		return;
	}
	/*CreateTextureUploadHeap();
	D3D12_SUBRESOURCE_DATA textureData = {};
	textureData.pData = image.pixels.data();
	textureData.RowPitch = textureResDesc.Width * texturePixelSize;
	textureData.SlicePitch = textureData.RowPitch * textureResDesc.Height;

	UpdateSubresources(&wkp_graphicDevice.lock()->GetCommandList(),
		texture.Get(),
		textureUploadHeap.Get(),
		0, 0, 1, &textureData);

	
	image.pixels.clear();*/
	dataRefresh = false;
}

void ButiEngine::Resource_Texture_Dx12_RenderTarget::Attach(int slot)
{
	wkp_graphicDevice.lock()->GetCommandList().SetGraphicsRootDescriptorTable(slot, handle);
}

void ButiEngine::Resource_Texture_Dx12_RenderTarget::SetIsCleared(bool arg_isClear)
{
	isCleared = arg_isClear;
}

ButiEngine::Vector2 ButiEngine::Resource_Texture_Dx12_RenderTarget::GetSize()
{
	return Vector2(image.width,image.height);
}

