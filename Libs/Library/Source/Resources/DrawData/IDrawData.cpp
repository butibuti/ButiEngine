#include "stdafx.h"
#include"Header/Common/CollisionPrimitive.h"
#include "..\..\..\Header\Resources\DrawData\IDrawData.h"

void ButiEngine::DrawData::SetBlendMode(const BlendMode& arg_BlendMode)
{
}

void ButiEngine::DrawData::MatrixUpdate()
{
    shp_worldMatrixUpdater->WorldMatrixUpdate();
}

std::shared_ptr<ButiEngine::Collision::CollisionPrimitive_Box_AABB> ButiEngine::MeshDrawData::GetMeshAABB()
{
    const BoxEightCorner& box = shp_renderer->GetScene().lock()->GetSceneManager().lock()->GetApplication().lock()->GetResourceContainer()->GetMesh(meshTag).lock()->GetBackUpdata().GetBoxEightCorner();
    Vector3 length = box.up_right_front - box.down_left_back;
    auto output = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(length, shp_transform);

    return output;
}

std::shared_ptr<ButiEngine::Collision::CollisionPrimitive_Box_OBB> ButiEngine::MeshDrawData::GetMeshOBB()
{
    const BoxEightCorner& box = shp_renderer->GetScene().lock()->GetSceneManager().lock()->GetApplication().lock()->GetResourceContainer()->GetMesh(meshTag).lock()->GetBackUpdata().GetBoxEightCorner();
    Vector3 length = box.up_right_front - box.down_left_back;
    auto output = ObjectFactory::Create<Collision::CollisionPrimitive_Box_OBB>(length, shp_transform);

    return output;
}

void ButiEngine::MeshDrawData::SetPrimitive(std::shared_ptr<Collision::CollisionPrimitive> arg_prim)
{
    shp_primitive = arg_prim;
}

void ButiEngine::MeshDrawData::SetOctRegistPtr(unsigned int* arg_ptr)
{
    collisionRegistPtr = arg_ptr;
}

std::shared_ptr<ButiEngine::Collision::CollisionPrimitive> ButiEngine::MeshDrawData::GetPrimitive()
{
    return shp_primitive;
}

unsigned int* ButiEngine::MeshDrawData::GetOctRegistPtr()
{
    return collisionRegistPtr;
}
