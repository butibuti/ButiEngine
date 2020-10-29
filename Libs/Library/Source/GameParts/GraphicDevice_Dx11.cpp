#include "stdafx.h"
#include "..\..\Header\GameParts\GraphicDevice_Dx11.h"



ButiEngine::GraphicDevice_Dx11::GraphicDevice_Dx11(std::weak_ptr<Application> arg_wkp_application)
{
	wkp_application = arg_wkp_application;
}

void ButiEngine::GraphicDevice_Dx11::Initialize()
{
	std::vector<D3D_DRIVER_TYPE> driverTypes
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
		D3D_DRIVER_TYPE_SOFTWARE,
	};

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = wkp_application.lock()->GetWindow()->GetSize().x;
	swapChainDesc.BufferDesc.Height = wkp_application.lock()->GetWindow()->GetSize().y;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = wkp_application.lock()->GetWindow()->GetHandle();
	swapChainDesc.Windowed = true;

	ID3D11Texture2D* hpTexture2dDepth = NULL;
	D3D11_TEXTURE2D_DESC hTexture2dDesc;
	hTexture2dDesc.Width = swapChainDesc.BufferDesc.Width;
	hTexture2dDesc.Height = swapChainDesc.BufferDesc.Height;
	hTexture2dDesc.MipLevels = 1;
	hTexture2dDesc.ArraySize = 1;
	hTexture2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	hTexture2dDesc.SampleDesc = swapChainDesc.SampleDesc;
	hTexture2dDesc.Usage = D3D11_USAGE_DEFAULT;
	hTexture2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hTexture2dDesc.CPUAccessFlags = 0;
	hTexture2dDesc.MiscFlags = 0;

	const D3D_FEATURE_LEVEL FeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,		
		//D3D_FEATURE_LEVEL_10_1,	
		//D3D_FEATURE_LEVEL_10_0,	
	};
	UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);
	D3D_FEATURE_LEVEL level;
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		FeatureLevels,
		NumFeatureLevels,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&swapChain,
		&device,
		&level,
		&context
	);

	(device->CreateTexture2D(&hTexture2dDesc, NULL, &hpTexture2dDepth));


	//ステンシルターゲット作成
	hpDepthStencilView = NULL;
	D3D11_DEPTH_STENCIL_VIEW_DESC hDepthStencilViewDesc;
	hDepthStencilViewDesc.Format = hTexture2dDesc.Format;
	hDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	hDepthStencilViewDesc.Flags = 0;
	(device->CreateDepthStencilView(hpTexture2dDepth, &hDepthStencilViewDesc, &hpDepthStencilView));
	//HRESULT result;




	CoCreateInstance(CLSID_WICImagingFactory, nullptr,
		CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(imageFactory.GetAddressOf()));

	//標準
	{
		RasterizerStateData data{ CullMode::back,true };

		auto  rasterizerState = CreateRasterizerState(data);

		map_rasterizerStates.emplace(data, rasterizerState);

		nowRasterizeStateData = data;
		GetContext().RSSetState(rasterizerState.Get());
	}
	{
		RasterizerStateData data{ CullMode::back,false };

		auto  rasterizerState = CreateRasterizerState(data);

		map_rasterizerStates.emplace(data, rasterizerState);
	}
	{
		RasterizerStateData data{ CullMode::front,true };

		auto  rasterizerState = CreateRasterizerState(data);

		map_rasterizerStates.emplace(data, rasterizerState);
	}
	{
		RasterizerStateData data{ CullMode::front,false };

		auto  rasterizerState = CreateRasterizerState(data);

		map_rasterizerStates.emplace(data, rasterizerState);
	}
	{
		RasterizerStateData data{ CullMode::none,true };

		auto  rasterizerState = CreateRasterizerState(data);

		map_rasterizerStates.emplace(data, rasterizerState);
	}
	{
		RasterizerStateData data{ CullMode::none,false };

		auto  rasterizerState = CreateRasterizerState(data);

		map_rasterizerStates.emplace(data, rasterizerState);
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture = nullptr;
	GetSwapChain().GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(texture.GetAddressOf()));

	GetDevice().CreateRenderTargetView(texture.Get(), nullptr, renderTargetView.GetAddressOf());

}

void ButiEngine::GraphicDevice_Dx11::PreInitialize()
{
}

ID3D11Device & ButiEngine::GraphicDevice_Dx11::GetDevice()
{
	return *device.Get();
}

void ButiEngine::GraphicDevice_Dx11::ClearDepthStancil(const float arg_depth)
{
	GetContext().ClearDepthStencilView(hpDepthStencilView, D3D11_CLEAR_DEPTH ,arg_depth, 0);
}

ID3D11DeviceContext & ButiEngine::GraphicDevice_Dx11::GetContext()
{
	return *context.Get();
}

IDXGISwapChain & ButiEngine::GraphicDevice_Dx11::GetSwapChain()
{
	return *swapChain.Get();
}

ButiEngine::GraphicDevice_Dx11::~GraphicDevice_Dx11()
{
	for (int i = 0; i < vec_descs.size(); i++) {
		delete vec_descs.at(i);
	}
}

Microsoft::WRL::ComPtr<ID3D11RasterizerState> ButiEngine::GraphicDevice_Dx11::CreateRasterizerState(const RasterizerStateData& arg_data)
{
	D3D11_RASTERIZER_DESC* rasterDesc=new D3D11_RASTERIZER_DESC();
	if (arg_data.isFill)
		rasterDesc->FillMode =D3D11_FILL_MODE:: D3D11_FILL_SOLID;
	else
		rasterDesc->FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc->CullMode =(D3D11_CULL_MODE)arg_data.cullMode;


	Microsoft::WRL::ComPtr<ID3D11RasterizerState> out;
	GetDevice().CreateRasterizerState(rasterDesc, out.GetAddressOf());

	vec_descs.push_back(rasterDesc);

	return out;
}

void ButiEngine::GraphicDevice_Dx11::SetRasterizerState(const RasterizerStateData& rasterDesc)
{
	if (nowRasterizeStateData == rasterDesc)
		return;

	nowRasterizeStateData = rasterDesc;
	GetContext().RSSetState(map_rasterizerStates.at(rasterDesc).Get());
}





void ButiEngine::GraphicDevice_Dx11::Update()
{
	swapChain->Present(1, 0);
}

void ButiEngine::GraphicDevice_Dx11::Release()
{
	context->ClearState();
	context->Flush();
}

void ButiEngine::GraphicDevice_Dx11::ClearWindow()
{
	GetContext().ClearRenderTargetView(renderTargetView.Get(), clearColor);
	GetContext().ClearDepthStencilView(hpDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	GetContext().OMSetRenderTargets(1, renderTargetView.GetAddressOf(),hpDepthStencilView);


}
