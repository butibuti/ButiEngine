#include"stdafx.h"

#include"Header/Resources/Resource_Texture_Dx12.h"
#include"Header/Device/DescriptorHeapManager.h"

ButiEngine::Resource_Texture_Dx12::Resource_Texture_Dx12(const std::vector<BYTE>& buffer, int width, int height, std::shared_ptr<GraphicDevice> arg_graphicDevice)
{
	wkp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	image.width = width;
	image.height = height;
	image.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	image.rowPitch = width * 4;
	image.slicePitch = width * height * 4;
	image.pixels.clear();
	image.pixels = buffer;
	TexMetadata texMetadata;

	texMetadata.width = width;
	texMetadata.height = height;
	texMetadata.depth = 1;
	texMetadata.arraySize = 1;
	texMetadata.mipLevels = 1;
	texMetadata.miscFlags = 0;
	texMetadata.miscFlags2 = 0;
	texMetadata.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	texMetadata.dimension = TEX_DIMENSION_TEXTURE2D;

	CreateTexture(&image, 1, texMetadata);

	auto srvHandleInfo = wkp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->GetNowHandle();
	handle = srvHandleInfo.GPUHandle;
	//テクスチャハンドルを作成
	auto cpuHandle = srvHandleInfo.CPUHandle;
	//テクスチャのシェーダリソースビューを作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//フォーマット
	srvDesc.Format = GetTextureDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = GetTextureDesc().MipLevels;
	//シェーダリソースビュー
	wkp_graphicDevice.lock()->GetDevice().CreateShaderResourceView(
		GetTexture().Get(),
		&srvDesc,
		cpuHandle);
}

void ButiEngine::Resource_Texture_Dx12::SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice)
{
	wkp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}

void ButiEngine::Resource_Texture_Dx12::Attach(int slot)
{
	wkp_graphicDevice.lock()->GetCommandList().SetGraphicsRootDescriptorTable(slot, handle);
}

void ButiEngine::Resource_Texture_Dx12::CreateTexture(Image* srcImages, size_t nimages, const TexMetadata& metadata)
{
	if (metadata.IsVolumemap()) {
		throw ButiException(
			L"この形式には対応してません",
			L"if (metadata.IsVolumemap())",
			L"TextureResource::Impl::CreateTexture()"
		);
	}

	std::unique_ptr<CUSTAM_SUBRESOURCE_DATA[]> initData(new (std::nothrow) CUSTAM_SUBRESOURCE_DATA[metadata.mipLevels * metadata.arraySize]);
	if (!initData) {
		throw ButiException(
			L"テクスチャデータの領域取得に失敗しました",
			L"if (!initData)",
			L"TextureResource::Impl::CreateTexture()"
		);

	}

	//--- 1D or 2D texture case ---------------------------------------------------
	size_t idx = 0;
	for (size_t item = 0; item < metadata.arraySize; ++item)
	{
		for (size_t level = 0; level < metadata.mipLevels; ++level)
		{
			size_t index = metadata.ComputeIndex(level, item, 0);
			if (index >= nimages) {
				throw ButiException(
					L"データのインデックスが範囲外です",
					L"if (index >= nimages)",
					L"TextureResource::Impl::CreateTexture()"
				);

			}
			Image& img = srcImages[index];

			if (img.format != metadata.format) {
				throw ButiException(
					L"データのフォーマットが違います",
					L"if (img.format != metadata.format)",
					L"TextureResource::Impl::CreateTexture()"
				);
			}
			if (img.pixels.size() <= 0) {
				throw ButiException(
					L"データのポインタが不正です",
					L"if (!img.pixels)",
					L"TextureResource::Impl::CreateTexture()"
				);
			}
			if (idx >= (metadata.mipLevels * metadata.arraySize)) {
				throw ButiException(
					L"データの並びが不定です",
					L"if (idx >= (metadata.mipLevels * metadata.arraySize))",
					L"TextureResource::Impl::CreateTexture()"
				);
			}
			++idx;
		}
	}

	DXGI_FORMAT tformat = metadata.format;

	switch (metadata.dimension)
	{
	case TEX_DIMENSION_TEXTURE1D:
	{
		::ZeroMemory(&textureResDesc, sizeof(D3D12_RESOURCE_DESC));
		textureResDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
		textureResDesc.Format = tformat;
		textureResDesc.Width = static_cast<UINT>(metadata.width);
		textureResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		textureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.depth);
		textureResDesc.SampleDesc.Count = 1;
		textureResDesc.SampleDesc.Quality = 0;
		textureResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
	}
	break;

	case TEX_DIMENSION_TEXTURE2D:
	{

		::ZeroMemory(&textureResDesc, sizeof(D3D12_RESOURCE_DESC));
		textureResDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
		textureResDesc.Format = tformat;
		textureResDesc.Width = static_cast<UINT>(metadata.width);
		textureResDesc.Height = static_cast<UINT>(metadata.height);
		textureResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		if (metadata.IsCubemap()) {
			textureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
		}
		else {
			textureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.depth);
		}
		textureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
		textureResDesc.SampleDesc.Count = 1;
		textureResDesc.SampleDesc.Quality = 0;
		textureResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	}
	break;

	case TEX_DIMENSION_TEXTURE3D:
	{
		::ZeroMemory(&textureResDesc, sizeof(D3D12_RESOURCE_DESC));
		textureResDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
		textureResDesc.Format = tformat;
		textureResDesc.Width = static_cast<UINT>(metadata.width);
		textureResDesc.Height = static_cast<UINT>(metadata.height);
		textureResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
		textureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
		textureResDesc.SampleDesc.Count = 1;
		textureResDesc.SampleDesc.Quality = 0;
		textureResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
	}
	break;
	}

	texturePixelSize = static_cast<DWORD>(image.rowPitch) / (UINT)textureResDesc.Width;



	wkp_graphicDevice.lock()->GetDevice().CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE, &textureResDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&texture));
	//

	dataRefresh = true;

}

void ButiEngine::Resource_Texture_Dx12::CreateTextureUploadHeap()
{
	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(texture.Get(), 0, 1);


	auto hr = wkp_graphicDevice.lock()->GetDevice().CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&textureUploadHeap));

}

void ButiEngine::Resource_Texture_Dx12::UpdateResourceRelease()
{
	textureUploadHeap = nullptr;
}

void ButiEngine::Resource_Texture_Dx12::Initialize()
{
	wkp_graphicDevice.lock()->AddResource(&(*GetThis<Resource_Texture_Dx12>()));
}
Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::Resource_Texture_Dx12::GetTexture()
{
	return texture;
}
Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::Resource_Texture_Dx12::GetTextureUploadHeap()
{
	return textureUploadHeap;
}
D3D12_RESOURCE_DESC ButiEngine::Resource_Texture_Dx12::GetTextureDesc()
{
	return textureResDesc;
}
void ButiEngine::Resource_Texture_Dx12::ResourceUpdate()
{
	if (!dataRefresh) {
		return;
	}
	CreateTextureUploadHeap();
	D3D12_SUBRESOURCE_DATA textureData = {};
	textureData.pData = image.pixels.data();
	textureData.RowPitch = textureResDesc.Width * texturePixelSize;
	textureData.SlicePitch = textureData.RowPitch * textureResDesc.Height;

	UpdateSubresources(&wkp_graphicDevice.lock()->GetCommandList(),
		texture.Get(),
		textureUploadHeap.Get(),
		0, 0, 1, &textureData);

	wkp_graphicDevice.lock()->GetCommandList().ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			texture.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
		)
	);
	image.pixels.clear();
	dataRefresh = false;
}
size_t __cdecl ButiEngine::Resource_Texture_Dx12::TexMetadata::ComputeIndex(size_t mip, size_t item, size_t slice) const
{
	if (mip >= mipLevels)
		return size_t(-1);

	switch (dimension)
	{
	case TEX_DIMENSION_TEXTURE1D:
	case TEX_DIMENSION_TEXTURE2D:
		if (slice > 0)
			return size_t(-1);

		if (item >= arraySize)
			return size_t(-1);

		return (item * (mipLevels)+mip);

	case TEX_DIMENSION_TEXTURE3D:
		if (item > 0)
		{
			// No support for arrays of volumes
			return size_t(-1);
		}
		else
		{
			size_t index = 0;
			size_t d = depth;

			for (size_t level = 0; level < mip; ++level)
			{
				index += d;
				if (d > 1)
					d >>= 1;
			}

			if (slice >= d)
				return size_t(-1);

			index += slice;

			return index;
		}

	default:
		return size_t(-1);
	}
}