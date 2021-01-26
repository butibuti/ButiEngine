#include"stdafx.h"
#include"Header/Device/PipelineStateManager.h"
#include"Header/Device/GraphicResourceUtil_Dx12.h"
#include"Header/Device/DescriptorHeapManager.h"
#include "..\..\Header\GameParts\GraphicDevice_Dx12.h"
ButiEngine::GraphicDevice_Dx12::GraphicDevice_Dx12(std::weak_ptr<IApplication> arg_wkp_application)
{
	wkp_application = arg_wkp_application;
}
void ButiEngine::GraphicDevice_Dx12::Initialize()
{
	HRESULT hr;


	Microsoft::WRL::ComPtr<IDXGIFactory4> factory4;
	hr = CreateDXGIFactory1(IID_PPV_ARGS(factory4.GetAddressOf()));


	IDXGIAdapter1* p_adapter=nullptr;
	{

		std::vector<IDXGIAdapter1*> vec_p_adapters;

		for (int i = 0; factory4->EnumAdapters1(i, &p_adapter) != DXGI_ERROR_NOT_FOUND; i++) {
			vec_p_adapters.push_back(p_adapter);
		}

		for (int  i = vec_p_adapters.size()-1;i>=0 ; i--) {
			DXGI_ADAPTER_DESC1 adapterDesc;
			vec_p_adapters.at(i)->GetDesc1(&adapterDesc);

			auto name = adapterDesc.Description;
			if (!StringHelper::Contains(name, L"Microsoft")&& !StringHelper::Contains(name, L"Intel")) {
				p_adapter = vec_p_adapters.at(i);
				break;
			}
		}
	}

	D3D_FEATURE_LEVEL level;
	{

		D3D_FEATURE_LEVEL levels[] = {
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};
		for (int i = 0; i < sizeof(levels) / sizeof(D3D_FEATURE_LEVEL); i++) {

			hr = D3D12CreateDevice(nullptr, levels[i], IID_PPV_ARGS(device.GetAddressOf()));
			if (hr == S_OK) {
				level = levels[i];
				break;
			}
		}
	}

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
	ZeroMemory(&commandQueueDesc, sizeof(commandQueueDesc));
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	hr = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(commandQueue.GetAddressOf()));




	DXGI_SWAP_CHAIN_DESC swapChaindesc = {};
	swapChaindesc.BufferCount = FrameCount;
	swapChaindesc.BufferDesc.Width = wkp_application.lock()->GetWindow()->GetSize().x;
	swapChaindesc.BufferDesc.Height = wkp_application.lock()->GetWindow()->GetSize().y;
	swapChaindesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChaindesc.OutputWindow = wkp_application.lock()->GetWindow()->GetHandle();
	swapChaindesc.Windowed = TRUE;
	swapChaindesc.SampleDesc.Count = 1;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	// スワップチェインを生成.
	hr = factory4->CreateSwapChain(commandQueue.Get(), &swapChaindesc, swapChain.GetAddressOf());
	if (FAILED(hr))
	{
		return;
	}

	// IDXGISwapChain3にする.
	hr = swapChain->QueryInterface(IID_PPV_ARGS(swapChain3.GetAddressOf()));
	if (FAILED(hr))
	{
		return;
	}

	// フレームバッファ番号を設定.
	frameIndex = swapChain3->GetCurrentBackBufferIndex();



	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = FrameCount;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;


	hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(renderTargetDescripterHeap.GetAddressOf()));
	if (FAILED(hr))
	{
	}

	renderTargetDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	{

	}

	// 深度ステンシルビュー用ディスクリプタヒープの設定.
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	// 深度ステンシルビュー用ディスクリプタヒープを生成.

	hr = device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(depthStencilDescriptorHeap.GetAddressOf()));
	if (FAILED(hr))
	{
	}
	// 深度ステンシルビューのディスクリプタサイズを取得.

	depthDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);


	// 深度ステンシルビューを生成.
	{

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
		desc.Width = wkp_application.lock()->GetWindow()->GetSize().x;
		desc.Height = wkp_application.lock()->GetWindow()->GetSize().y;
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
		hr = device->CreateCommittedResource(
			&prop,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(depthStencil.GetAddressOf()));
		if (FAILED(hr))
		{
		} // 深度ステンシルビューの設定
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

		// 深度ステンシルビューを生成.
		device->CreateDepthStencilView(depthStencil.Get(), &dsvDesc, depthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	}

	auto handle = renderTargetDescripterHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewdesc = {};
	renderTargetViewdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewdesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewdesc.Texture2D.MipSlice = 0;
	renderTargetViewdesc.Texture2D.PlaneSlice = 0;

	for (UINT i = 0; i < FrameCount; i++)
	{
		HRESULT hr;
		// バックバッファ取得.
		hr = swapChain3->GetBuffer(i, IID_PPV_ARGS(renderTargets[i].GetAddressOf()));
		if (FAILED(hr))
		{
		}
		// レンダーターゲットビューを生成.
		device->CreateRenderTargetView(renderTargets[i].Get(), &renderTargetViewdesc, handle);
		handle.ptr += renderTargetDescriptorSize;

		hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator[i].GetAddressOf()));

	}
	hr = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(bundleCommandAllocator.GetAddressOf()));

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(0, nullptr, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	Microsoft::WRL::ComPtr<ID3DBlob> signature;
	Microsoft::WRL::ComPtr<ID3DBlob> error;

	D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
	device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));

	//clear処理用
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator[frameIndex].Get(), nullptr, IID_PPV_ARGS(clearCommandList.GetAddressOf()));

	clearCommandList->Close();

	//presentBarrier処理用
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator[frameIndex].Get(), nullptr, IID_PPV_ARGS(presentCommandList.GetAddressOf()));

	hr = presentCommandList->Close();

	//upload処理用
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator[frameIndex].Get(), nullptr, IID_PPV_ARGS(uploadCommandList.GetAddressOf()));

	hr = uploadCommandList->Close();
	//draw処理用
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator[frameIndex].Get(), nullptr, IID_PPV_ARGS(drawCommandList.GetAddressOf()));

	hr = drawCommandList->Close();
	//gui処理用
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator[frameIndex].Get(), nullptr, IID_PPV_ARGS(guiCommandList.GetAddressOf()));

	hr = guiCommandList->Close();

	// シザー矩形を設定.
	scissorRect.left = 0;
	scissorRect.right = wkp_application.lock()->GetWindow()->GetSize().x;
	scissorRect.top = 0;
	scissorRect.bottom = wkp_application.lock()->GetWindow()->GetSize().y;


	// フェンスを生成.
	hr = device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.GetAddressOf()));
	if (FAILED(hr))
	{

	}

	// フェンス用イベントを生成.
	fenceValue = 1;
	fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
	if (fenceEvent == nullptr)
	{
	}

	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr,
		CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(imageFactory.GetAddressOf()));

	shp_descripterManager = std::make_shared<DescriptorHeapManager>(GetThis<GraphicDevice_Dx12>());
	shp_descripterManager->Initialize(*device.Get());

	shp_pipelineStateManager = ObjectFactory::Create<PipelineStateManager>(GetThis<GraphicDevice_Dx12>());
}

void ButiEngine::GraphicDevice_Dx12::PreInitialize()
{
}

void ButiEngine::GraphicDevice_Dx12::Release()
{
	shp_descripterManager = nullptr;
}

void ButiEngine::GraphicDevice_Dx12::ClearDepthStancil(const float arg_depth )
{
	currentCommandList->ClearDepthStencilView(depthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, arg_depth, 0, 0, nullptr);

}

ID3D12Device& ButiEngine::GraphicDevice_Dx12::GetDevice()
{
	return *device.Get();
}

ID3D12CommandQueue& ButiEngine::GraphicDevice_Dx12::GetCommandQueue()
{
	return *commandQueue.Get();
}

ID3D12CommandAllocator& ButiEngine::GraphicDevice_Dx12::GetCommandAllocator()
{
	return *commandAllocator[FrameCount - 1].Get();
}

ID3D12CommandAllocator& ButiEngine::GraphicDevice_Dx12::GetCommandAllocator(const UINT arg_index)
{
	return *commandAllocator[arg_index].Get();
}

ID3D12CommandAllocator& ButiEngine::GraphicDevice_Dx12::GetBundleCommandAllocator()
{
	return *bundleCommandAllocator.Get();
}

ID3D12GraphicsCommandList& ButiEngine::GraphicDevice_Dx12::GetCommandList()
{
	return *currentCommandList;
}

ID3D12GraphicsCommandList& ButiEngine::GraphicDevice_Dx12::GetUploadCommandList()
{
	return *uploadCommandList.Get();
}


ID3D12GraphicsCommandList& ButiEngine::GraphicDevice_Dx12::GetClearCommandList()
{
	return *clearCommandList.Get();
}

std::pair<Microsoft::WRL::ComPtr<ID3D12RootSignature>, D3D12_ROOT_SIGNATURE_DESC> ButiEngine::GraphicDevice_Dx12::GetRootSignature(const std::wstring& Key)
{
	auto it = map_rootSignature.count(Key);
	if (!it) {
		return { nullptr, D3D12_ROOT_SIGNATURE_DESC() };
	}
	return map_rootSignature[Key];
}

std::weak_ptr< ButiEngine::DescriptorHeapManager> ButiEngine::GraphicDevice_Dx12::GetDescriptorHeapManager()
{
	return shp_descripterManager;
}

ButiEngine::PipelineStateManager& ButiEngine::GraphicDevice_Dx12::GetPipelineStateManager()
{
	return *shp_pipelineStateManager;
}

void ButiEngine::GraphicDevice_Dx12::SetRootSignature(const std::wstring& Key, const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootsig, const D3D12_ROOT_SIGNATURE_DESC& arg_desc)
{
	if (GetRootSignature(Key).first != nullptr) {
		throw ButiException(
			L"すでにそのルートシグネチャは存在します",
			Key,
			L"DeviceResources::SetRootSignature()"
		);
	}
	map_rootSignature[Key] = { rootsig ,arg_desc };
}

void ButiEngine::GraphicDevice_Dx12::AddResource(Resource* arg_resource)
{
	vec_uploadResources.push_back(arg_resource);
}

void ButiEngine::GraphicDevice_Dx12::GraphicsCommandListReset()
{
}

void ButiEngine::GraphicDevice_Dx12::ResourceUploadRelease()
{
	for (auto itr = vec_uploadResources.begin(); itr != vec_uploadResources.end(); itr++) {
		(*itr)->UpdateResourceRelease();
	}
	vec_uploadResources.clear();
}

void ButiEngine::GraphicDevice_Dx12::UnSetCommandList()
{
	currentCommandList = nullptr;
}

ID3D12Fence& ButiEngine::GraphicDevice_Dx12::GetFence()
{
	return *fence.Get();
}

IDXGISwapChain& ButiEngine::GraphicDevice_Dx12::GetSwapChain()
{
	return *swapChain3.Get();
}

void ButiEngine::GraphicDevice_Dx12::WaitGPU()
{

	const UINT64 fenceV = fenceValue;
	auto hr = commandQueue->Signal(fence.Get(), fenceV);

	if (hr != S_OK) {
		auto reason = device->GetDeviceRemovedReason();
		int i = 0;
	}
	fenceValue++;
	// Wait until the previous frame is finished.
	if (fence->GetCompletedValue() < fenceV)
	{
		hr = fence->SetEventOnCompletion(fenceV, fenceEvent);
		WaitForSingleObject(fenceEvent, INFINITE);

		if (hr != S_OK) {
			auto reason = device->GetDeviceRemovedReason();
			int i = 0;
		}
	}

	frameIndex = swapChain3->GetCurrentBackBufferIndex();

}


void ButiEngine::GraphicDevice_Dx12::Update()
{
	auto hr = presentCommandList->Reset(commandAllocator[frameIndex].Get(), pipelineState.Get());

	if (hr != S_OK) {
		auto reason = device->GetDeviceRemovedReason();
		int i = 0;
	}
	//プレゼント用のバリアを張る
	auto desc = CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	presentCommandList->ResourceBarrier(1,
		&desc);

	hr = presentCommandList->Close();
	SetCommandList(presentCommandList.Get());
	InsertCommandList();

	if (hr != S_OK) {
		auto reason = device->GetDeviceRemovedReason();
		int i = 0;
	}
	// Execute the command list.


	commandQueue->ExecuteCommandLists((UINT)vec_drawCommandLists.size(), &(vec_drawCommandLists[0]));


	// Present the frame.
	hr = swapChain3->Present(1, 0);

	if (hr != S_OK) {
		auto reason = device->GetDeviceRemovedReason();
		int i = 0;
	}
	WaitGPU();
	vec_drawCommandLists.clear();

}

void ButiEngine::GraphicDevice_Dx12::Set()
{
	CommandListHelper::Close(drawCommandList);
	InsertCommandList();

	commandQueue->ExecuteCommandLists((UINT)vec_drawCommandLists.size(), &(vec_drawCommandLists[0]));

	WaitGPU();
	vec_drawCommandLists.clear();
}

void ButiEngine::GraphicDevice_Dx12::SetGUICommand()
{
	CommandListHelper::Close(guiCommandList);
	InsertCommandList();
}

void ButiEngine::GraphicDevice_Dx12::ClearWindow()
{
	commandAllocator[frameIndex]->Reset();

	clearCommandList->Reset(commandAllocator[frameIndex].Get(), pipelineState.Get());

	clearCommandList->SetGraphicsRootSignature(rootSignature.Get());

	// Indicate that the back FrameCount will be used as a render target.
	auto desc = CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	clearCommandList->ResourceBarrier(1,
		&desc);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(renderTargetDescripterHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, renderTargetDescriptorSize);
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(depthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	clearCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
	// Record commands.

	clearCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	clearCommandList->ClearDepthStencilView(depthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	clearCommandList->Close();
	SetCommandList(clearCommandList.Get());
	InsertCommandList();
}

void ButiEngine::GraphicDevice_Dx12::SetCommandList(ID3D12GraphicsCommandList* arg_currentCommandList)
{
	if (currentCommandList) {
		throw ButiException(L"commandList is already seted.", L"if (!currentCommandList)", L"ButiEngine::GraphicDevice_Dx12::SetCommandList");
	}
	currentCommandList = arg_currentCommandList;
}

void ButiEngine::GraphicDevice_Dx12::SetDefaultRenderTarget()
{
	CommandList_SetScissorRect();
	CommandList_SetRenderTargetView();
}

void ButiEngine::GraphicDevice_Dx12::CommandList_SetScissorRect()
{
	currentCommandList->RSSetScissorRects(1, &scissorRect);
}

void ButiEngine::GraphicDevice_Dx12::CommandList_SetRenderTargetView()
{

	//レンダーターゲットビューのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
		renderTargetDescripterHeap->GetCPUDescriptorHandleForHeapStart(),
		GetFrameIndex(),
		renderTargetDescriptorSize);
	//デプスステンシルビューのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
		depthStencilDescriptorHeap->GetCPUDescriptorHandleForHeapStart()
	);
	//取得したハンドルをセット
	currentCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);


}

void ButiEngine::GraphicDevice_Dx12::CommandList_SetRenderTargetViewWithoutDepth()
{
	//レンダーターゲットビューのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
		renderTargetDescripterHeap->GetCPUDescriptorHandleForHeapStart(),
		GetFrameIndex(),
		renderTargetDescriptorSize);
	//取得したハンドルをセット
	currentCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
}

void ButiEngine::GraphicDevice_Dx12::InsertCommandList()
{
	if (!currentCommandList) {
		return;
	}
	vec_drawCommandLists.push_back(currentCommandList);
	currentCommandList = nullptr;
}

void ButiEngine::GraphicDevice_Dx12::ResourceUpload()
{
	if (!vec_uploadResources.size()) {
		return;
	}
	CommandListHelper::Reset(uploadCommandList,GetThis<GraphicDevice_Dx12>());

	SetCommandList(uploadCommandList.Get());

	for (auto itr = vec_uploadResources.begin(); itr != vec_uploadResources.end(); itr++) {
		(*itr)->ResourceUpdate();
	}
	CommandListHelper::Close(uploadCommandList);
	InsertCommandList();
}

void ButiEngine::GraphicDevice_Dx12::DrawStart()
{
	CommandListHelper::Reset(pipelineState, drawCommandList, GetThis<GraphicDevice_Dx12>());
	SetCommandList(drawCommandList.Get());
	ID3D12DescriptorHeap* ppHeaps[] = { shp_descripterManager->GetDescriptorHeap().Get(), shp_descripterManager->GetSamplerHeap().Get() };
	auto heapCount = _countof(ppHeaps);
	currentCommandList->SetDescriptorHeaps(heapCount, ppHeaps);

}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ButiEngine::GraphicDevice_Dx12::GetRtvHeap() const
{
	return  renderTargetDescripterHeap;
}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ButiEngine::GraphicDevice_Dx12::GetDsvHeap() const
{
	return depthStencilDescriptorHeap;
}

ID3D12Resource& ButiEngine::GraphicDevice_Dx12::GetTextureUploadHeap()
{
	return *textureUploadHeap.Get();
}

CD3DX12_CPU_DESCRIPTOR_HANDLE ButiEngine::GraphicDevice_Dx12::GetRtvHandle() const
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
		GetRtvHeap()->GetCPUDescriptorHandleForHeapStart(),
		frameIndex,
		renderTargetDescriptorSize);
	return rtvHandle;
}

CD3DX12_CPU_DESCRIPTOR_HANDLE ButiEngine::GraphicDevice_Dx12::GetDsvHandle() const
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
		GetDsvHeap()->GetCPUDescriptorHandleForHeapStart()
	);
	return dsvHandle;
}

const D3D12_RECT& ButiEngine::GraphicDevice_Dx12::GetScissorRect() const
{
	return scissorRect;
}