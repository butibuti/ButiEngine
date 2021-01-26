#pragma once
#include"stdafx.h"
#include"../GameParts/GraphicDevice_Dx12.h"
#include"../Resources/Resource_Shader_Dx12.h"
namespace ButiEngine {
	namespace PipelineStateHelper {
		static inline Microsoft::WRL::ComPtr<ID3D12PipelineState> CreateDirect(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc, std::shared_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) {

			Microsoft::WRL::ComPtr<ID3D12PipelineState> Ret;
			auto hr = arg_wkp_graphicDevice->GetDevice().CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&Ret));

			return Ret;
		}


		struct ButiD3DX12_BLEND_DESC : public D3D12_BLEND_DESC
		{
			explicit ButiD3DX12_BLEND_DESC(const D3D12_BLEND_DESC& desc) :
				D3D12_BLEND_DESC(desc)
			{}
			explicit ButiD3DX12_BLEND_DESC()
			{
				AlphaToCoverageEnable = TRUE;
				IndependentBlendEnable = FALSE;
				const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
				{
					TRUE,FALSE,
					D3D12_BLEND_ONE, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
					D3D12_BLEND_ONE, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
					D3D12_LOGIC_OP_NOOP,
					D3D12_COLOR_WRITE_ENABLE_ALL,
				};
				for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
					RenderTarget[i] = defaultRenderTargetBlendDesc;
			}
			ButiD3DX12_BLEND_DESC(const BlendMode& arg_blend)
			{
				AlphaToCoverageEnable = FALSE;
				IndependentBlendEnable = FALSE;
				D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc;

				switch (arg_blend) {
				case BlendMode::AlphaBlend:
					defaultRenderTargetBlendDesc = { TRUE, FALSE,
					D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
					D3D12_BLEND_ONE, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
					D3D12_LOGIC_OP_AND_REVERSE,
					D3D12_COLOR_WRITE_ENABLE_ALL, };
					break;
				case BlendMode::Addition:
					defaultRenderTargetBlendDesc = { TRUE, FALSE,
				D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_ONE, D3D12_BLEND_OP_ADD,
				D3D12_BLEND_ONE, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
				D3D12_LOGIC_OP_NOOP,
				D3D12_COLOR_WRITE_ENABLE_ALL, };
					break;
				case BlendMode::Subtruction:
					defaultRenderTargetBlendDesc = { TRUE, FALSE,
				D3D12_BLEND_INV_SRC_COLOR, D3D12_BLEND_ONE, D3D12_BLEND_OP_ADD,
				D3D12_BLEND_ONE, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
				D3D12_LOGIC_OP_NOOP,
				D3D12_COLOR_WRITE_ENABLE_ALL, };
					break;
				case BlendMode::Reverse:
					defaultRenderTargetBlendDesc = { TRUE, FALSE,
				D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_ONE, D3D12_BLEND_OP_REV_SUBTRACT,
				D3D12_BLEND_ONE, D3D12_BLEND_INV_SRC_ALPHA, D3D12_BLEND_OP_SUBTRACT,
				D3D12_LOGIC_OP_NOOP,
				D3D12_COLOR_WRITE_ENABLE_ALL, };
					break;
				case BlendMode::NoBlend:
					//AlphaToCoverageEnable = TRUE;
					defaultRenderTargetBlendDesc = { FALSE, FALSE,
				D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
				D3D12_BLEND_ZERO, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
				D3D12_LOGIC_OP_NOOP,
				D3D12_COLOR_WRITE_ENABLE_ALL, };
					break;
				}
				for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
					RenderTarget[i] = defaultRenderTargetBlendDesc;
			}
		};
		static inline Microsoft::WRL::ComPtr<ID3D12PipelineState> CreateDefault3D(const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature, D3D12_GRAPHICS_PIPELINE_STATE_DESC& RetDesc, D3D12_RASTERIZER_DESC& arg_rasteriserDesc, std::shared_ptr<IResource_Shader> arg_shader, const BlendMode arg_BlendMode, const TopologyType arg_topologyType, std::shared_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice,const bool isDepth=true) {


			//ZeroMemory(&RetDesc, sizeof(RetDesc));
			RetDesc.InputLayout = { arg_shader->GetVertexShader().lock()->GetThis<Resource_VertexShader_Dx12>()->GetInputLayoutVector().data(),
				(UINT)arg_shader->GetVertexShader().lock()->GetThis<Resource_VertexShader_Dx12>()->GetInputLayoutVector().size() };
			RetDesc.pRootSignature = rootSignature.Get();
			RetDesc.VS =
			{
				reinterpret_cast<UINT8*>(arg_shader->GetVertexShader().lock()->GetThis<Resource_VertexShader_Dx12>()->GetVertexShaderBlob()->GetBufferPointer()),
				arg_shader->GetVertexShader().lock()->GetThis<Resource_VertexShader_Dx12>()->GetVertexShaderBlob()->GetBufferSize()
			};
			RetDesc.PS =
			{
				reinterpret_cast<UINT8*>(arg_shader->GetPixelShader().lock()->GetThis<Resource_PixelShader_Dx12>()->GetPixelShaderBlob()->GetBufferPointer()),
				arg_shader->GetPixelShader().lock()->GetThis<Resource_PixelShader_Dx12>()->GetPixelShaderBlob()->GetBufferSize()
			};
			if (arg_shader->GetIsContainGeometryShader()) {
				RetDesc.GS = {
					reinterpret_cast<UINT8*>(arg_shader->GetGeometryShader().lock()->GetThis<Resource_GeometryShader_Dx12>()->GetGeometryShaderBlob()->GetBufferPointer()),
					arg_shader->GetGeometryShader().lock()->GetThis<Resource_GeometryShader_Dx12>()->GetGeometryShaderBlob()->GetBufferSize()
				};
			}
			RetDesc.RasterizerState = arg_rasteriserDesc;
			RetDesc.BlendState = ButiD3DX12_BLEND_DESC(arg_BlendMode);
			RetDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

			if (!isDepth) {
				RetDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
			}

			RetDesc.SampleMask = UINT_MAX;
			RetDesc.PrimitiveTopologyType = (D3D12_PRIMITIVE_TOPOLOGY_TYPE)arg_topologyType;
			RetDesc.NumRenderTargets = 1;
			RetDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			RetDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
			RetDesc.SampleDesc.Count = 1;

			return CreateDirect(RetDesc,arg_wkp_graphicDevice);
		}

		template<typename Vertex, typename VS>
		static inline Microsoft::WRL::ComPtr<ID3D12PipelineState> CreateShadowmap3D(const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature, D3D12_GRAPHICS_PIPELINE_STATE_DESC& RetDesc, std::shared_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) {

			CD3DX12_RASTERIZER_DESC rasterizerStateDesc(D3D12_DEFAULT);
			//表面カリング
			rasterizerStateDesc.CullMode = D3D12_CULL_MODE_FRONT;
			rasterizerStateDesc.FillMode = D3D12_FILL_MODE_SOLID;
			rasterizerStateDesc.DepthClipEnable = TRUE;

			ZeroMemory(&RetDesc, sizeof(RetDesc));
			RetDesc.InputLayout = { Vertex::GetVertexElement(), Vertex::GetNumElements() };
			RetDesc.pRootSignature = rootSignature.Get();
			RetDesc.VS =
			{
				reinterpret_cast<UINT8*>(VS::GetPtr()->GetShaderMicrosoft::WRL::ComPtr()->GetBufferPointer()),
				VS::GetPtr()->GetShaderMicrosoft::WRL::ComPtr()->GetBufferSize()
			};

			RetDesc.PS = CD3DX12_SHADER_BYTECODE(0, 0);
			RetDesc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
			RetDesc.NumRenderTargets = 0;

			RetDesc.RasterizerState = rasterizerStateDesc;
			RetDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			RetDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
			RetDesc.SampleMask = UINT_MAX;
			RetDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			RetDesc.NumRenderTargets = 0;
			RetDesc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
			RetDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
			RetDesc.SampleDesc.Count = 1;
			return CreateDirect(RetDesc,arg_wkp_graphicDevice);
		}

	}


	namespace CommandListHelper {
		static inline  Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CreateSimple( std::shared_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) {
			//デバイスの取得
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> Ret;
			(arg_wkp_graphicDevice->GetDevice().CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				&arg_wkp_graphicDevice->GetCommandAllocator(arg_wkp_graphicDevice->GetFrameIndex()),
				nullptr,
				IID_PPV_ARGS(&Ret)));
			return Ret;
		}
		static inline  Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CreateBundleCommandList(std::shared_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) {
			//デバイスの取得
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> Ret;
			auto hr= (arg_wkp_graphicDevice->GetDevice().CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_BUNDLE,
				&arg_wkp_graphicDevice->GetBundleCommandAllocator(),
				nullptr,
				IID_PPV_ARGS(&Ret)));
			return Ret;
		}

		static inline  Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CreateDefault(const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState, std::shared_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) {
			//デバイスの取得
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> Ret;
			auto hr = (arg_wkp_graphicDevice->GetDevice().CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				&arg_wkp_graphicDevice->GetCommandAllocator(arg_wkp_graphicDevice->GetFrameIndex()),
				pipelineState.Get(),
				IID_PPV_ARGS(&Ret)));
			return Ret;
		}
		static inline  Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CreateCopy(const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState, std::shared_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) {
			//デバイスの取得
			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> Ret;
			(arg_wkp_graphicDevice->GetDevice().CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_COPY,
				&arg_wkp_graphicDevice->GetCommandAllocator(arg_wkp_graphicDevice->GetFrameIndex()),
				pipelineState.Get(),
				IID_PPV_ARGS(&Ret)));
			return Ret;
		}

		static inline  void Reset(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList, std::shared_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) {
			//デバイスの取得
			commandList->Reset(&arg_wkp_graphicDevice->GetCommandAllocator(arg_wkp_graphicDevice->GetFrameIndex()), nullptr);

		}
		static inline  void Reset(const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState, const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,std::shared_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) {
			//デバイスの取得
			auto hr = commandList->Reset(&arg_wkp_graphicDevice->GetCommandAllocator(arg_wkp_graphicDevice->GetFrameIndex()), pipelineState.Get());
			if (hr != S_OK) {
				throw ButiException(L"", L"", L"");
			}
		}
		static inline  void BundleReset(const Microsoft::WRL::ComPtr<ID3D12PipelineState>& pipelineState, const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList,std::shared_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) {
			//デバイスの取得
			auto hr = commandList->Reset(&arg_wkp_graphicDevice->GetBundleCommandAllocator(), pipelineState.Get());
			if (hr != S_OK) {
				throw ButiException(L"", L"", L"");
			}
		}
		static inline  void Close(const Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>& commandList) {
			auto hr = commandList->Close();
			if (hr != S_OK) {
				throw ButiException(L"", L"", L"");
			}
		}

	}

	namespace DescriptorHeapHelper {
		static inline Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDirect(const D3D12_DESCRIPTOR_HEAP_DESC& desc, ID3D12Device& device) {

			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> Ret;
			auto hr= device.CreateDescriptorHeap(&desc, IID_PPV_ARGS(Ret.GetAddressOf()));

			return Ret;
		}
		static inline Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateCbvSrvUavHeap(const UINT arg_numDescriptorHeap, ID3D12Device& device) {
			//CbvSrvデスクプリタヒープ
			D3D12_DESCRIPTOR_HEAP_DESC CbvSrvHeapDesc = {};
			CbvSrvHeapDesc.NumDescriptors = arg_numDescriptorHeap;
			CbvSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			CbvSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			return CreateDirect(CbvSrvHeapDesc, device);
		}
		static inline Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateSamplerHeap(const UINT arg_numDescriptorHeap, ID3D12Device& device) {
			//サンプラーデスクプリタヒープ
			D3D12_DESCRIPTOR_HEAP_DESC SamplerHeapDesc = {};
			SamplerHeapDesc.NumDescriptors = arg_numDescriptorHeap;
			SamplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
			SamplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			return CreateDirect(SamplerHeapDesc, device);
		}

	}
	namespace SamplerHelper {
		static inline void CreateSampler(const SamplerState State, D3D12_CPU_DESCRIPTOR_HANDLE& Handle, ID3D12Device& device) {
			D3D12_SAMPLER_DESC samplerDesc = {};

			samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
			samplerDesc.MipLODBias = 0.0f;
			samplerDesc.MaxAnisotropy = 1;
			samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;

			switch (State) {
			case SamplerState::PointWrap:
				samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				break;
			case SamplerState::PointClamp:
				samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				break;
			case SamplerState::LinearWrap:
				samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				break;
			case SamplerState::LinearClamp:
				samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				break;
			case SamplerState::AnisotropicWrap:
				samplerDesc.Filter = D3D12_FILTER_ANISOTROPIC;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				break;
			case SamplerState::AnisotropicClamp:
				samplerDesc.Filter = D3D12_FILTER_ANISOTROPIC;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				break;
			case SamplerState::ComparisonLinear:
				samplerDesc.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
				samplerDesc.BorderColor[0] = 1.0f;
				samplerDesc.BorderColor[1] = 1.0f;
				samplerDesc.BorderColor[2] = 1.0f;
				samplerDesc.BorderColor[3] = 1.0f;
				samplerDesc.MinLOD = 0;
				samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
				samplerDesc.MipLODBias = 0.0f;
				samplerDesc.MaxAnisotropy = 0;
				samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

				break;
			}



			device.CreateSampler(&samplerDesc, Handle);
		}
	}
	//--------------------------------------------------------------------------------------
	/// ルートシグネチャのHelper
	//--------------------------------------------------------------------------------------
	namespace RootSignatureHelper {
		static inline Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateDirect(const D3D12_ROOT_SIGNATURE_DESC& desc, std::shared_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) {
			
			Microsoft::WRL::ComPtr<ID3DBlob> signature;
			Microsoft::WRL::ComPtr<ID3DBlob> error;
			Microsoft::WRL::ComPtr<ID3D12RootSignature> Ret;
			D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);

			arg_wkp_graphicDevice->GetDevice().CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&Ret));
			return Ret;
		}
		//一番シンプルなルートシグネチャ
		static inline Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateSimple(std::shared_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) {

			Microsoft::WRL::ComPtr<ID3D12RootSignature> Ret = arg_wkp_graphicDevice->GetRootSignature(L"Simple").first;
			if (Ret != nullptr) {
				return Ret;
			}

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			Ret = CreateDirect(rootSignatureDesc,arg_wkp_graphicDevice);
			arg_wkp_graphicDevice->SetRootSignature(L"Simple", Ret, rootSignatureDesc);
			return Ret;
		}
		//コンスタントバッファのみ
		static inline Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateCbv(std::shared_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) {
			Microsoft::WRL::ComPtr<ID3D12RootSignature> Ret = arg_wkp_graphicDevice->GetRootSignature(L"Cbv").first;
			if (Ret != nullptr) {
				return Ret;
			}

			CD3DX12_DESCRIPTOR_RANGE ranges[1];
			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
			CD3DX12_ROOT_PARAMETER rootParameters[1];
			rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_ALL);

			D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
				D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, rootSignatureFlags);

			Ret = CreateDirect(rootSignatureDesc, arg_wkp_graphicDevice);
			arg_wkp_graphicDevice->SetRootSignature(L"Cbv", Ret, rootSignatureDesc);
			return Ret;
		}
		//シェーダリソースとサンプラー
		static inline Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateSrvSmp( std::shared_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) {
			Microsoft::WRL::ComPtr<ID3D12RootSignature> Ret = arg_wkp_graphicDevice->GetRootSignature(L"SrvSmp").first;
			if (Ret != nullptr) {
				return Ret;
			}

			CD3DX12_DESCRIPTOR_RANGE ranges[2];
			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
			ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);

			CD3DX12_ROOT_PARAMETER rootParameters[2];
			rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
			Ret = CreateDirect(rootSignatureDesc, arg_wkp_graphicDevice);
			arg_wkp_graphicDevice->SetRootSignature(L"SrvSmp", Ret, rootSignatureDesc);
			return Ret;
		}

		//シェーダリソースとサンプラーとコンスタントバッファ
		static inline Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateSrvSmpCbv( std::shared_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) {
			
			auto Ret = arg_wkp_graphicDevice->GetRootSignature(L"SrvSmpCbv");
			if (Ret.first != nullptr) {
				return Ret.first;
			}


			CD3DX12_DESCRIPTOR_RANGE ranges[3];
			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
			ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);
			ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

			CD3DX12_ROOT_PARAMETER rootParameters[3];
			rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[2].InitAsDescriptorTable(1, &ranges[2], D3D12_SHADER_VISIBILITY_ALL);

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			auto out = CreateDirect(rootSignatureDesc, arg_wkp_graphicDevice);
			arg_wkp_graphicDevice->SetRootSignature(L"SrvSmpCbv", out, rootSignatureDesc);
			return out;
		}

		static inline Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateSrvSmpCbvMat(UINT materialCount, UINT srvCount, D3D12_ROOT_SIGNATURE_DESC& arg_rootSignatureDesc, std::shared_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) {
			auto Ret = arg_wkp_graphicDevice->GetRootSignature(L"SrvSmpCbvMat" + std::to_wstring(materialCount)+L"srv:" + std::to_wstring(srvCount));
			if (Ret.first != nullptr) {
				arg_rootSignatureDesc = Ret.second;
				return Ret.first;
			}


			std::vector< CD3DX12_DESCRIPTOR_RANGE> ranges;
			for (int i = 0; i < srvCount; i++) {
				CD3DX12_DESCRIPTOR_RANGE in;
				ranges.push_back(in);
				ranges[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, i);
			}
			CD3DX12_DESCRIPTOR_RANGE sampler;
			sampler.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);
			ranges.push_back(sampler);
			CD3DX12_DESCRIPTOR_RANGE cbv;
			cbv.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
			ranges.push_back(cbv);

			for (UINT i = 0; i < materialCount; i++) {

				CD3DX12_DESCRIPTOR_RANGE in;
				in.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1 + i);
				ranges.push_back(in);
			}

			std::vector<CD3DX12_ROOT_PARAMETER> rootParameters;
			for (int i = 0; i < srvCount; i++) {
				CD3DX12_ROOT_PARAMETER in;
				in.InitAsDescriptorTable(1, &ranges[i], D3D12_SHADER_VISIBILITY_PIXEL);
				rootParameters.push_back(in);
			}

			CD3DX12_ROOT_PARAMETER samplerParam;
			samplerParam.InitAsDescriptorTable(1, &ranges[srvCount], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters.push_back(samplerParam);

			CD3DX12_ROOT_PARAMETER cbvParam;
			cbvParam.InitAsDescriptorTable(1, &ranges[srvCount+1], D3D12_SHADER_VISIBILITY_ALL);
			rootParameters.push_back(cbvParam);

			for (UINT i = 0; i < materialCount; i++) {
				CD3DX12_ROOT_PARAMETER in;
				in.InitAsDescriptorTable(1, &ranges[srvCount+2 + i], D3D12_SHADER_VISIBILITY_ALL);
				rootParameters.push_back(in);
			}

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(rootParameters.size(), rootParameters.data(), 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			arg_rootSignatureDesc = rootSignatureDesc;

			auto out = CreateDirect(arg_rootSignatureDesc, arg_wkp_graphicDevice);
			arg_wkp_graphicDevice->SetRootSignature(L"SrvSmpCbvMat" + std::to_wstring(materialCount) + L"srv:" + std::to_wstring(srvCount), out, arg_rootSignatureDesc);
			return out;
		}
	}

}