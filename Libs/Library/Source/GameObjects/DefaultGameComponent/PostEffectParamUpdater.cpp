#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\MeshDrawComponent.h"

ButiEngine::PostEffectParamUpdater::PostEffectParamUpdater(const Vector4& arg_pase)
{
    pase = arg_pase;
}

ButiEngine::PostEffectParamUpdater::PostEffectParamUpdater()
{
}

void ButiEngine::PostEffectParamUpdater::OnUpdate()
{

    if (shp_param) {
        shp_param->Get().lightDir += pase;
    }
}

void ButiEngine::PostEffectParamUpdater::OnSet()
{
    auto drawComponent = gameObject.lock()->GetGameComponent<MeshDrawComponent>();

    if (drawComponent) {
        shp_param = drawComponent->GetCBuffer<LightVariable>("LightBuffer");
    }
}

void ButiEngine::PostEffectParamUpdater::Start()
{
    if (shp_param) {
        return;
    }

    auto drawComponent = gameObject.lock()->GetGameComponent<MeshDrawComponent>();

    if (drawComponent) {
        shp_param = drawComponent->GetCBuffer<LightVariable>("LightBuffer");
    }


}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::PostEffectParamUpdater::Clone()
{
    return ObjectFactory::Create<PostEffectParamUpdater>(pase);
}

void ButiEngine::PostEffectParamUpdater::OnShowUI()
{
    GUI::DragFloat4("##paseEdit", pase, 0.01f, -FLT_MAX, FLT_MAX);
    if (shp_param) {

        if (GUI::ArrowButton("##down", GUI::GuiDir_Left)) {
            shp_param->Get().lightDir -= pase;
        }
        if (GUI::ArrowButton("##up", GUI::GuiDir_Right)) {
            shp_param->Get().lightDir += pase;
        }
    }
}
