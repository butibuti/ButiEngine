#include "stdafx.h"
#include "BuldingMover.h"


float ButiEngine::BuildingMover::endZ = -180;
float ButiEngine::BuildingMover::startZ = 230;

void ButiEngine::BuildingMover::OnUpdate()
{
    gameObject.lock()->transform->Translate(velocity * GameDevice::WorldSpeed);
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
    GUI::DragFloat3("Velocity", &velocity.x);
    GUI::DragFloat("StartZ", &startZ);
    GUI::DragFloat("EndZ", &endZ);
}
