#pragma once
#include"stdafx.h"
#include"Vertex.h"
namespace ButiEngine {
	class GraphicDevice;
	class Resource_RealTimeMesh : public Resource, public IResource_Mesh
	{
	public:
		virtual void SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice) = 0;
		void Initialize()override {}
		void PreInitialize()override {}
		void SetBackupData(std::shared_ptr<BackupDataBase> arg_backupData);
		virtual void Draw() = 0;
		UINT GetVertexCount();
		UINT GetIndexCount();
		virtual void Update()=0;
		const BackupDataBase& GetBackUpdata()const;
		std::shared_ptr<BackupDataBase> GetBackupData_Row();
	protected:
		UINT verticesCount;
		UINT indexCount;
		UINT stride;
		bool canAccessWrite;
		bool isDataRefresh = true;

		std::shared_ptr<BackupDataBase> backupData;


	};

}