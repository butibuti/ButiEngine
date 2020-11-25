#include "stdafx.h"
#include "BuldingMover.h"


float ButiEngine::BuildingMover::endZ = -180;
float ButiEngine::BuildingMover::startZ = 230;

void ButiEngine::BuildingMover::OnUpdate()
{
    gameObject.lock()->transform->Translate(velocity);
    if (gameObject.lock()->transform->GetLocalPosition().z < endZ) {
        gameObject.lock()->transform->SetLocalPosition().z = startZ;
    }
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::BuildingMover::Clone()
{
    auto ret=ObjectFactory::Create<BuildingMover>();
    ret->velocity = velocity;
    return ret;
}

void ButiEngine::BuildingMover::OnShowUI()
{
    ImGui::DragFloat3("Velocity", &velocity.x);
    ImGui::DragFloat("StartZ", &startZ);
    ImGui::DragFloat("EndZ", &endZ);
}
