#include "stdafx.h"
#include "GameStarter.h"

#include"PlayerLookingCamera.h"
void ButiEngine::GameStarter::Start()
{
    gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("Player").lock()->GetBehavior("PlayerBehavior")->SetIsActive(false);
    gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("EnemySpawner").lock()->GetGameComponent("EnemySpawner")->SetIsActive(false);


}

void ButiEngine::GameStarter::OnUpdate()
{
    if (shp_relTimer->Update()) {
            auto player = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("Player");
            player .lock()->GetBehavior("PlayerBehavior")->SetIsActive(true);
            gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("EnemySpawner").lock()->GetGameComponent("EnemySpawner")->SetIsActive(true);
            
            auto playerLooker= gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("camera").lock()->AddGameComponent<PlayerLookingCamera>();
            playerLooker->SetBlendPos(Vector3());
            playerLooker->SetPer(0.3);
            playerLooker->SetTarget(player.lock()->transform);

            SetIsRemove(true);
            GameDevice::WorldSpeed = 1.0f;
    }
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::GameStarter::Clone()
{
    auto ret = ObjectFactory::Create<GameStarter>();
    if(shp_relTimer)
    ret->shp_relTimer = shp_relTimer->Clone();
    ret->requestPush = requestPush;
    return ret;
}

void ButiEngine::GameStarter::OnShowUI()
{
    GUI::DragInt("RequestPush", requestPush);
    if(shp_relTimer)
    shp_relTimer->ShowGUI();
}

void ButiEngine::GameStarter::OnSet()
{
    if (!shp_relTimer) {

        shp_relTimer = ObjectFactory::Create<RelativeTimer>(60);
        shp_relTimer->Start();
    }
}
