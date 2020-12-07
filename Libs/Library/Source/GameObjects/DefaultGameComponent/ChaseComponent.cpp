#include "stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\ChaseComponent.h"


ButiEngine::ChaseComponent::ChaseComponent(std::shared_ptr<Transform> arg_shp_target, const float arg_speed)
{
	shp_target = arg_shp_target;
	speed = arg_speed;
}

void ButiEngine::ChaseComponent::OnUpdate()
{
	if (!shp_target) { return; }
	auto velocity = Vector3(shp_target->GetWorldPosition() - gameObject.lock()->transform->GetWorldPosition());
	gameObject.lock()->Translate (velocity.GetNormalize() * (velocity.GetLength() * velocity.GetLength() )*speed*0.01f);

}

void ButiEngine::ChaseComponent::OnSet()
{
}

void ButiEngine::ChaseComponent::OnShowUI()
{
	GUI::SliderFloat("speed", &speed, 0.0, 1.0, "%.3f");


	std::string target = "ChaseTransform:";
	if (shp_target) {
		target += "Existence";
	}
	else {
		target += "nullptr";
	}


	GUI::BeginChild("##BaseTransform", Vector2((GUI::GetFontSize()) * (target.size() + 2), GUI::GetFontSize() * 2), true);
	GUI::BulletText((target).c_str());
	if (shp_target) {
		GUI::SameLine();
		if (GUI::Button("Detach")) {
			shp_target = nullptr;
		}
	}
	GUI::SameLine();
	if (GUI::Button("Attach New")) {
		if (!shp_target)
			shp_target=ObjectFactory::Create<Transform>();
		else {
			shp_target = shp_target->GetBaseTransform()->Clone();
		}
	}

	if (GUI::IsWindowHovered())
	{
		auto obj =gameObject.lock()-> GetApplication().lock()->GetGUIController()->GetDraggingObject();


		if (obj && obj->IsThis<GameObject>()) {

			auto trans = obj->GetThis<GameObject>()->transform;
			shp_target = trans;
		}

	}
	GUI::EndChild();

}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ChaseComponent::Clone()
{
	return ObjectFactory::Create<ChaseComponent>(shp_target, speed);
}
