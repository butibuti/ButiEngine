#pragma once
#include"GraphicDevice.h"
#include <D3dx12.h>
namespace ButiEngine {
	class DescriptorHeapManager;
	class PipelineStateManager;

	static const unsigned char SamplerTableRegion = 1;
	class IApplication;
	class GraphicDevice_Dx12 :public GraphicDevice
	{
	public:

		GraphicDevice_Dx12(std::weak_ptr<IApplication> arg_wkp_application);
		void Initialize()override;

		void PreInitialize()override;

		void Release()override;

		void ClearDepthStancil(const float arg_depth=1.0f) override;

		ID3D12Device& GetDevice();

		ID3D12CommandQueue& GetCommandQueue();

		ID3D12CommandAllocator& GetCommandAllocator();

		ID3D12CommandAllocator& GetCommandAllocator(const UINT arg_index);

		ID3D12CommandAllocator& GetBundleCommandAllocator();

		ID3D12GraphicsCommandList& GetCommandList();

		ID3D12GraphicsCommandList& GetUploadCommandList();

		ID3D12GraphicsCommandList& GetClearCommandList();

		std::pair< Microsoft::WRL::ComPtr<ID3D12RootSignature>, D3D12_ROOT_SIGNATURE_DESC> GetRootSignature(const std::wstring& Key);

		std::weak_ptr<DescriptorHeapManager> GetDescriptorHeapManager();

		PipelineStateManager& GetPipelineStateManager();

		void SetRootSignature(const std::wstring& Key, const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootsig, const D3D12_ROOT_SIGNATURE_DESC& arg_desc);

		void AddResource(Resource* rarg_resource);

		void GraphicsCommandListReset();

		void ResourceUploadRelease()override;

		void UnSetCommandList();

		ID3D12Fence& GetFence();

		IDXGISwapChain& GetSwapChain();

		void WaitGPU();

		void Update() override;

		void Set();
		void SetGUICommand();

		void ClearWindow() override;

		void SetCommandList(ID3D12GraphicsCommandList* arg_currentCommandList);
		void SetDefaultRenderTarget()override;
		void CommandList_SetScissorRect();

		void CommandList_SetRenderTargetView();
		void CommandList_SetRenderTargetViewWithoutDepth();

		void InsertCommandList();

		void ResourceUpload()override;

		void DrawStart()override;

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetRtvHeap() const;

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetDsvHeap() const;

		ID3D12Resource& GetTextureUploadHeap();

		CD3DX12_CPU_DESCRIPTOR_HANDLE GetRtvHandle() const;

		CD3DX12_CPU_DESCRIPTOR_HANDLE GetDsvHandle() const;

		const D3D12_RECT& GetScissorRect()const;

		UINT GetFrameCount() {
			return FrameCount;
		}

		UINT GetFrameIndex() {
			return frameIndex;
		}

	private:

		static const UINT FrameCount = 3;
		Microsoft::WRL::ComPtr<ID3D12Device> device;

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator[FrameCount];

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> bundleCommandAllocator;

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> clearCommandList;

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> presentCommandList;

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> uploadCommandList;
		std::vector<Resource*> vec_uploadResources;

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> drawCommandList;

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> guiCommandList;

		std::shared_ptr<DescriptorHeapManager> shp_descripterManager;

		std::shared_ptr<PipelineStateManager> shp_pipelineStateManager;

		ID3D12GraphicsCommandList* currentCommandList;

		std::vector< ID3D12CommandList*> vec_drawCommandLists;
		Microsoft::WRL::ComPtr<ID3D12Fence>  fence;

		std::map<std::wstring, std::pair<Microsoft::WRL::ComPtr<ID3D12RootSignature>, D3D12_ROOT_SIGNATURE_DESC>> map_rootSignature;
		Microsoft::WRL::ComPtr<ID3D12Resource> renderTargets[FrameCount];

		Microsoft::WRL::ComPtr<ID3D12Resource> textureUploadHeap;

		D3D12_RECT     scissorRect;

		Microsoft::WRL::ComPtr<ID3D12Resource> depthStencil;
		UINT depthDescriptorSize;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>  renderTargetDescripterHeap;
		UINT   renderTargetDescriptorSize;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> depthStencilDescriptorHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE rtv_handle[FrameCount];


		Microsoft::WRL::ComPtr<ID3D12RootSignature>  rootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;


		Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain3;
		UINT frameIndex;
		HANDLE  fenceEvent;
		UINT64  fenceValue;
	};
}