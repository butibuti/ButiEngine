#pragma once
#include"stdafx.h"
#include"Resource_Texture.h"
namespace ButiEngine {
	class GraphicDevice_Dx12;
	class Resource_Texture_Dx12 :public Resource_Texture
	{
		struct CUSTAM_SUBRESOURCE_DATA
		{
			const void* pSysMem;
			UINT SysMemPitch;
			UINT SysMemSlicePitch;
		};
		enum  TEX_DIMENSION
		{
			TEX_DIMENSION_TEXTURE1D = 2,
			TEX_DIMENSION_TEXTURE2D = 3,
			TEX_DIMENSION_TEXTURE3D = 4,
		}; 
		enum TEX_MISC_FLAG
		// Subset here matches D3D10_RESOURCE_MISC_FLAG and D3D11_RESOURCE_MISC_FLAG
		{
			TEX_MISC_TEXTURECUBE = 0x4L,
		};

		enum TEX_MISC_FLAG2
		{
			TEX_MISC2_ALPHA_MODE_MASK = 0x7L,
		};

		enum TEX_ALPHA_MODE
			// Matches DDS_ALPHA_MODE, encoded in MISC_FLAGS2
		{
			TEX_ALPHA_MODE_UNKNOWN = 0,
			TEX_ALPHA_MODE_STRAIGHT = 1,
			TEX_ALPHA_MODE_PREMULTIPLIED = 2,
			TEX_ALPHA_MODE_OPAQUE = 3,
			TEX_ALPHA_MODE_CUSTOM = 4,
		};
	public:
		struct TexMetadata
		{
			size_t          width;
			size_t          height;     // Should be 1 for 1D textures
			size_t          depth;      // Should be 1 for 1D or 2D textures
			size_t          arraySize;  // For cubemap, this is a multiple of 6
			size_t          mipLevels;
			uint32_t        miscFlags;
			uint32_t        miscFlags2;
			DXGI_FORMAT     format;
			TEX_DIMENSION   dimension;

			size_t __cdecl ComputeIndex(_In_ size_t mip, _In_ size_t item, _In_ size_t slice) const;
			// Returns size_t(-1) to indicate an out-of-range error

			bool __cdecl IsCubemap() const { return (miscFlags & TEX_MISC_TEXTURECUBE) != 0; }
			// Helper for miscFlags

			bool __cdecl IsPMAlpha() const { return ((miscFlags2 & TEX_MISC2_ALPHA_MODE_MASK) == TEX_ALPHA_MODE_PREMULTIPLIED) != 0; }
			void __cdecl SetAlphaMode(TEX_ALPHA_MODE mode) { miscFlags2 = (miscFlags2 & ~static_cast<uint32_t>(TEX_MISC2_ALPHA_MODE_MASK)) | static_cast<uint32_t>(mode); }
			TEX_ALPHA_MODE __cdecl GetAlphaMode() const { return static_cast<TEX_ALPHA_MODE>(miscFlags2 & TEX_MISC2_ALPHA_MODE_MASK); }
			// Helpers for miscFlags2

			bool __cdecl IsVolumemap() const { return (dimension == TEX_DIMENSION_TEXTURE3D); }
			// Helper for dimension
		};

		struct Image
		{
			size_t      width;
			size_t      height;
			DXGI_FORMAT format;
			size_t      rowPitch;
			size_t      slicePitch;
			std::vector<uint8_t>    pixels;
		};
		Resource_Texture_Dx12(const std::vector<BYTE>& buffer, int width, int height, std::shared_ptr<GraphicDevice> arg_graphicDevice);
		~Resource_Texture_Dx12() {
		}
		void SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice) override;
		void Attach(int slot)override;
		void CreateTexture(Image* srcImages, size_t nimages, const TexMetadata& metadata);
		virtual void CreateTextureUploadHeap();
		void Initialize()override;
		Microsoft::WRL::ComPtr<ID3D12Resource> GetTexture();
		Microsoft::WRL::ComPtr<ID3D12Resource> GetTextureUploadHeap();
		D3D12_RESOURCE_DESC GetTextureDesc();
	 protected:
		 Resource_Texture_Dx12() {
		 }
		void UpdateResourceRelease()override;
		void ResourceUpdate() override;
		CD3DX12_GPU_DESCRIPTOR_HANDLE handle;
		Image image;
		//テクスチャ用のデスクプリタ
		D3D12_RESOURCE_DESC textureResDesc;
		UINT texturePixelSize;
		//リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> texture;
		//GPUアップロードのためのリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> textureUploadHeap;
		//テクスチャファイルへのパス
		std::wstring fileName{ L"" };
		bool dataRefresh = true;

		std::weak_ptr<GraphicDevice_Dx12> wkp_graphicDevice;
	};

}