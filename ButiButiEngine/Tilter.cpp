#include "stdafx.h"
#include "Tilter.h"

#include"Header/GameObjects/DefaultGameComponent/TransformAnimation.h"
#include"Header/GameObjects/DefaultGameComponent/LookAtComponent.h"
void ButiEngine::Tilter::Start()
{
    SetIsRemove(true);
    auto origin = gameObject.lock()->transform->Clone();
    gameObject.lock()->transform->RollLocalRotationX_Degrees(angle);
    auto anim=gameObject.lock()->AddGameComponent<TransformAnimation>();
    anim->SetTargetTransform(origin);
    anim->SetSpeed(0.05);
    anim->SetTime(0);

}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Tilter::Clone()
{
    auto ret = ObjectFactory::Create<Tilter>();
    ret->angle = angle;
    return ret;
}

void ButiEngine::Tilter::OnShowUI()
{
    GUI::DragFloat("Angle", &angle);
}

void ButiEngine::Tilter::SetAngle(const float arg_angle)
{
    angle = arg_angle;
}
