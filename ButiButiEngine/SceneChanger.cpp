#include "stdafx.h"
#include "SceneChanger.h"

void ButiEngine::SceneChanger::Start()
{

}

void ButiEngine::SceneChanger::OnUpdate()
{

	if (GameDevice::input.GetAnyButton() || GameDevice::input.CheckKey(Keys::Space)) {
		auto manager = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock();
		manager->RemoveScene(removeScene);
		manager->LoadScene(loadScene, ObjectFactory::Create<Scene>(manager, SceneInformation(loadScene)));
		gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->ChangeScene(nextScene,changeInterval);
	}
}

void ButiEngine::SceneChanger::OnShowUI()
{
	ImGui::BulletText("SceneChangeFrame");
	ImGui::DragInt("##SceneChangeFrame", &changeInterval, 1, 0, 600);
	if (ImGui::TreeNode("RemoveScene")) {

		ImGui::InputTextWithHint("##RemoveName", removeScene.c_str(), CallBacks::objectName, 64, 64, CallBacks::ImguiCallBack);
		ImGui::SameLine();

		if (ImGui::Button("Change")) {
			removeScene = CallBacks::objectName;
			CallBacks::ObjectNameReset();

		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("ChangeScene")) {

		ImGui::InputTextWithHint("##ChangeName", nextScene.c_str(), CallBacks::objectName, 64, 64, CallBacks::ImguiCallBack);
		ImGui::SameLine();

		if (ImGui::Button("Change")) {
			nextScene = CallBacks::objectName;
			CallBacks::ObjectNameReset();

		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("LoadScene")) {

		ImGui::InputTextWithHint("##LoadName", loadScene.c_str(), CallBacks::objectName, 64, 64, CallBacks::ImguiCallBack);
		ImGui::SameLine();

		if (ImGui::Button("Change")) {
			loadScene = CallBacks::objectName;
			CallBacks::ObjectNameReset();

		}
		ImGui::TreePop();
	}
}
