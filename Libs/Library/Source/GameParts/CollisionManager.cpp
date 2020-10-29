#include "stdafx.h"



ButiEngine::Collision::CollisionManager::CollisionManager(const int arg_initLayerCount)
{
    for(int i=0;i<arg_initLayerCount;i++)
    vec_collisionLayer.push_back(ObjectFactory::Create<CollisionLayer>(5,Vector3(-30,-30,-60),Vector3(30,30,30)));
}

void ButiEngine::Collision::CollisionManager::Update()
{
    for (auto itr = vec_collisionLayer.begin(); itr != vec_collisionLayer.end(); itr++) {
        (*itr)->Update();
    }
    int f = 0;
}

UINT* ButiEngine::Collision::CollisionManager::RegistCollisionObject(const int layerNum, std::shared_ptr< CollisionObject> arg_registObj)
{
    if (layerNum >= vec_collisionLayer.size()) {
        return nullptr;
    }
    return vec_collisionLayer.at(layerNum)->RegistCollisionObj(arg_registObj);
}

void ButiEngine::Collision::CollisionManager::UnRegistCollisionObject(const int layerNum, UINT* registNum)
{
    if (layerNum >= vec_collisionLayer.size()) {
        return ;
    }
    vec_collisionLayer.at(layerNum)->UnRegistCollisionObj(registNum);
}
