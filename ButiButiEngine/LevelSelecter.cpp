#include "stdafx.h"
#include "LevelSelecter.h"
#include "CloudMover.h"
#include"Header/GameObjects/DefaultGameComponent/TransformAnimation.h"
#include"Header/GameObjects/DefaultGameComponent/LookAtComponent.h"
#include"Header/GameObjects/DefaultGameComponent/MeshDrawComponent.h"
#include"Header/GameParts/ResourceContainer.h"
#include"BuldingMover.h"
void ButiEngine::LevelSelecter::Start()
{
    wkp_cam = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("camera");
    shp_initCameraTransform = wkp_cam.lock()->transform->Clone();
    shp_initCameraTransform->RollLocalRotationX_Degrees(90);
    CloudMover::startZ = 530;
    BuildingMover::startZ = 630;
    auto sceneManager= gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock();
    
    for (int i = 0; i < max; i++) {
        sceneManager->RemoveScene("Stage_"+std::to_string(i+1));

        sceneManager->LoadScene("Stage_" + std::to_string(i + 1), ObjectFactory::Create<Scene>(sceneManager, SceneInformation("Stage_" + std::to_string(i + 1))));
    }
    wkp_draw= gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("LevelText").lock()->GetGameComponent<MeshDrawComponent>();
}

void ButiEngine::LevelSelecter::OnUpdate()
{
    if (GameDevice::input.GetLeftStick().x >= 0.1 || GameDevice::input.TriggerKey(Keys::D)) {
        auto transformanim = wkp_cam.lock()->GetGameComponent<TransformAnimation>();
        if (transformanim) {
            return;
        }

        auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("se_Select.wav", "Sound/");

        gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.05);
        nowSelectNum++;
        nowSelectNum = min(max-1, nowSelectNum);

        auto anim = wkp_cam.lock()->AddGameComponent<TransformAnimation>();
        auto targetTransform = shp_initCameraTransform->Clone();
        targetTransform->TranslateZ( nowSelectNum * unit);

        anim->SetTargetTransform(targetTransform);
        anim->SetSpeed(0.05);

        auto levelTextTag= gameObject.lock()->GetResourceContainer()->GetMaterialTag("UI_Stage" + std::to_string(nowSelectNum+1));

        wkp_draw.lock()->SetMaterialTag(levelTextTag); 
        wkp_draw.lock()->ReRegist();

    }else if (GameDevice::input.GetLeftStick().x <= -0.1||GameDevice::input.TriggerKey(Keys::A)) {



        auto transformanim = wkp_cam.lock()->GetGameComponent<TransformAnimation>();
        if (transformanim) {
            return;
        }
        auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("se_Select.wav", "Sound/");

        gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.05);
        nowSelectNum--;
        nowSelectNum = max(0, nowSelectNum);
        auto anim = wkp_cam.lock()->AddGameComponent<TransformAnimation>();
        auto targetTransform = shp_initCameraTransform->Clone();
        targetTransform->TranslateZ( nowSelectNum * unit);
        anim->SetTargetTransform(targetTransform);
        anim->SetSpeed(0.05);

        auto levelTextTag = gameObject.lock()->GetResourceContainer()->GetMaterialTag("UI_Stage" + std::to_string(nowSelectNum+1));

        wkp_draw.lock()->SetMaterialTag(levelTextTag);
        wkp_draw.lock()->ReRegist();
    }
    
    if (GameDevice::input.GetPadButtonTriger(PadButtons::XBOX_A) || GameDevice::input.TriggerKey(Keys::Space)) {


        auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("se_Decision.wav", "Sound/");

        gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.05);

        auto transformanim = wkp_cam.lock()->GetGameComponent<TransformAnimation>();
        if (transformanim) {
            return;
        } 
        auto anim = wkp_cam.lock()->AddGameComponent<TransformAnimation>();
        auto targetTransform = wkp_cam.lock()->transform->Clone();
        targetTransform->RollLocalRotationX_Degrees(-90);
        anim->SetTargetTransform(targetTransform);
        anim->SetSpeed(0.05);
        gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->ChangeScene("Stage_"+std::to_string(nowSelectNum+1), 60);
        SetIsRemove(true);

        auto uibase= gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("UIBase");
        if (uibase.lock()) {
            auto uianim= uibase.lock()->AddGameComponent<TransformAnimation>();
            float y= gameObject.lock()->GetApplication().lock()->GetWindow()->GetSize().y;
            auto uiTarget = ObjectFactory::Create<Transform>(Vector3(0, -(y), 0));
            uianim->SetSpeed(0.05);
            uianim->SetTargetTransform(uiTarget);
        }
    }

}

std::shared_ptr< ButiEngine::GameComponent> ButiEngine::LevelSelecter::Clone()
{
    auto ret = ObjectFactory::Create<LevelSelecter>();;
    ret->nowSelectNum = nowSelectNum;
    ret->max = max;
    return ret;
}

void ButiEngine::LevelSelecter::OnShowUI()
{
    ImGui::DragInt("MaxStage", &max);
    ImGui::DragFloat("Unit", &unit);
}
