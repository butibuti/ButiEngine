#pragma once
#include "stdafx.h"
#include"Header/GameParts/CollisionLayer.h"

bool ButiEngine::Collision::OctRegistObj::Remove()
{
    if (!p_cell)
        return false;


    if (shp_next)
    {
        shp_next->shp_bef = shp_bef;
    }
    if (shp_bef)
    {
        shp_bef->shp_next = shp_next;
    }
    p_cell->OnRemove(this);
    shp_next = nullptr;
    shp_bef = nullptr;
    p_cell = nullptr;
    return true;
}
ButiEngine::Collision::CollisionLayer::CollisionLayer(const unsigned char  arg_level, const Vector3& arg_minPos, const Vector3& arg_maxPos)
{

    if(arg_level<=MaxLevel)
        maxLevel = arg_level; 
    else{
        maxLevel = MaxLevel;
    }
    rangeMax = arg_maxPos;
    rangeMin = arg_minPos;
    width = rangeMax - rangeMin;
    OctPow[0] = 1;
    OctPowSevenDevided[0] = 0;
    for (int i = 1; i < MaxLevel + 1; i++) {
        OctPow[i] = OctPow[i - 1] * 8;
        OctPowSevenDevided[i] = (OctPow[i] - 1) / 7;
    }
    maxCellNum = OctPowSevenDevided[MaxLevel-1];

    unit = width / ((float)(1 << maxLevel));
    ary_cells = new OctCell*[maxCellNum] ();

    CreateCell(0);

}
unsigned int* ButiEngine::Collision::CollisionLayer::RegistCollisionObj(std::shared_ptr<CollisionObject> arg_collisionObj)
{

    UINT* index =new UINT( vec_shp_collisionObjs.size());

    vec_shp_collisionObjs.push_back(std::make_shared<OctRegistObj>( arg_collisionObj));

    vec_index.push_back(index);
    return index;

}

void ButiEngine::Collision::CollisionLayer::UnRegistCollisionObj(unsigned int* arg_index)
{
    auto index = *arg_index;
    if (index >= vec_shp_collisionObjs.size()) {
        return;
    }
    auto itr = vec_shp_collisionObjs.begin();
    itr += index;
    (*itr)->Remove();
    vec_shp_collisionObjs.erase(itr);

    delete arg_index;
    auto numItr = vec_index.begin() + index;
    numItr= vec_index.erase(numItr);

    for (; numItr != vec_index.end(); numItr++) {
        *(*numItr) -=1;
    }
}

void ButiEngine::Collision::CollisionLayer::Initialize()
{
}

void ButiEngine::Collision::CollisionLayer::Update()
{
    RegistOctree();
    HitCheck();
}


void ButiEngine::Collision::CollisionLayer::HitCheck()
{
    std::list<std::shared_ptr<CollisionObject>> list_objStack;
    std::vector<std::shared_ptr<CollisionObject>> vec_collisionObjects;

    CreateCollisionObjectList(0, vec_collisionObjects, list_objStack);
    //std::cout << vec_collisionObjects.size()/2 << std::endl;
    auto endItr = vec_collisionObjects.end();
    for (auto itr = vec_collisionObjects.begin(); itr != endItr;itr+=2) {
        auto hit = (*itr)->wkp_coliisionPrim.lock()->IsHit((*(itr+1))->wkp_coliisionPrim);
        if (hit) {
            (*itr)->wkp_gameObj.lock()->Hit((*(itr + 1))->wkp_gameObj.lock());
            (*(itr + 1))->wkp_gameObj.lock()->Hit((*itr)->wkp_gameObj.lock());
        }
    }
}
