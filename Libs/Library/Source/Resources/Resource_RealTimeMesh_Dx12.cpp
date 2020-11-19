#include "stdafx.h"
#include "..\..\Header\Resources\Resource_RealTimeMesh_Dx12.h"

ButiEngine::Resource_RealTimeMesh_Dx12::Resource_RealTimeMesh_Dx12(std::shared_ptr<GraphicDevice> arg_graphicDevice)
{
	wkp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}



void ButiEngine::Resource_RealTimeMesh_Dx12::Draw()
{

	wkp_graphicDevice.lock()->GetCommandList().IASetVertexBuffers(0, 1, &vertexBufferView);
	wkp_graphicDevice.lock()->GetCommandList().IASetIndexBuffer(&indexBufferView);


}

void ButiEngine::Resource_RealTimeMesh_Dx12::Initialize()
{
	wkp_graphicDevice.lock()->AddResource(&(*GetThis<Resource_RealTimeMesh_Dx12>()));
}

void ButiEngine::Resource_RealTimeMesh_Dx12::SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice)
{
	wkp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}

void ButiEngine::Resource_RealTimeMesh_Dx12::SetVertexBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> arg_vertexBuffer)
{
	vertexBuffer = arg_vertexBuffer;
}

void ButiEngine::Resource_RealTimeMesh_Dx12::SetIndexBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> arg_indexBuffer)
{
	indexBuffer = arg_indexBuffer;
}

void ButiEngine::Resource_RealTimeMesh_Dx12::SetVertexBufferUploadHeap(Microsoft::WRL::ComPtr<ID3D12Resource> arg_vertexBufferUploadHeap)
{
	vertexBufferUploadHeap = arg_vertexBufferUploadHeap;
}

void ButiEngine::Resource_RealTimeMesh_Dx12::SetIndexBufferUploadHeap(Microsoft::WRL::ComPtr<ID3D12Resource> arg_indexBufferUploadHeap)
{
	indexBufferUploadHeap = arg_indexBufferUploadHeap;
}

void ButiEngine::Resource_RealTimeMesh_Dx12::SetVertexBufferView(D3D12_VERTEX_BUFFER_VIEW arg_vertexBufferview)
{
	vertexBufferView = arg_vertexBufferview;
}

void ButiEngine::Resource_RealTimeMesh_Dx12::SetIndexBufferView(D3D12_INDEX_BUFFER_VIEW arg_indexBufferView)
{
	indexBufferView = arg_indexBufferView;
}

void ButiEngine::Resource_RealTimeMesh_Dx12::SetVertexCount(const UINT arg_vertexCount)
{
	verticesCount = arg_vertexCount;
}

void ButiEngine::Resource_RealTimeMesh_Dx12::SetIndexCount(const UINT arg_indexCount)
{
	indexCount = arg_indexCount;
}

Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::Resource_RealTimeMesh_Dx12::GetVertexBuffer()
{
	return vertexBuffer;
}

Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::Resource_RealTimeMesh_Dx12::GetIndexBuffer()
{
	return indexBuffer;
}

Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::Resource_RealTimeMesh_Dx12::GetVertexBufferUploadHeap()
{
	return vertexBufferUploadHeap;
}

Microsoft::WRL::ComPtr<ID3D12Resource> ButiEngine::Resource_RealTimeMesh_Dx12::GetIndexBufferUploadHeap()
{
	return indexBufferUploadHeap;
}

void ButiEngine::Resource_RealTimeMesh_Dx12::Update()
{
	if (isDataRefresh) {
		return;
	}
	wkp_graphicDevice.lock()->AddResource(GetThis<Resource>().get());

	isDataRefresh = true;
}

void ButiEngine::Resource_RealTimeMesh_Dx12::ResourceUpdate()
{
	if (!backupData) {
		return;
	}
	backupData->ResourceUpdate(GetThis<Resource_RealTimeMesh_Dx12>(), wkp_graphicDevice);


	isDataRefresh = false;
}

void ButiEngine::Resource_RealTimeMesh_Dx12::UpdateResourceRelease()
{
}
