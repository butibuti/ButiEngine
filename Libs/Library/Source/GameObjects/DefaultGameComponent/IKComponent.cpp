#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\IKComponent.h"
#include"Header/Resources/Bone.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\ModelDrawComponent.h"

void ButiEngine::IKComponent::OnRemove()
{

    shp_modelData = nullptr;
    vec_endBones.clear();
    vec_ikData.clear();
}

void ButiEngine::IKComponent::OnSet()
{
    auto model = gameObject.lock()->GetGameComponent<ModelDrawComponent>();
    if (model) {
        shp_modelData = model->GetModelData();


        RegistIK();
    }

    /*auto bones = shp_modelData->GetBones();
    for (auto itr = bones.begin(); itr != bones.end(); itr++) {
        auto boneTrans = ObjectFactory::Create<Transform>(Vector3(), Vector3(), Vector3(0.1, 0.1, 0.1));
        boneTrans->SetBaseTransform((*itr)->transform,true);
        auto   drawcomp = ObjectFactory::Create<MeshDrawComponent>(gameObject.lock()->GetResourceContainer()->GetMeshTag("Cube_UV_Normal"), gameObject.lock()->GetResourceContainer()->GetShaderTag("Shader/Compiled/OnlyMaterial"), gameObject.lock()->GetResourceContainer()->GetMaterialTag("Red"), nullptr, 0,boneTrans);

        gameObject.lock()->AddGameComponent(drawcomp);
        
    }*/

    

}

void ButiEngine::IKComponent::Start()
{
    if (shp_modelData) {
        return;
    }
    auto model = gameObject.lock()->GetGameComponent<ModelDrawComponent>();
    if (model) {
        shp_modelData = model->GetModelData();
        RegistIK();

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
            if (GUI::TreeNode(Util::ToUTF8( ((*itr)->boneName)))) {
                if ((*itr)->transform->ShowUI()) {
                    isEdit = true;
                }
                GUI::TreePop();
            }
        }
        if (GameDevice::GetInput()->TriggerKey(Keys::A)) {

            shp_modelData->InverseKinematic();
            shp_modelData->BonePowerAdd();
        }
        GUI::TreePop();
    }
    if (GUI::TreeNode("Edit IK")) {
        for (auto itr = vec_ikData.begin(); itr != vec_ikData.end(); itr++) {
            auto target = shp_modelData->SerchBoneByIndex((*itr)->extremityBoneIndex);
            if(target&& GUI::TreeNode(target->boneName) ){
                int i = 0;
                for (auto linkItr = target->ikDatas.begin()->links.begin(); linkItr != target->ikDatas.begin()->links.end();i++, linkItr++) {
                    bool isEdit = false;
                    if (GUI::TreeNode(linkItr->shp_linkBone->boneName)) {
                        if (GUI::Checkbox("IK Constraint", &linkItr->radianLimit)) {
                            (*itr)->links[i].radianLimit = linkItr->radianLimit;
                        }

                        if (linkItr->radianLimit) {
                            Vector3 upLimit = linkItr->upLimit;
                            upLimit.ToDegrees();
                            if (GUI::DragFloat3("UpLimit", upLimit)) {
                                upLimit.ToRadian();
                                upLimit = Matrix4x4().CreateFromEuler_local(upLimit).GetEulerOneValue_local();
                                linkItr->upLimit = upLimit;
                                (*itr)->links[i].upLimit = upLimit;
                            }

                            Vector3 downLimit = linkItr->downLimit;
                            downLimit.ToDegrees();
                            if (GUI::DragFloat3("DownLimit", downLimit)) {
                                downLimit.ToRadian();
                                downLimit = Matrix4x4().CreateFromEuler_local(downLimit).GetEulerOneValue_local();
                                linkItr->downLimit = downLimit;
                                (*itr)->links[i].downLimit = downLimit;
                            }
                        }


                        GUI::TreePop();
                    }
                    
                    

                }

                GUI::TreePop();
            }
        }
        GUI::TreePop();
    }

    if (GUI::TreeNode("Add IK")) {
        static char targetBoneName[128];
        static char linkBoneName[128];
        static int linkCount = 0;


        GUI::BulletText("TargetBone");
        GUI::InputText("TargetBone", targetBoneName, 128);
        GUI::BulletText("LinkBone");
        GUI::InputText("LinkBone", linkBoneName, 128);
        GUI::BulletText("linkCount");
        GUI::InputInt("linkCount", linkCount);

        if (GUI::Button("Add")) {
            std::string boneStr = std::string(targetBoneName);
            std::string linkBoneStr =std::string( linkBoneName);

            auto target = shp_modelData->SerchBoneByName(boneStr);
            auto extremity = shp_modelData->SerchBoneByName(linkBoneStr);
            if (target&&extremity) {

                std::shared_ptr< IKData> data=std::make_shared<IKData>();
                data->loopCount = 30;
                data->targetBoneIndex = extremity ->ownIndex;
                data->extremityBoneIndex = target->ownIndex;
                std::shared_ptr<Bone> parentBone=extremity;
                for (int i = 0; i < linkCount; i++) {
                    parentBone = parentBone->parentBone;
                    if (parentBone == nullptr) {
                        break;
                    }
                    IKLink link;
                    link.linkBone = parentBone->ownIndex;
                    //link.shp_linkBone = parentBone;
                    data->links.push_back(link);
                }
                vec_ikData.push_back(data);

            }
            RegistIK();
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

void ButiEngine::IKComponent::RegistIK()
{
    auto endItr = vec_ikData.end();
    for (auto itr = vec_ikData.begin(); itr != endItr; itr++) {
        auto target = shp_modelData->SerchBoneByIndex((*itr)->extremityBoneIndex);
        if (target) {
            for (auto tItr = target->ikDatas.begin(); tItr != target->ikDatas.end(); tItr++) {
                if ((tItr)->targetBoneIndex == (*itr)->targetBoneIndex) {
                    continue;
                }
            }
            target->ikDatas.push_back(*(*itr));
            target->SetOtherBoneLinks(shp_modelData->vec_bone);
        }
    }
    shp_modelData->SetIKBone();
    vec_endBones.clear();
    vec_endBones = shp_modelData->GetIKBones();
}
