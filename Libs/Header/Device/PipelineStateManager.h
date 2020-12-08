#include"stdafx.h"

#include <D3dx12.h>
namespace ButiEngine
{
	class GraphicDevice_Dx12;
	class Resource_Shader;
	class PipelineStateManager :public IObject {
	public:
		PipelineStateManager(std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice);
		void Release();
		void ClearPipelineState();
		void Initialize() {}
		void PreInitialize() {}
		Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState(const std::string& arg_pipeLineState);
		Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState(const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature, const D3D12_ROOT_SIGNATURE_DESC rootdesc, D3D12_GRAPHICS_PIPELINE_STATE_DESC& RetDesc, D3D12_RASTERIZER_DESC& arg_rasteriserDesc, std::shared_ptr<IResource_Shader> arg_shader, const BlendMode arg_BlendMode, const TopologyType arg_topologyType,const bool arg_isDepth);
	private:
		std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D12PipelineState>> map_pipeLineState;
		std::weak_ptr<GraphicDevice_Dx12> wkp_graphicDevice;
	};
}