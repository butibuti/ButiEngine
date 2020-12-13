#include "stdafx.h"
#include "..\..\Header\GameParts\CollisionManager.h"



ButiEngine::Collision::CollisionManager::CollisionManager(const int arg_initLayerCount)
{
    for(int i=0;i<arg_initLayerCount;i++)
    vec_collisionLayer.push_back(ObjectFactory::Create<CollisionLayer<GameObject>>(5,Vector3(-30,-30,-30),Vector3(30,30,30)));
}

void ButiEngine::Collision::CollisionManager::Update()
{

    for (auto itr = vec_collisionLayer.begin(); itr != vec_collisionLayer.end(); itr++) {
        (*itr)->Update();
        std::vector<std::shared_ptr<CollisionObject_GameObject>> vec_collisionObjects;
        (*itr)->Check(vec_collisionObjects);


        auto endItr = vec_collisionObjects.end();
        for (auto itr = vec_collisionObjects.begin(); itr != endItr; itr += 2) {
            auto hit = (*itr)->wkp_coliisionPrim.lock()->IsHit((*(itr + 1))->wkp_coliisionPrim);
            if (hit) {
                (*itr)->wkp_obj.lock()->Hit((*(itr + 1))->wkp_obj.lock());
                (*(itr + 1))->wkp_obj.lock()->Hit((*itr)->wkp_obj.lock());
            }
        }

    }

}

UINT* ButiEngine::Collision::CollisionManager::RegistCollisionObject(const int layerNum, std::shared_ptr< Collision::CollisionPrimitive>arg_prim, std::shared_ptr<GameObject> arg_registObj)
{
    if (layerNum >= vec_collisionLayer.size()) {
        return nullptr;
    }
    return vec_collisionLayer.at(layerNum)->RegistCollisionObj(arg_prim,arg_registObj);
}

void ButiEngine::Collision::CollisionManager::UnRegistCollisionObject(const int layerNum, UINT* registNum)
{
    if (layerNum >= vec_collisionLayer.size()) {
        return ;
    }
    vec_collisionLayer.at(layerNum)->UnRegistCollisionObj(registNum);
}

UINT ButiEngine::Collision::CollisionManager::GetLayerCount()
{
    return vec_collisionLayer.size();
}
