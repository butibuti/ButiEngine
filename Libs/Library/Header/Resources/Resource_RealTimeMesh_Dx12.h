#pragma once
#include"stdafx.h"
#include"Resource_RealTimeMesh.h"
#include"../GameParts/GraphicDevice_Dx12.h"
namespace ButiEngine {
	class GraphicDevice_Dx12;
	class Resource_RealTimeMesh_Dx12 :public Resource_RealTimeMesh
	{
	public:
		Resource_RealTimeMesh_Dx12(std::shared_ptr<GraphicDevice> arg_graphicDevice);
		void Draw()override;

		void Initialize()override;

		void SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice) override;

		void SetVertexBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> arg_vertexBuffer);
		void SetIndexBuffer(Microsoft::WRL::ComPtr<ID3D12Resource> arg_indeexBuffer);
		void SetVertexBufferUploadHeap(Microsoft::WRL::ComPtr<ID3D12Resource> arg_vertexBufferUploadHeap);
		void SetIndexBufferUploadHeap(Microsoft::WRL::ComPtr<ID3D12Resource> arg_indexBufferUploadHeap);

		void SetVertexBufferView(D3D12_VERTEX_BUFFER_VIEW arg_vertexBufferview);
		void SetIndexBufferView(D3D12_INDEX_BUFFER_VIEW arg_indexBufferView);

		void SetVertexCount(const UINT arg_vertexCount);

		void SetIndexCount(const UINT arg_indexCount);

		Microsoft::WRL::ComPtr<ID3D12Resource> GetVertexBuffer();

		Microsoft::WRL::ComPtr<ID3D12Resource> GetIndexBuffer();

		Microsoft::WRL::ComPtr<ID3D12Resource> GetVertexBufferUploadHeap();

		Microsoft::WRL::ComPtr<ID3D12Resource> GetIndexBufferUploadHeap();

		void Update()override;

	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferUploadHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBufferUploadHeap;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
		D3D12_INDEX_BUFFER_VIEW indexBufferView;


		std::weak_ptr<GraphicDevice_Dx12> wkp_graphicDevice;


		void ResourceUpdate() override;

		void UpdateResourceRelease()override;

	};


	template<typename T>
	inline void ButiEngine::BackupData<T>::ResourceUpdate(std::shared_ptr<Resource_RealTimeMesh_Dx12> arg_resource, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice)
	{
		//頂点バッファの更新
		D3D12_SUBRESOURCE_DATA vertexData = {};
		vertexData.pData = (void*)&vertices[0];
		vertexData.RowPitch = vertices.size() * sizeof(T);
		vertexData.SlicePitch = vertexData.RowPitch;
		UpdateSubresources<1>(&arg_wkp_graphicDevice.lock()->GetCommandList(), arg_resource->GetVertexBuffer().Get(), arg_resource->GetVertexBufferUploadHeap().Get(), 0, 0, 1, &vertexData);
		auto trans = CD3DX12_RESOURCE_BARRIER::Transition(arg_resource->GetVertexBuffer().Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		arg_wkp_graphicDevice.lock()->GetCommandList().ResourceBarrier(1,&trans );
		//インデックスバッファの更新
		if (indices.size() > 0) {
			D3D12_SUBRESOURCE_DATA indexData = {};
			indexData.pData = (void*)&indices[0];
			indexData.RowPitch = indices.size() * sizeof(UINT);
			indexData.SlicePitch = indexData.RowPitch;
			UpdateSubresources<1>(&arg_wkp_graphicDevice.lock()->GetCommandList(), arg_resource->GetIndexBuffer().Get(), arg_resource->GetIndexBufferUploadHeap().Get(), 0, 0, 1, &indexData);
			auto tr = CD3DX12_RESOURCE_BARRIER::Transition(arg_resource->GetIndexBuffer().Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
			arg_wkp_graphicDevice.lock()->GetCommandList().ResourceBarrier(1, &tr);
		}
	}
}