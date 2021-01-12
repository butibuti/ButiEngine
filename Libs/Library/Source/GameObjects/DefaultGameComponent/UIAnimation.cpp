#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\UIAnimation.h"

void ButiEngine::UIAnimation::SetInitTransform(std::shared_ptr<Transform> arg_shp_InitTransform)
{
    shp_initTransform= arg_shp_InitTransform->Clone();

    shp_initTransform->SetLocalPosition().x /= size.x;
    shp_initTransform->SetLocalPosition().y /= size.y;
    shp_initTransform->SetLocalScale().x /= size.x;
    shp_initTransform->SetLocalScale().y /= size.y;
}

void ButiEngine::UIAnimation::SetTargetTransform(std::shared_ptr<Transform> arg_shp_targetTransform)
{
    shp_targetTransform = arg_shp_targetTransform->Clone();
    shp_targetTransform->SetLocalPosition().x /= size.x;
    shp_targetTransform->SetLocalPosition().y /= size.y;
    shp_targetTransform->SetLocalScale().x /= size.x;
    shp_targetTransform->SetLocalScale().y /= size.y;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::UIAnimation::Clone()
{
	return ObjectFactory::Create<UIAnimation>();
}

void ButiEngine::UIAnimation::PositionSet()
{
    if (!shp_targetTransform) {
        return;

    }
    if (!gameObject.lock()) {
        return;
    }
    float time = Easing::GetEase(t, easeType);
    gameObject.lock()->transform->SetLocalPosition(shp_initTransform->SetLocalPosition() + (shp_targetTransform->SetLocalPosition() - shp_initTransform->SetLocalPosition()) * time);
    gameObject.lock()->transform->SetLocalPosition().x *= size.x;
    gameObject.lock()->transform->SetLocalPosition().y *= size.y;
    gameObject.lock()->transform->SetLocalScale(shp_initTransform->GetLocalScale() + (shp_targetTransform->GetLocalScale() - shp_initTransform->GetLocalScale()) * time);
    gameObject.lock()->transform->SetLocalScale().x *= size.x;
    gameObject.lock()->transform->SetLocalScale().y *= size.y;

    gameObject.lock()->transform->SetLocalRotation(MathHelper::LearpQuat(shp_initTransform->GetLocalRotation().ToQuat(), shp_targetTransform->GetLocalRotation().ToQuat(), time).ToMatrix());

}

void ButiEngine::UIAnimation::OnShowUI()
{
    GUI::BulletText("Time");

    if (GUI::SliderFloat("##time", &t, 0.0, 1.0f)) {
        PositionSet();
    }
    GUI::BulletText("Speed");

    if (GUI::DragFloat("##speed", &speed, 0.0005f, -500, 500, "%.4f")) {
        PositionSet();
    }

    GUI::Checkbox("IsLoop", &isReverse);

    GUI::Text(Easing::EasingTypeNames[(int)easeType].c_str());

    if (GUI::ArrowButton("##EaseType_UP", GUI::GuiDir_Up)) {
        easeType = ((Easing::EasingType)(((int)easeType) + 1));
        if ((int)easeType > (int)Easing::EasingType::CubeRotate90) {
            easeType = Easing::EasingType::CubeRotate90;
        }
    }
    GUI::SameLine();
    if (GUI::ArrowButton("##EaseType_Down", GUI::GuiDir_Down)) {

        easeType = ((Easing::EasingType)(((int)easeType) - 1));
        if ((int)easeType < 0) {
            easeType = Easing::EasingType::EaseIn;
        }
    }

    if (GUI::TreeNode("InitTransform")) {
        if (shp_initTransform) {
            if (shp_initTransform->ShowUI()) {
                PositionSet();
            }
        }
        GUI::TreePop();
    }

    if (GUI::TreeNode("TargetTransform")) {
        if (shp_targetTransform) {

            if (shp_targetTransform->ShowUI()) {
                PositionSet();
            }
        }
        else {
            if (GUI::Button("Attach New")) {
                shp_targetTransform = ObjectFactory::Create<Transform>();
            }
        }
        std::string target = "Target:";
        if (shp_targetTransform) {
            target += "Existence";
        }
        else {
            target += "nullptr";
        }


        GUI::BeginChild("##TargetTransform", Vector2((GUI::GetFontSize()) * (target.size() + 2), GUI::GetFontSize() * 2), true);
        GUI::BulletText((target).c_str());
        if (shp_targetTransform) {
            GUI::SameLine();
            if (GUI::Button("Detach")) {
                shp_targetTransform = nullptr;
            }
        }

        if (GUI::IsWindowHovered())
        {
            auto obj = gameObject.lock()->GetApplication().lock()->GetGUIController()->GetDraggingObject();


            if (obj && obj->IsThis<GameObject>()) {
                auto trans = obj->GetThis<GameObject>()->transform;
                if (shp_targetTransform != trans) {
                    shp_targetTransform = trans;
                }
            }

        }
        GUI::EndChild();


        GUI::TreePop();
    }
}

void ButiEngine::UIAnimation::OnSet()
{
    if (!shp_initTransform) {
        shp_initTransform = gameObject.lock()->transform->Clone();
    }
    size.x = gameObject.lock()->GetApplication().lock()->GetWindow()->GetSize().x;
    size.y= gameObject.lock()->GetApplication().lock()->GetWindow()->GetSize().y;
}
