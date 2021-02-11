#include "stdafx.h"

ButiEngine::LookAtComponent::LookAtComponent(std::shared_ptr<Transform> arg_shp_lookTarget)
{
	shp_lookTarget = arg_shp_lookTarget;
}

void ButiEngine::LookAtComponent::OnUpdate()
{

	if (!shp_lookTarget) { return; }
	gameObject.lock()->transform->SetLookAtRotation(shp_lookTarget->GetWorldPosition());
}

void ButiEngine::LookAtComponent::OnSet()
{
	if (!shp_lookTarget) { return; }
	gameObject.lock()->transform->SetLookAtRotation(shp_lookTarget->GetWorldPosition());
}

void ButiEngine::LookAtComponent::Detach()
{
	shp_lookTarget = nullptr;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::LookAtComponent::Clone()
{
	return ObjectFactory::Create<LookAtComponent>(shp_lookTarget);
}

void ButiEngine::LookAtComponent::OnShowUI()
{

	std::string target = "Looking:";
	if (shp_lookTarget) {
		target += "Existence";
	}
	else {
		target += "nullptr";
	}


	GUI::BeginChild("##LookTransform", Vector2((GUI::GetFontSize())*(target.size()+2), GUI::GetFontSize()*2), true);
	GUI::BulletText((target).c_str());
	if (shp_lookTarget) {
		GUI::SameLine();
		if (GUI::Button("DontLook")) {
			shp_lookTarget = nullptr;
		}
	}
	GUI::SameLine();
	if (GUI::Button("Attach New")) {
		if(!shp_lookTarget)
		shp_lookTarget = ObjectFactory::Create<Transform>();
		else {
			shp_lookTarget = shp_lookTarget->Clone();
		}
	}

	if (GUI::IsWindowHovered())
	{
		auto obj = gameObject.lock()->GetApplication().lock()->GetGUIController()->GetDraggingObject();


		if (obj && obj->IsThis<GameObject>()) {

			auto trans= obj->GetThis<GameObject>()->transform;
			shp_lookTarget = trans;
		}

	}
	GUI::EndChild();
	if (shp_lookTarget) {

		shp_lookTarget->ShowUI();

	}

	if (!shp_lookTarget) { return; }
	gameObject.lock()->transform->SetLookAtRotation(shp_lookTarget->GetWorldPosition());
}
