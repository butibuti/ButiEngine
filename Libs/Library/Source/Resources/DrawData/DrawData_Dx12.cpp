#include "stdafx.h"

#include"Header/GameParts/ResourceContainer.h"
#include"Header/Device/GraphicResourceUtil_Dx12.h"
#include"Header/Device/PipelineStateManager.h"
#include "..\..\..\Header\Resources\DrawData\DrawData_Dx12.h"

ButiEngine::MeshDrawData_Dx12::MeshDrawData_Dx12(const MeshTag& arg_meshTag, const ShaderTag& arg_shader, const MaterialTag& arg_materialTag,  std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice, std::shared_ptr< DrawInformation >arg_shp_drawInfo, std::shared_ptr<Transform> arg_shp_transform)
	:MeshDrawData_Dx12(arg_meshTag, arg_shader, std::vector<MaterialTag>{arg_materialTag}, arg_shp_renderer,arg_wkp_graphicDevice,arg_shp_drawInfo, arg_shp_transform)
{
}

ButiEngine::MeshDrawData_Dx12::MeshDrawData_Dx12(const MeshTag & arg_meshTag,  const ShaderTag & arg_shader, const std::vector<MaterialTag>& arg_materialTag,  std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice, std::shared_ptr< DrawInformation >arg_shp_drawInfo, std::shared_ptr<Transform> arg_shp_transform)
{
	shp_renderer = arg_shp_renderer;
	meshTag = arg_meshTag;
	shaderTag = arg_shader;
	vec_materialTags = arg_materialTag;
	wkp_graphicDevice = arg_wkp_graphicDevice;
	subset.push_back(wkp_graphicDevice.lock()->GetApplication().lock()-> GetResourceContainer()->GetMesh(meshTag).lock()->GetIndexCount());
	boxEightCorner = wkp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer()->GetMesh(meshTag).lock()->GetBackUpdata().GetBoxEightCorner();
	shp_drawInfo = arg_shp_drawInfo;
	transform = arg_shp_transform;
}

ButiEngine::MeshDrawData_Dx12::MeshDrawData_Dx12(const ModelTag & arg_model, const ShaderTag & arg_shader,  std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice, std::shared_ptr< DrawInformation >arg_shp_drawInfo, std::shared_ptr<Transform> arg_shp_transform)
	:MeshDrawData_Dx12(arg_wkp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer()->GetModel(arg_model).lock()->GetMeshTag(),arg_shader,
		arg_wkp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer()->GetModel(arg_model).lock()->GetMaterialTags(), arg_shp_renderer,arg_wkp_graphicDevice, arg_shp_drawInfo,arg_shp_transform)
{
	shp_drawInfo = arg_shp_drawInfo;
	modelTag =arg_model;
	subset = wkp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer()->GetModel(arg_model).lock()->GetSubset();
	
}


void ButiEngine::MeshDrawData_Dx12::ChangeCullMode(const CullMode& arg_cull)
{
	rasterizerStateDesc.CullMode = (D3D12_CULL_MODE)arg_cull;


	//pipelineState = PipelineStateHelper::CreateDefault3D(rootSignature, pipeLineDesc,rasterizerStateDesc, wkp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer()->GetShader(shaderTag).lock(),shp_drawInfo->drawSettings. blendMode, wkp_graphicDevice.lock());


	CommandListHelper::BundleReset(pipelineState, commandList, wkp_graphicDevice.lock());
	CommandSet();
}

void ButiEngine::MeshDrawData_Dx12::ChangeFillMode(const bool isFill)
{
	if(isFill)
	rasterizerStateDesc .FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
	else
	rasterizerStateDesc .FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME;


	//pipelineState = PipelineStateHelper::CreateDefault3D(rootSignature, pipeLineDesc,rasterizerStateDesc, wkp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer()->GetShader(shaderTag).lock(), shp_drawInfo->drawSettings.blendMode, wkp_graphicDevice.lock());


	CommandListHelper::BundleReset(pipelineState, commandList, wkp_graphicDevice.lock());
	CommandSet();
}

void ButiEngine::MeshDrawData_Dx12::ChangeSwitchFillMode()
{
	if (rasterizerStateDesc.FillMode == D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID)
		rasterizerStateDesc.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME;
	else
		rasterizerStateDesc.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;


	//pipelineState = PipelineStateHelper::CreateDefault3D(rootSignature, pipeLineDesc, rasterizerStateDesc, wkp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer()->GetShader(shaderTag).lock(), shp_drawInfo->drawSettings.blendMode, wkp_graphicDevice.lock());

	CommandListHelper::BundleReset(pipelineState, commandList,wkp_graphicDevice.lock());
	CommandSet();
}

void ButiEngine::MeshDrawData_Dx12::Initialize()
{

	{
		auto lightBuffer_Dx12 = ObjectFactory::Create<CBuffer_Dx12<LightVariable>>(2);

		shp_drawInfo->vec_exCBuffer.push_back(lightBuffer_Dx12);

		auto light = LightVariable();
		light.LightDir = Vector4(Vector3(-1.0f, -1.0f, 0.0f), 1);

		lightBuffer_Dx12->Get() = light;
	}
	DrawData_Dx12::Initialize();
	
	CommandListHelper::BundleReset(pipelineState, commandList,wkp_graphicDevice.lock());
	CommandSet();


}


void ButiEngine::MeshDrawData_Dx12::Draw() {
	BufferUpdate();
	CommandExecute();
}




void ButiEngine::DrawData_Dx12::Initialize()
{

	CreatePipeLineState(shp_drawInfo->vec_exCBuffer.size()+1);

	//デスクプリタヒープのハンドルの配列の作成

	for (auto itr = shp_drawInfo->vec_exCBuffer.begin(); itr != shp_drawInfo->vec_exCBuffer.end(); itr++) {
		(*itr)->SetGraphicDevice(wkp_graphicDevice.lock());
		(*itr)->CreateBuffer();
	}

	auto samplerHandleInfo = wkp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->GetSamplerHandle(0);
	samplerBufferDescriptorHandle= (samplerHandleInfo.GPUHandle);

	auto cbuffer_Dx12 = ObjectFactory::Create<CBuffer_Dx12<ShaderVariable>>(0);

	cbuffer_Dx12->SetGraphicDevice(wkp_graphicDevice.lock());

	cbuffer_Dx12->CreateBuffer();

	Matrix4x4 mat;
	cbuffer_Dx12->Get().World = mat;

	cbuffer = cbuffer_Dx12;

	commandList = CommandListHelper::CreateBundleCommandList( wkp_graphicDevice.lock());
	CommandListHelper::Close(commandList);

	switch (shp_drawInfo->drawSettings.billboardMode)
	{
	case BillBoardMode::full:
		shp_worldMatrixUpdater = std::make_shared<MatrixUpdater_billBoard>(cbuffer, transform,wkp_graphicDevice);
		break;
	case BillBoardMode::x:
		shp_worldMatrixUpdater = std::make_shared<MatrixUpdater_billBoardX>(cbuffer, transform,wkp_graphicDevice);
		break;
	case BillBoardMode::y:
		shp_worldMatrixUpdater = std::make_shared<MatrixUpdater_billBoardY>(cbuffer, transform, wkp_graphicDevice);
		break;
	case BillBoardMode::z:
		shp_worldMatrixUpdater = std::make_shared<MatrixUpdater_billBoardZ>(cbuffer, transform, wkp_graphicDevice);
		break;
	case BillBoardMode::none:
		shp_worldMatrixUpdater = std::make_shared<MatrixUpdater_default>(cbuffer, transform, wkp_graphicDevice);
		break;
	default:
		shp_worldMatrixUpdater = std::make_shared<MatrixUpdater_default>(cbuffer, transform,wkp_graphicDevice);
		break;
	}
}

void ButiEngine::DrawData_Dx12::CreatePipeLineState(const UINT arg_exCBuffer)
{
	if (arg_exCBuffer <= cBufferCount)
		return;

	cBufferCount = arg_exCBuffer;
	ZeroMemory(&pipeLineDesc, sizeof(pipeLineDesc));
	rootSignature = RootSignatureHelper::CreateSrvSmpCbvMat(cBufferCount, rootdesc, wkp_graphicDevice.lock());

	rasterizerStateDesc = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

	rasterizerStateDesc.CullMode = (D3D12_CULL_MODE)shp_drawInfo->drawSettings.cullMode;
	rasterizerStateDesc.FillMode =shp_drawInfo->drawSettings.isFill;

	auto pipeLineTopology = shp_drawInfo->drawSettings.topologyType;
	if (pipeLineTopology == TopologyType::triangleList) {
		pipeLineTopology = TopologyType::triangle;
	}else if (pipeLineTopology == TopologyType::pointList) {
		pipeLineTopology = TopologyType::point;
	}
	pipelineState = wkp_graphicDevice.lock()->GetPipelineStateManager().GetPipelineState(rootSignature, rootdesc, pipeLineDesc, rasterizerStateDesc, wkp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer()->GetShader(shaderTag).lock(), shp_drawInfo->drawSettings.blendMode,pipeLineTopology );

}

void ButiEngine::DrawData_Dx12::CommandExecute()
{
	wkp_graphicDevice.lock()->GetCommandList().ExecuteBundle(commandList.Get());

}

void ButiEngine::DrawData_Dx12::BufferUpdate()
{

	
	shp_worldMatrixUpdater->WorldMatrixUpdate();
	cbuffer->Get().View = wkp_graphicDevice.lock()->GetCameraViewMatrix();
	cbuffer->Get().Projection = wkp_graphicDevice.lock()->GetProjectionMatrix();
	

	cbuffer->Update();


	for (auto itr =shp_drawInfo-> vec_exCBuffer.begin(); itr !=shp_drawInfo-> vec_exCBuffer.end(); itr++) {
		(*itr)->Update();
	}
}

void ButiEngine::DrawData_Dx12::CommandSet()
{

	wkp_graphicDevice.lock()->SetCommandList(commandList.Get());
	//ルートシグネチャのセット
	commandList-> SetGraphicsRootSignature(rootSignature.Get());
	//デスクプリタヒープのセット
	ID3D12DescriptorHeap* ppHeaps[] = { wkp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->GetDescriptorHeap().Get() ,wkp_graphicDevice.lock()->GetDescriptorHeapManager().lock()->GetSamplerHeap().Get() };
	auto heapCount = _countof(ppHeaps);
	commandList ->SetDescriptorHeaps(heapCount, ppHeaps);
	//GPUデスクプリタヒープハンドルのセット
	commandList->SetGraphicsRootDescriptorTable(TextureTableRegion, samplerBufferDescriptorHandle);
	cbuffer->Attach();

	for (auto itr =shp_drawInfo-> vec_exCBuffer.begin(); itr !=shp_drawInfo-> vec_exCBuffer.end(); itr++) {
		(*itr)->Attach();
	}

	shp_renderer->Draw(meshTag);

	wkp_graphicDevice.lock()->GetCommandList().IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)shp_drawInfo->drawSettings.topologyType);
	auto spliteCount = subset.size();

	UINT offset = 0;
	for (int i = 0; i < subset.size(); i++) {

		shp_renderer->MaterialAttach( vec_materialTags.at(i));
		UINT count = subset.at(i);
		commandList->DrawIndexedInstanced(count, 1, offset, 0, 0);

		offset += count;
	}
	wkp_graphicDevice.lock()->UnSetCommandList();

	CommandListHelper::Close(commandList);
}

std::shared_ptr<ButiEngine::ICBuffer> ButiEngine::DrawData_Dx12::AddICBuffer(std::shared_ptr<ICBuffer> arg_cbuffer)
{
	CreatePipeLineState(arg_cbuffer->GetSlot());

	auto output= DrawData::AddICBuffer(arg_cbuffer);

	CommandListHelper::BundleReset(pipelineState, commandList,wkp_graphicDevice.lock());
	CommandSet();
	return output;
}

void ButiEngine::MatrixUpdater_default::WorldMatrixUpdate()
{
	cbuffer->Get().World = transform->ToMatrix();
}

void ButiEngine::MatrixUpdater_billBoard::WorldMatrixUpdate()
{
	cbuffer->Get().World = (XMMATRIX)transform->ToMatrix() * wkp_graphicDevice.lock()->GetViewMatrixBillBoard();
}

void ButiEngine::MatrixUpdater_billBoardX::WorldMatrixUpdate()
{
	

	cbuffer->Get().World = (XMMATRIX)transform->ToMatrix() * wkp_graphicDevice.lock()->GetViewMatrixBillBoardX();
}

void ButiEngine::MatrixUpdater_billBoardY::WorldMatrixUpdate()
{


	cbuffer->Get().World = (XMMATRIX)transform->ToMatrix() * wkp_graphicDevice.lock()->GetViewMatrixBillBoardY();
}

void ButiEngine::MatrixUpdater_billBoardZ::WorldMatrixUpdate()
{
	cbuffer->Get().World = (XMMATRIX)transform->ToMatrix() * wkp_graphicDevice.lock()->GetViewMatrixBillBoardZ();
}
