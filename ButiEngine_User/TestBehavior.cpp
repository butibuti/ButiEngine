#include"stdafx_u.h"
#include "TestBehavior.h"
#include"Header/GameObjects/DefaultGameComponent/TransformAnimation.h"

void ButiEngine::TestBehavior::OnUpdate()
{
    if (GameDevice::GetInput()->CheckKey(Keys::A)) {
        gameObject.lock()->transform->RollLocalRotationY_Degrees(1);
    }
    if (GameDevice::GetInput()->CheckKey(Keys::D)) {

        gameObject.lock()->transform->RollLocalRotationY_Degrees(-1);
    }
    if (GameDevice::GetInput()->CheckKey(Keys::W)) {
        gameObject.lock()->transform->RollLocalRotationX_Degrees(1);
    }
    if (GameDevice::GetInput()->CheckKey(Keys::S)) {

        gameObject.lock()->transform->RollLocalRotationX_Degrees(-1);
    }
    if (GameDevice::GetInput()->CheckKey(Keys::E)) {
        gameObject.lock()->transform->RollLocalRotationZ_Degrees(1);
    }
    if (GameDevice::GetInput()->CheckKey(Keys::R)) {

        gameObject.lock()->transform->RollLocalRotationZ_Degrees(-1);
    }

    if (GameDevice::GetInput()->TriggerKey(Keys::Space)) {
        auto AnimPlane = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("Plane_Target");
        auto anim = AnimPlane.lock()->GetGameComponent<TransformAnimation>();

        if (!anim) {
            anim= AnimPlane.lock()->AddGameComponent<TransformAnimation>();
            anim->SetSpeed(1.0 / 120);
            anim->SetTargetTransform(gameObject.lock()->transform->Clone());
            anim->SetReverse(false);
        }
    }



}

void ButiEngine::TestBehavior::OnSet()
{
}

void ButiEngine::TestBehavior::Start()
{
}

std::shared_ptr<ButiEngine::Behavior> ButiEngine::TestBehavior::Clone()
{
    return ObjectFactory::Create<TestBehavior>();
}
