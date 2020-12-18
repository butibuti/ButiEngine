#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\IKComponent.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\ModelDrawComponent.h"

void ButiEngine::IKComponent::OnSet()
{
    auto model = gameObject.lock()->GetGameComponent<ModelDrawComponent>();
    if (model) {
        shp_modelData = model->GetModelData();


        std::map<std::shared_ptr<Bone>, int> map;
        auto vec_ikBone = shp_modelData->GetIKBones();
        auto endItr = vec_ikBone.end();
        for (auto itr = vec_ikBone.begin(); itr != endItr; itr++) {
            for (auto dataItr = (*itr)->ikDatas.begin(); dataItr != (*itr)->ikDatas.end(); dataItr++) {
                if (!map.count(dataItr->shp_targetBone)) 
                    map.emplace(dataItr->shp_targetBone, 0);
            }
        }
        auto endmapItr = map.end();
        for (auto mapItr = map.begin(); mapItr != endmapItr; mapItr++) {
            vec_endBones.push_back(mapItr->first);
        }
    }
}

void ButiEngine::IKComponent::Start()
{
    if (shp_modelData) {
        return;
    }
    auto model = gameObject.lock()->GetGameComponent<ModelDrawComponent>();
    if (model) {
        shp_modelData = model->GetModelData();

        std::map<std::shared_ptr<Bone>, int> map;
        auto vec_ikBone = shp_modelData->GetIKBones();
        auto endItr = vec_ikBone.begin();
        for (auto itr = vec_ikBone.begin(); itr != endItr; itr++) {
            for (auto dataItr = (*itr)->ikDatas.begin(); dataItr != (*itr)->ikDatas.end(); dataItr++) {
                if(!map.count(dataItr->shp_targetBone)) map.emplace( dataItr->shp_targetBone,0);
            }
        }
        auto endmapItr = map.end();
        for (auto mapItr = map.begin(); mapItr != endmapItr; mapItr++) {
            vec_endBones.push_back(mapItr->first);
        }

    }
}

std::shared_ptr< ButiEngine::GameComponent> ButiEngine::IKComponent::Clone()
{
    return ObjectFactory::Create<IKComponent>();
}

void ButiEngine::IKComponent::OnShowUI()
{
    if(GUI::TreeNode("EndEffectors") ){
        auto endItr = vec_endBones.end();
        bool isEdit = false;
        for (auto itr = vec_endBones.begin(); itr != endItr; itr++) {
            if (GUI::TreeNode(Util::ToUTF8( Util::WStringToString((*itr)->boneName)))) {
                if ((*itr)->transform->ShowUI()) {
                    isEdit = true;
                }
                GUI::TreePop();
            }
        }
        if (isEdit) {

            shp_modelData->InverseKinematic();
            shp_modelData->BonePowerAdd();
        }
        GUI::TreePop();
    }
}

void ButiEngine::IKComponent::OnUpdate()
{
    if (shp_modelData) {
        shp_modelData->InverseKinematic();
        shp_modelData->BonePowerAdd();
    }
}
