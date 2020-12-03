#include "stdafx.h"
#include "CloudMover.h"

float ButiEngine::CloudMover::endZ = -70;
float ButiEngine::CloudMover::startZ = 130;

void ButiEngine::CloudMover::OnUpdate()
{
    gameObject.lock()->transform->Translate(velocity * GameDevice::WorldSpeed);
    if (gameObject.lock()->transform->GetLocalPosition().z < endZ) {
        gameObject.lock()->transform->SetLocalPosition().z = startZ;
    }
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::CloudMover::Clone()
{
    auto ret = ObjectFactory::Create<CloudMover>();
    ret->velocity = velocity;
    return ret;
}

void ButiEngine::CloudMover::OnShowUI()
{
    GUI::DragFloat3("Velocity", &velocity.x);
    GUI::DragFloat("StartZ", &startZ);
    GUI::DragFloat("EndZ", &endZ);
}
