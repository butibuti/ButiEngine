#include "stdafx.h"
#include "..\..\Header\GameParts\CollisionManager.h"



ButiEngine::Collision::CollisionManager::CollisionManager(const int arg_initLayerCount)
{
    for(int i=0;i<arg_initLayerCount;i++)
    AddLayer(Vector3(30,30,30),5);
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

void ButiEngine::Collision::CollisionManager::AddLayer(const Vector3& size, const int level)
{

    vec_collisionLayer.push_back(ObjectFactory::Create<CollisionLayer<GameObject>>(level,-size/2,size/2));
    vec_collisionLayerInfo.push_back({ size,Vector3(), level });
}

void ButiEngine::Collision::CollisionManager::RemoveLayer(const int arg_layer)
{
    vec_collisionLayer.erase(vec_collisionLayer.begin() + arg_layer);
    vec_collisionLayerInfo.erase(vec_collisionLayerInfo.begin() + arg_layer);
}

void ButiEngine::Collision::CollisionManager::ReCreateLayers()
{
    vec_collisionLayer.clear();
    for (auto itr = vec_collisionLayerInfo.begin(); itr != vec_collisionLayerInfo.end(); itr++) {
        vec_collisionLayer.push_back(ObjectFactory::Create<CollisionLayer<GameObject>>(itr->level,-itr->size/2 , itr->size / 2));
    }
}

bool ButiEngine::Collision::CollisionManager::IsWillHit(std::shared_ptr<Collision::CollisionPrimitive> arg_prim, int arg_layer)
{
    if (arg_layer >= vec_collisionLayer.size()) {
        return false;
    }

    std::vector<std::shared_ptr<CollisionObject_GameObject>> vec_collisionObjects;
    vec_collisionLayer.at(arg_layer)->Check(arg_prim,vec_collisionObjects);


    auto endItr = vec_collisionObjects.end();
    for (auto itr = vec_collisionObjects.begin(); itr != endItr; itr += 2) {
        auto hit = (*itr)->wkp_coliisionPrim.lock()->IsHit(arg_prim);
        if (hit) {
            return true;
        }
    }

    return false;
}

void ButiEngine::Collision::CollisionManager::ShowGUI()
{
    GUI::Begin("CollisionManager");
    {
        static Vector3 size=Vector3(100,100,100);
        static int level=5;

        GUI::DragFloat3("AddLayerSize", size, 1.0f, 10, 5000);
        GUI::SliderInt("AddLayerLevel", level, 1, 8);

        if (GUI::Button("Add Layer")) {
            AddLayer(size,level);
        }
        int remove = -1;
        for (int i = 0; i < vec_collisionLayerInfo.size(); i++) {
            GUI::BulletText("Layer:" + std::to_string(i));
            GUI::SameLine();

            GUI::Text("Size x:" + std::to_string(vec_collisionLayerInfo.at(i).size.x) + " y:"+ std::to_string(vec_collisionLayerInfo.at(i).size.y) + " z:"+ std::to_string(vec_collisionLayerInfo.at(i).size.z));

            GUI::SameLine();
            GUI::Text("Level:" + std::to_string(vec_collisionLayerInfo.at(i).level));
            GUI::SameLine();

            if (GUI::Button("Ramove##"+std::to_string(i))) {
                remove = i;
            }
        }

        if (remove >= 0) {
            RemoveLayer(remove);
        }
    }
    GUI::End();
}

void ButiEngine::OutputCereal(const std::shared_ptr<Collision::CollisionManager>& v,  const std::string& path)
{
    std::stringstream stream;


    cereal::BinaryOutputArchive binOutArchive(stream);
    binOutArchive(v);

    std::ofstream outputFile( path, std::ios::binary);

    outputFile << stream.str();

    outputFile.close();
    stream.clear();
}

void ButiEngine::InputCereal(std::shared_ptr<Collision::CollisionManager>& v, const std::string& path)
{
    std::stringstream stream;

    std::ifstream inputFile(path, std::ios::binary);

    stream << inputFile.rdbuf();

    cereal::BinaryInputArchive binInputArchive(stream);


    binInputArchive(v);
}
