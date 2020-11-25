#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\TransformAnimation.h"

void ButiEngine::TransformAnimation::OnUpdate()
{

    t += speed * direction;

    if (t >= 1.0f) {
        t = 1.0f;
        if (isReverse) {
            direction = -1;
        }
        else
        {
            SetIsRemove(true);
        }
    }else if (t <= 0.0f) {
        t = 0.0f;
        if (isReverse) {
            direction = 1;
        }
        else
        {
            SetIsRemove(true);
        }
    }

    PositionSet();

}

std::shared_ptr<ButiEngine::Transform> ButiEngine::TransformAnimation::GetInitTransform()
{
    return shp_initTransform;
}

std::shared_ptr<ButiEngine::Transform> ButiEngine::TransformAnimation::GetTargetTransform()
{
    return shp_targetTransform;
}

void ButiEngine::TransformAnimation::SetInitTransform(std::shared_ptr<Transform> arg_shp_InitTransform)
{
    shp_initTransform = arg_shp_InitTransform;
}

void ButiEngine::TransformAnimation::SetTargetTransform(std::shared_ptr<Transform> arg_shp_targetTransform)
{
    shp_targetTransform = arg_shp_targetTransform;
}

void ButiEngine::TransformAnimation::SetEaseType(const Easing::EasingType type)
{
    easeType = type;
}

void ButiEngine::TransformAnimation::SetSpeed(const float arg_speed)
{
    speed = arg_speed;
}

void ButiEngine::TransformAnimation::SetTime(const float arg_time)
{
    t = arg_time;
}

void ButiEngine::TransformAnimation::SetReverse(const bool arg_isReverse)
{
    isReverse = arg_isReverse;
}


std::shared_ptr<ButiEngine::GameComponent> ButiEngine::TransformAnimation::Clone()
{
    auto ret = ObjectFactory::Create<TransformAnimation>();
    ret->shp_targetTransform = shp_targetTransform;
    ret->t = t;
    ret->speed = speed;
    ret->direction = direction;
    ret->easeType = easeType;
    return ret;
}

void ButiEngine::TransformAnimation::OnShowUI()
{
    ImGui::BulletText("Time");
  
    if (ImGui::SliderFloat("##time", &t, 0.0, 1.0f)) {
        PositionSet();
    }
    ImGui::BulletText("Speed");

    if (ImGui::DragFloat("##speed", &speed, 0.0005f, -500, 500,"%.4f")) {
        PositionSet();
    }

    ImGui::Checkbox("IsLoop", &isReverse);

    ImGui::Text(Easing::EasingTypeNames[(int)easeType].c_str());

    if (ImGui::ArrowButton("##EaseType_UP", ImGuiDir_Up)) {
        easeType =((Easing::EasingType)(((int)easeType)+ 1));
        if ((int)easeType >(int) Easing::EasingType::Liner) {
            easeType = Easing::EasingType::Liner;
        }
    }
    ImGui::SameLine();
    if (ImGui::ArrowButton("##EaseType_Down", ImGuiDir_Down)) {

        easeType = ((Easing::EasingType)(((int)easeType) -1));
        if ((int)easeType < 0) {
            easeType = Easing::EasingType::EaseIn;
        }
    }

    if (ImGui::TreeNode("InitTransform")) {
        if (shp_initTransform) {
            shp_initTransform->ShowUI();
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("TargetTransform")) {
        if (shp_targetTransform) {
            shp_targetTransform->ShowUI();
        }
        else {
            if (ImGui::Button("Attach New")) {
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


        ImGui::BeginChild("##TargetTransform", ImVec2((ImGui::GetFontSize()) * (target.size() + 2), ImGui::GetFontSize() * 2), true);
        ImGui::BulletText((target).c_str());
        if (shp_targetTransform) {
            ImGui::SameLine();
            if (ImGui::Button("Detach")) {
                shp_targetTransform = nullptr;
            }
        }

        if (ImGui::IsWindowHovered())
        {
            auto obj = gameObject.lock()->GetApplication().lock()->GetGUIController()->GetDraggingObject();


            if (obj && obj->IsThis<GameObject>()) {
                auto trans = obj->GetThis<GameObject>()->transform;
                if (shp_targetTransform != trans) {
                    shp_targetTransform = trans;
                }
            }

        }
        ImGui::EndChild();


        ImGui::TreePop();
    }
}

void ButiEngine::TransformAnimation::PositionSet()
{
    if (!shp_targetTransform) {
        return;

    }
    if (!gameObject.lock()) {
        return;
    }
    float time = Easing::GetEase(t, easeType);
    gameObject.lock()->transform->SetLocalPosition(shp_initTransform->SetLocalPosition() + (shp_targetTransform->SetLocalPosition() - shp_initTransform->SetLocalPosition()) *time);
    gameObject.lock()->transform->SetLocalScale(shp_initTransform->GetLocalScale()+(shp_targetTransform->GetLocalScale()-shp_initTransform->GetLocalScale())*time);

    
    gameObject.lock()->transform->SetLocalRotation(Calculator::SphereLerp(shp_initTransform->GetLocalRotation().ToQuat(), shp_targetTransform->GetLocalRotation().ToQuat(),time).ToMatrix());
}

void ButiEngine::TransformAnimation::OnSet()
{
    if (!shp_initTransform) {
        shp_initTransform = gameObject.lock()->transform->Clone();
    }
}
