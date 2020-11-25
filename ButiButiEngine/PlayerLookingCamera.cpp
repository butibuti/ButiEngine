#include "stdafx.h"
#include "PlayerLookingCamera.h"

void ButiEngine::PlayerLookingCamera::OnUpdate()
{
	if (!shp_lookTarget) { return; }

	auto lookPos = per* shp_lookTarget->GetWorldPosition() + (1 - per) * blendPos;

	gameObject.lock()->transform->SetLookAtRotation(lookPos);
	
	if (abs(targetPer - per) > 0.03) {
		per += 0.02 * (targetPer - per) / abs(targetPer - per);
	}

}

void ButiEngine::PlayerLookingCamera::OnSet()
{
}

void ButiEngine::PlayerLookingCamera::Detach()
{
	shp_lookTarget = nullptr;
}

void ButiEngine::PlayerLookingCamera::SetBlendPos(const Vector3& arg_pos)
{
	blendPos = arg_pos;
}

void ButiEngine::PlayerLookingCamera::SetPer(const float arg_per)
{

	 targetPer= arg_per;
}

void ButiEngine::PlayerLookingCamera::SetTarget(std::shared_ptr<Transform> arg_shp_lookTarget)
{
	shp_lookTarget = arg_shp_lookTarget;
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
