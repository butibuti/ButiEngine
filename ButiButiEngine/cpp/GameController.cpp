#include "stdafx.h"
#include "..\include\GameController.h"

void ButiEngine::GameController::Start()
{
	auto manager = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock();
	manager->RemoveScene(befStage);
}

void ButiEngine::GameController::OnUpdate()
{
	if (nowScore>=targetScore) {
		shp_timer->Start();
	}

	if (shp_timer->Update()) {
		gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->ChangeScene(nextStage);
	}

}

void ButiEngine::GameController::OnSet()
{
	if (!shp_timer)
		shp_timer = ObjectFactory::Create<RelativeTimer>(60);
}

int ButiEngine::GameController::AddScore(int arg_score)
{
	nowScore += arg_score;

	if (nowScore >= targetScore) {
		auto manager = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock();
		manager->LoadScene(nextStage, ObjectFactory::Create<Scene>(manager, SceneInformation(nextStage)));
		manager->ChangeScene(nextStage, 120);
	}

	return nowScore;
	
}

void ButiEngine::GameController::Failed()
{
	gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->ChangeScene("FailedScene",0);
}

void ButiEngine::GameController::OnShowUI()
{
	ImGui::BulletText("StageRange_Max");
	ImGui::DragFloat3("##max", &stageMax.x, 0.1f, -80.0f, 80.0f);
	ImGui::BulletText("StageRange_Min");
	ImGui::DragFloat3("##min", &stageMin.x, 0.1f, -80.0f, 80.0f);
	ImGui::BulletText("TargetScore");
	ImGui::DragInt("##TargetScore", &targetScore, 1.0, 0, 10000000000);
	ImGui::BulletText("NowScore");
	ImGui::DragInt("##NowScore", &nowScore, 1.0, 0, 10000000000);
	ImGui::BulletText("NextStageName");

	ImGui::InputTextWithHint("##Name", nextStage.c_str(), CallBacks::objectName, 64, 64, CallBacks::ImguiCallBack);
	ImGui::SameLine();

	if (ImGui::Button("Change")) {
		nextStage = CallBacks::objectName;
		CallBacks::ObjectNameReset();

	}
	ImGui::BulletText("BefStageName");
	ImGui::InputTextWithHint("##befName", befStage.c_str(), CallBacks::objectName, 64, 64, CallBacks::ImguiCallBack);
	ImGui::SameLine();

	if (ImGui::Button("Change##befName")) {
		befStage = CallBacks::objectName;
		CallBacks::ObjectNameReset();

	}

	ImGui::BulletText("Timer");
	shp_timer->ShowGUI();
}
