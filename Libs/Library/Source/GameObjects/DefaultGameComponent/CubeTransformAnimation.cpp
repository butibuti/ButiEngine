#include "stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\CubeTransformAnimation.h"


void ButiEngine::CubeTransformAnimation::SetXEaseType(const Easing::EasingType type)
{
	xEaseType = type;
}

void ButiEngine::CubeTransformAnimation::SetYEaseType(const Easing::EasingType type)
{
	yEaseType = type;
}

void ButiEngine::CubeTransformAnimation::SetZEaseType(const Easing::EasingType type)
{
	zEaseType = type;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::CubeTransformAnimation::Clone()
{
	auto output = ObjectFactory::Create<CubeTransformAnimation>();
	output->easeType = easeType;
	output->xEaseType = xEaseType;
	output->yEaseType = yEaseType;
	output->zEaseType = zEaseType;

	output->shp_targetTransform = shp_targetTransform;
	output->t = t;
	output->speed = speed;
	output->direction = direction;
	return output;
}

void ButiEngine::CubeTransformAnimation::OnShowUI()
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
        if ((int)easeType > (int)Easing::EasingType::CubeRotateMin180) {
            easeType = Easing::EasingType::CubeRotateMin180;
        }
    }
    GUI::SameLine();
    if (GUI::ArrowButton("##EaseType_Down", GUI::GuiDir_Down)) {

        easeType = ((Easing::EasingType)(((int)easeType) - 1));
        if ((int)easeType < 0) {
            easeType = Easing::EasingType::EaseIn;
        }
    }


    GUI::Text("X:" + Easing::EasingTypeNames[(int)xEaseType]);

    if (GUI::ArrowButton("##xEaseType_UP", GUI::GuiDir_Up)) {
        xEaseType = ((Easing::EasingType)(((int)xEaseType) + 1));
        if ((int)xEaseType > (int)Easing::EasingType::CubeRotateMin180) {
            xEaseType = Easing::EasingType::CubeRotateMin180;
        }
    }
    GUI::SameLine();
    if (GUI::ArrowButton("##xEaseType_Down", GUI::GuiDir_Down)) {

        xEaseType = ((Easing::EasingType)(((int)xEaseType) - 1));
        if ((int)easeType < 0) {
            xEaseType = Easing::EasingType::EaseIn;
        }
    }

    GUI::Text("Y:" + Easing::EasingTypeNames[(int)yEaseType]);

    if (GUI::ArrowButton("##yEaseType_UP", GUI::GuiDir_Up)) {
        yEaseType = ((Easing::EasingType)(((int)yEaseType) + 1));
        if ((int)yEaseType > (int)Easing::EasingType::CubeRotateMin180) {
            yEaseType = Easing::EasingType::CubeRotateMin180;
        }
    }
    GUI::SameLine();
    if (GUI::ArrowButton("##yEaseType_Down", GUI::GuiDir_Down)) {

        yEaseType = ((Easing::EasingType)(((int)yEaseType) - 1));
        if ((int)easeType < 0) {
            yEaseType = Easing::EasingType::EaseIn;
        }
    }

    GUI::Text("Z:" + Easing::EasingTypeNames[(int)zEaseType]);

    if (GUI::ArrowButton("##zEaseType_UP", GUI::GuiDir_Up)) {
        zEaseType = ((Easing::EasingType)(((int)zEaseType) + 1));
        if ((int)zEaseType > (int)Easing::EasingType::CubeRotateMin180) {
            zEaseType = Easing::EasingType::CubeRotateMin180;
        }
    }
    GUI::SameLine();
    if (GUI::ArrowButton("##zEaseType_Down", GUI::GuiDir_Down)) {

        zEaseType = ((Easing::EasingType)(((int)zEaseType) - 1));
        if ((int)easeType < 0) {
            zEaseType = Easing::EasingType::EaseIn;
        }
    }

    if (GUI::TreeNode("InitTransform")) {
        if (shp_initTransform) {
            shp_initTransform->ShowUI();
        }
        GUI::TreePop();
    }

    if (GUI::TreeNode("TargetTransform")) {
        if (shp_targetTransform) {
            shp_targetTransform->ShowUI();
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

void ButiEngine::CubeTransformAnimation::PositionSet()
{
    if (!shp_targetTransform) {
        return;

    }
    if (!gameObject.lock()) {
        return;
    }
    float time = Easing::GetEase(t, easeType);
    gameObject.lock()->transform->SetLocalPosition(shp_initTransform->SetLocalPosition() + (shp_targetTransform->SetLocalPosition() - shp_initTransform->SetLocalPosition()) * time+Vector3(Easing::GetEase(time,xEaseType), Easing::GetEase(time, yEaseType), Easing::GetEase(time, zEaseType)));
    gameObject.lock()->transform->SetLocalScale(shp_initTransform->GetLocalScale() + (shp_targetTransform->GetLocalScale() - shp_initTransform->GetLocalScale()) * time);


    gameObject.lock()->transform->SetLocalRotation(MathHelper::LearpQuat(shp_initTransform->GetLocalRotation().ToQuat(), shp_targetTransform->GetLocalRotation().ToQuat(), time).ToMatrix());

}
