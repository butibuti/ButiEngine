#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\IKComponent.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\ModelDrawComponent.h"

void ButiEngine::IKComponent::OnSet()
{
    auto model = gameObject.lock()->GetGameComponent<ModelDrawComponent>();
    if (model) {
        shp_modelData = model->GetModelData();


        vec_endBones = shp_modelData->GetIKBones();
    }

    auto bone = shp_modelData->SerchBoneByName(L"¶‘«‚h‚j");
    auto drawcomp = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
    if (!drawcomp) {
        
        drawcomp = ObjectFactory::Create<MeshDrawComponent>(gameObject.lock()->GetResourceContainer()->GetMeshTag("Cube_UV_Normal"), gameObject.lock()->GetResourceContainer()->GetShaderTag("Shader/Compiled/OnlyMaterial"), gameObject.lock()->GetResourceContainer()->GetMaterialTag("Red"), nullptr,0,bone->transform);

        gameObject.lock()->AddGameComponent(drawcomp);
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

        vec_endBones = shp_modelData->GetIKBones();
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
        if (GameDevice::GetInput()->TriggerKey(Keys::A)) {

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
