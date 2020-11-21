#include "stdafx.h"
#include "PlayerLookingCamera.h"

void ButiEngine::PlayerLookingCamera::OnUpdate()
{
	if (!shp_lookTarget) { return; }
	gameObject.lock()->transform->SetLookAtRotation(shp_lookTarget->GetWorldPosition());
}

void ButiEngine::PlayerLookingCamera::OnSet()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::PlayerLookingCamera::Clone()
{
	auto ret = ObjectFactory::Create<PlayerLookingCamera>();
	ret->blendPos = blendPos;
	ret->shp_lookTarget = shp_lookTarget;
	ret->per = per;
	return ret;
}

void ButiEngine::PlayerLookingCamera::OnShowUI()
{
	ImGui::BulletText("Per");
	ImGui::SliderFloat("##per", &per, 0, 1.0);

	ImGui::BulletText("BlendPos");
	ImGui::DragFloat3("##BlendPos", &blendPos.x, 0.1,-100, 100);
	std::string target = "Looking:";
	if (shp_lookTarget) {
		target += "Existence";
	}
	else {
		target += "nullptr";
	}


	ImGui::BeginChild("##LookTransform", ImVec2((ImGui::GetFontSize()) * (target.size() + 2), ImGui::GetFontSize() * 2), true);
	ImGui::BulletText((target).c_str());
	if (shp_lookTarget) {
		ImGui::SameLine();
		if (ImGui::Button("DontLook")) {
			shp_lookTarget = nullptr;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Attach New")) {
		if (!shp_lookTarget)
			shp_lookTarget = ObjectFactory::Create<Transform>();
		else {
			shp_lookTarget = shp_lookTarget->Clone();
		}
	}

	if (ImGui::IsWindowHovered())
	{
		auto obj = gameObject.lock()->GetApplication().lock()->GetGUIController()->GetDraggingObject();


		if (obj && obj->IsThis<GameObject>()) {

			auto trans = obj->GetThis<GameObject>()->transform;
			shp_lookTarget = trans;
		}

	}
	ImGui::EndChild();
	if (shp_lookTarget) {

		shp_lookTarget->ShowUI();

	}
}
