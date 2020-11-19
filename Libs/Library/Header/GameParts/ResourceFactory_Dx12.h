#pragma once
#include"stdafx.h"
#include"Header/Resources/Resource_Mesh_Dx12.h"
#include"Header/Resources/Resource_RealTimeMesh_Dx12.h"
#include"ResourceFactory.h"
namespace ButiEngine {
	class ResourceFactory_Dx12 :public ResourceFactory {
	public:
		ResourceFactory_Dx12(std::weak_ptr<GraphicDevice> arg_wkp_graphicDevice);
		std::shared_ptr<Resource_Texture> CreateTextureFromFile(const std::string& filePath )override;


		std::shared_ptr<Resource_PixelShader> CreatePixelShaderFromFile(const std::string& filePath )override;
		std::shared_ptr<Resource_VertexShader> CreateVertexShaderFromFile(const std::string& filePath )override;
		std::shared_ptr<Resource_GeometryShader> CreateGeometryShaderFromFile(const std::string& filePath )override;
		std::shared_ptr<Resource_Material> CreateMaterial(const MaterialVariable& arg_materialVar, const TextureTag& arg_textureTag)override;
		std::shared_ptr<Resource_Material> CreateMaterial(const MaterialVariable& arg_materialVar, const std::vector< TextureTag>& arg_textureTag)override;
		template<typename T>
		std::shared_ptr<Resource_Mesh> CreateMesh(const BackupData<T>& inputMeshData) {
			auto shp_graphicDevice_Dx12 = wkp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>();
			auto output = ObjectFactory::Create<Resource_Mesh_Dx12>(shp_graphicDevice_Dx12);

			auto DataPtr = std::make_shared< BackupData<T> >();
			DataPtr->vertices.clear();
			for (auto v : inputMeshData.vertices) {
				DataPtr->vertices.push_back(v);
			}
			DataPtr->indices.clear();
			for (auto i : inputMeshData.indices) {
				DataPtr->indices.push_back(i);
			}
			DataPtr->eightCorner = inputMeshData.eightCorner;
			output->SetBackupData(DataPtr);
			UINT vertexCount;
			UINT indexCount;
			Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
			Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
			Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferUploadHeap;
			Microsoft::WRL::ComPtr<ID3D12Resource> indexBufferUploadHeap;
			D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
			D3D12_INDEX_BUFFER_VIEW indexBufferView;


			UINT vertexBufferSize = (UINT)(sizeof(T) * inputMeshData.vertices.size());
			//頂点バッファの作成
			{
				shp_graphicDevice_Dx12->GetDevice().CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&vertexBuffer));

				shp_graphicDevice_Dx12->GetDevice().CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&vertexBufferUploadHeap));
				// Initialize the vertex buffer view.
				vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
				vertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(T));
				vertexBufferView.SizeInBytes = static_cast<UINT>(vertexBufferSize);
			}
			//頂点数の設定
			vertexCount = static_cast<UINT>(inputMeshData.vertices.size());
			//インデックスバッファの作成
			UINT indexBufferSize = static_cast<UINT>(sizeof(UINT) * inputMeshData.indices.size());
			{
				shp_graphicDevice_Dx12->GetDevice().CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&indexBuffer));

				shp_graphicDevice_Dx12->GetDevice().CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&indexBufferUploadHeap));

				// Describe the index buffer view.
				indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
				indexBufferView.Format = DXGI_FORMAT_R32_UINT;
				indexBufferView.SizeInBytes = indexBufferSize;

			}
			//インデックス数の設定
			indexCount = static_cast<UINT>(inputMeshData.indices.size());

			output->SetIndexBuffer(indexBuffer);
			output->SetIndexBufferUploadHeap(indexBufferUploadHeap);
			output->SetIndexBufferView(indexBufferView);
			output->SetIndexCount(indexCount);
			output->SetVertexBuffer(vertexBuffer);
			output->SetVertexBufferUploadHeap(vertexBufferUploadHeap);
			output->SetVertexBufferView(vertexBufferView);
			output->SetVertexCount(vertexCount);


			return output;
		}


		template<typename T>
		std::shared_ptr<Resource_RealTimeMesh> CreateRealTimeMesh(const BackupData<T>& inputMeshData) {
			auto shp_graphicDevice_Dx12 = wkp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>();
			auto output = ObjectFactory::Create<Resource_RealTimeMesh_Dx12>(shp_graphicDevice_Dx12);

			auto DataPtr = std::make_shared< BackupData<T> >();
			DataPtr->vertices.clear();
			for (auto v : inputMeshData.vertices) {
				DataPtr->vertices.push_back(v);
			}
			DataPtr->indices.clear();
			for (auto i : inputMeshData.indices) {
				DataPtr->indices.push_back(i);
			}
			DataPtr->eightCorner = inputMeshData.eightCorner;
			output->SetBackupData(DataPtr);
			UINT vertexCount;
			UINT indexCount;
			Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
			Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
			Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferUploadHeap;
			Microsoft::WRL::ComPtr<ID3D12Resource> indexBufferUploadHeap;
			D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
			D3D12_INDEX_BUFFER_VIEW indexBufferView;


			UINT vertexBufferSize = (UINT)(sizeof(T) * inputMeshData.vertices.size());
			//頂点バッファの作成
			{
				shp_graphicDevice_Dx12->GetDevice().CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&vertexBuffer));

				shp_graphicDevice_Dx12->GetDevice().CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&vertexBufferUploadHeap));
				// Initialize the vertex buffer view.
				vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
				vertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(T));
				vertexBufferView.SizeInBytes = static_cast<UINT>(vertexBufferSize);
			}
			//頂点数の設定
			vertexCount = static_cast<UINT>(inputMeshData.vertices.size());
			//インデックスバッファの作成
			UINT indexBufferSize = static_cast<UINT>(sizeof(UINT) * inputMeshData.indices.size());
			{
				shp_graphicDevice_Dx12->GetDevice().CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&indexBuffer));

				shp_graphicDevice_Dx12->GetDevice().CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&indexBufferUploadHeap));

				// Describe the index buffer view.
				indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
				indexBufferView.Format = DXGI_FORMAT_R32_UINT;
				indexBufferView.SizeInBytes = indexBufferSize;

			}
			//インデックス数の設定
			indexCount = static_cast<UINT>(inputMeshData.indices.size());

			output->SetIndexBuffer(indexBuffer);
			output->SetIndexBufferUploadHeap(indexBufferUploadHeap);
			output->SetIndexBufferView(indexBufferView);
			output->SetIndexCount(indexCount);
			output->SetVertexBuffer(vertexBuffer);
			output->SetVertexBufferUploadHeap(vertexBufferUploadHeap);
			output->SetVertexBufferView(vertexBufferView);
			output->SetVertexCount(vertexCount);


			return output;
		}

	
	};



}

