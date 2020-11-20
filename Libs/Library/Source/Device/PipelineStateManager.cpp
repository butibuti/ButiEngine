#include"stdafx.h"
#include"Header/Device/PipelineStateManager.h"
#include"Header/Device/GraphicResourceUtil_Dx12.h"
#include"Header/Resources/Resource_Shader_Dx12.h"

ButiEngine::PipelineStateManager::PipelineStateManager(std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice)
{
	wkp_graphicDevice = arg_wkp_graphicDevice;
}
void ButiEngine::PipelineStateManager::Release()
{
	ClearPipelineState();
}

void ButiEngine::PipelineStateManager::ClearPipelineState()
{
	map_pipeLineState.clear();
}

Microsoft::WRL::ComPtr<ID3D12PipelineState> ButiEngine::PipelineStateManager::GetPipelineState(const std::string& arg_pipeLineState)
{
	return map_pipeLineState.at(arg_pipeLineState);
}

Microsoft::WRL::ComPtr<ID3D12PipelineState> ButiEngine::PipelineStateManager::GetPipelineState(const Microsoft::WRL::ComPtr<ID3D12RootSignature>& rootSignature, const D3D12_ROOT_SIGNATURE_DESC rootdesc, D3D12_GRAPHICS_PIPELINE_STATE_DESC& RetDesc, D3D12_RASTERIZER_DESC& arg_rasteriserDesc, std::shared_ptr<IResource_Shader> arg_shader, const BlendMode arg_BlendMode, const TopologyType arg_topologyType,const bool arg_isDepth)
{
	auto rasterStr = std::to_string(arg_rasteriserDesc.CullMode) + std::to_string(arg_rasteriserDesc.FillMode) + std::to_string(arg_rasteriserDesc.AntialiasedLineEnable) + std::to_string(arg_rasteriserDesc.ConservativeRaster);
	auto rootStr = std::to_string(rootdesc.Flags) + std::to_string(rootdesc.NumParameters) + std::to_string(rootdesc.NumStaticSamplers);
	auto blend = std::to_string((int)arg_BlendMode);
	std::string key = arg_shader->GetShaderName() + "/" + rasterStr + "/" + rootStr + "/" + blend;
	if (map_pipeLineState.count(key)) {
		return map_pipeLineState.at(key);
	}

	auto output = PipelineStateHelper::CreateDefault3D(rootSignature, RetDesc, arg_rasteriserDesc, arg_shader, arg_BlendMode,arg_topologyType, wkp_graphicDevice.lock(),arg_isDepth);
	map_pipeLineState.emplace(key, output);
	return output;
}