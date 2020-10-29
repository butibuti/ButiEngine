#include"stdafx.h"

#include"Header/Resources/Resource_Mesh.h"


void ButiEngine::Resource_Mesh::SetBackupData(std::shared_ptr<BackupDataBase> arg_backupData)
{
		backupData = arg_backupData;
}

UINT ButiEngine::Resource_Mesh::GetVertexCount()
{
	return verticesCount;
}

UINT ButiEngine::Resource_Mesh::GetIndexCount()
{
	return indexCount;
}

const ButiEngine::BackupDataBase& ButiEngine::Resource_Mesh::GetBackUpdata() const
{
	return *backupData;
}
