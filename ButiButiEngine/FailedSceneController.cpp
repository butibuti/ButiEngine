#include "stdafx.h"
#include "FailedSceneController.h"
#include"Result.h"
#include"Header/GameObjects/DefaultGameComponent/TransformAnimation.h"
#include"Header/GameObjects/DefaultGameComponent/LookAtComponent.h"
#include"ScoreUI.h"
#include"PlayerLookingCamera.h"
#include"Header/GameParts/ResourceContainer.h"

void ButiEngine::ClearSceneController::Start()
{
	vec_ranks.resize(5);
	for (int i = 0; i < 5; i++) {

		vec_ranks.at(i) = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("ScoreUI_"+std::to_string(i+1)).lock()->GetGameComponent<ScoreUI>();
	}
	shp_currentScore= gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("ScoreUI_Current").lock()->GetGameComponent<ScoreUI>();
}

void ButiEngine::ClearSceneController::OnUpdate()
{
	if (!isLoading) {
		isLoading = true;
		ButtonSelect();
		auto vec_rancValue = Result::GetInstance()->GetScores(Result::GetInstance()->GetReplayScene());
		
		bool isRankin = false;
		for (int i = 0; i < 5; i++) {
			vec_ranks.at(i)->SetScore(vec_rancValue.at(i));

			if (!isRankin&& vec_rancValue.at(i) == Result::GetInstance()->GetCurrentScore()) {
				auto ui = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("ScoreUI_" + std::to_string(i + 1)).lock();
				auto anim= ui->AddGameComponent<TransformAnimation>();
				auto target = ui->transform->Clone();
				target->SetLocalScale() *= 1.1;
				anim->SetTargetTransform(target);
				anim->SetSpeed(0.02);
				anim->SetReverse(true);
				isRankin = true;
			}
		}
		shp_currentScore->SetScore(Result::GetInstance()->GetCurrentScore());
	}

	if (GameDevice::input.GetLeftStick().x >= 0.1 || GameDevice::input.CheckKey(Keys::A)) {
		int bef = selectButton;

		selectButton--;
		selectButton = max(0, selectButton);
		if (bef!=selectButton) {
			auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("se_Select.wav", "Sound/");

			gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.05);
			ButtonSelect();
		}


	}
	else if (GameDevice::input.GetLeftStick().x <= -0.1 || GameDevice::input.CheckKey(Keys::D)) {
		int bef = selectButton;

		selectButton++;
		selectButton = min(1, selectButton);
		if (bef != selectButton) {
			auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("se_Select.wav", "Sound/");

			gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.05);
			ButtonSelect();
		}
	}

	if (GameDevice::input.GetAnyButton() || GameDevice::input.CheckKey(Keys::Space)) {
		isLoading = false;
		auto manager = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock();
		manager->RemoveScene(Result::GetInstance()->GetReplayScene());

		auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("se_Decision.wav", "Sound/");

		gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.05);


		auto wkp_cam = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("camera");
		auto anim = wkp_cam.lock()->AddGameComponent<TransformAnimation>();
		auto targetTransform = wkp_cam.lock()->transform->Clone();
		targetTransform->RollLocalRotationX_Degrees(-90);
		targetTransform->TranslateY(10);
		anim->SetTargetTransform(targetTransform);
		anim->SetSpeed(0.05);
		SetIsRemove(true);

		if (selectButton == 0) {

			manager->LoadScene(Result::GetInstance()->GetReplayScene(), ObjectFactory::Create<Scene>(manager, SceneInformation(Result::GetInstance()->GetReplayScene())));

			gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->ChangeScene(Result::GetInstance()->GetReplayScene(), changeInterval);


		}
		else if (selectButton==1) {
			manager->RemoveScene("LevelSelectScene");
			manager->LoadScene("LevelSelectScene", ObjectFactory::Create<Scene>(manager, SceneInformation("LevelSelectScene")));

			gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->ChangeScene("LevelSelectScene", changeInterval);

		}


		auto uibase = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("UIBase");
		if (uibase.lock()) {
			auto uianim = uibase.lock()->AddGameComponent<TransformAnimation>();
			float y = gameObject.lock()->GetApplication().lock()->GetWindow()->GetSize().y;
			auto uiTarget = ObjectFactory::Create<Transform>(Vector3(0, -(y), 0));
			uianim->SetSpeed(0.05);
			uianim->SetTargetTransform(uiTarget);
		}
	}
}

void ButiEngine::ClearSceneController::OnShowUI()
{
	ImGui::BulletText("SceneChangeFrame");
	ImGui::DragInt("##SceneChangeFrame", &changeInterval, 1, 0, 600);
}

void ButiEngine::ClearSceneController::ButtonSelect()
{

	auto ui_retryButton = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("UI_Retry");
	auto ui_selectButton = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("UI_Select");

	if (selectButton == 0) {

		auto anim = ui_selectButton.lock()->GetGameComponent<TransformAnimation>();

		if (!anim) {
			anim = ui_selectButton.lock()->AddGameComponent<TransformAnimation>();
		}

		auto targetTransform = ui_selectButton.lock()->transform->Clone();
		targetTransform->SetLocalScale(Vector3(268, 38.4, 1.0));

		anim->SetTargetTransform(targetTransform);
		anim->SetSpeed(0.08);


		anim = ui_retryButton.lock()->GetGameComponent<TransformAnimation>();

		if (!anim) {
			anim = ui_retryButton.lock()->AddGameComponent<TransformAnimation>();
		}
		anim->GetInitTransform()->SetLocalScale(Vector3(268, 38.4, 1.0));
		targetTransform = ui_retryButton.lock()->transform->Clone();
		targetTransform->SetLocalScale(Vector3(402, 57.6, 1.0));

		anim->SetTargetTransform(targetTransform);
		anim->SetSpeed(0.08);
		anim->SetReverse(true);

	}
	else if (selectButton == 1) {

		auto anim = ui_retryButton.lock()->GetGameComponent<TransformAnimation>();

		if (!anim) {
			anim = ui_retryButton.lock()->AddGameComponent<TransformAnimation>();
		}

		auto targetTransform = ui_retryButton.lock()->transform->Clone();
		targetTransform->SetLocalScale(Vector3(268, 38.4, 1.0));

		anim->SetTargetTransform(targetTransform);
		anim->SetSpeed(0.08);


		anim = ui_selectButton.lock()->GetGameComponent<TransformAnimation>();

		if (!anim) {
			anim = ui_selectButton.lock()->AddGameComponent<TransformAnimation>();
		}
		anim->GetInitTransform()->SetLocalScale(Vector3(268, 38.4, 1.0));
		targetTransform = ui_selectButton.lock()->transform->Clone();
		targetTransform->SetLocalScale(Vector3(402, 57.6, 1.0));

		anim->SetTargetTransform(targetTransform);
		anim->SetSpeed(0.08);
		anim->SetReverse(true);


	}
}

void ButiEngine::FailedSceneController::Start()
{
	shp_currentScore = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("ScoreUI_Current").lock()->GetGameComponent<ScoreUI>();

}

void ButiEngine::FailedSceneController::OnUpdate()
{
	if (!isLoading) {
		isLoading = true;

		shp_currentScore->SetScore(Result::GetInstance()->GetCurrentScore());
		ButtonSelect();
		auto vec_rancValue = Result::GetInstance()->GetScores(Result::GetInstance()->GetReplayScene());

	}

	if (GameDevice::input.GetLeftStick().x >= 0.1 || GameDevice::input.CheckKey(Keys::A)) {
		int bef = selectButton;

		selectButton--;
		selectButton = max(0, selectButton);
		if (bef != selectButton) {
			auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("se_Select.wav", "Sound/");

			gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.05);
			ButtonSelect();
		}


	}
	else if (GameDevice::input.GetLeftStick().x <= -0.1 || GameDevice::input.CheckKey(Keys::D)) {
		int bef = selectButton;

		selectButton++;
		selectButton = min(1, selectButton);
		if (bef != selectButton) {
			auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("se_Select.wav", "Sound/");

			gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.05);
			ButtonSelect();
		}
	}

	if (GameDevice::input.GetAnyButton() || GameDevice::input.CheckKey(Keys::Space)) {
		isLoading = false;
		auto manager = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock();
		manager->RemoveScene(Result::GetInstance()->GetReplayScene());


		auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("se_Decision.wav", "Sound/");

		gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.05);

		auto wkp_cam = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("camera");
		auto anim = wkp_cam.lock()->AddGameComponent<TransformAnimation>();
		auto targetTransform = wkp_cam.lock()->transform->Clone();
		targetTransform->RollLocalRotationX_Degrees(-90);
		targetTransform->TranslateY(10);
		anim->SetTargetTransform(targetTransform);
		anim->SetSpeed(0.05);
		SetIsRemove(true);

		if (selectButton == 0) {

			manager->LoadScene(Result::GetInstance()->GetReplayScene(), ObjectFactory::Create<Scene>(manager, SceneInformation(Result::GetInstance()->GetReplayScene())));

			gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->ChangeScene(Result::GetInstance()->GetReplayScene(), changeInterval);


		}
		else if (selectButton == 1) {
			manager->RemoveScene("LevelSelectScene");
			manager->LoadScene("LevelSelectScene", ObjectFactory::Create<Scene>(manager, SceneInformation("LevelSelectScene")));

			gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->ChangeScene("LevelSelectScene", changeInterval);

		}


		auto uibase = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("UIBase");
		if (uibase.lock()) {
			auto uianim = uibase.lock()->AddGameComponent<TransformAnimation>();
			float y = gameObject.lock()->GetApplication().lock()->GetWindow()->GetSize().y;
			auto uiTarget = ObjectFactory::Create<Transform>(Vector3(0, -(y), 0));
			uianim->SetSpeed(0.05);
			uianim->SetTargetTransform(uiTarget);
		}
	}
}

void ButiEngine::FailedSceneController::OnShowUI()
{

	ImGui::BulletText("SceneChangeFrame");
	ImGui::DragInt("##SceneChangeFrame", &changeInterval, 1, 0, 600);
}

void ButiEngine::FailedSceneController::ButtonSelect()
{
	auto ui_retryButton = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("UI_Retry");
	auto ui_selectButton = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("UI_Select");

	if (selectButton == 0) {

		auto anim = ui_selectButton.lock()->GetGameComponent<TransformAnimation>();

		if (!anim) {
			anim = ui_selectButton.lock()->AddGameComponent<TransformAnimation>();
		}

		auto targetTransform = ui_selectButton.lock()->transform->Clone();
		targetTransform->SetLocalScale(Vector3(268, 38.4, 1.0));

		anim->SetTargetTransform(targetTransform);
		anim->SetSpeed(0.08);


		anim = ui_retryButton.lock()->GetGameComponent<TransformAnimation>();

		if (!anim) {
			anim = ui_retryButton.lock()->AddGameComponent<TransformAnimation>();
		}
		anim->GetInitTransform()->SetLocalScale(Vector3(268, 38.4, 1.0));
		targetTransform = ui_retryButton.lock()->transform->Clone();
		targetTransform->SetLocalScale(Vector3(402, 57.6, 1.0));

		anim->SetTargetTransform(targetTransform);
		anim->SetSpeed(0.08);
		anim->SetReverse(true);

	}
	else if (selectButton == 1) {

		auto anim = ui_retryButton.lock()->GetGameComponent<TransformAnimation>();

		if (!anim) {
			anim = ui_retryButton.lock()->AddGameComponent<TransformAnimation>();
		}

		auto targetTransform = ui_retryButton.lock()->transform->Clone();
		targetTransform->SetLocalScale(Vector3(268, 38.4, 1.0));

		anim->SetTargetTransform(targetTransform);
		anim->SetSpeed(0.08);


		anim = ui_selectButton.lock()->GetGameComponent<TransformAnimation>();

		if (!anim) {
			anim = ui_selectButton.lock()->AddGameComponent<TransformAnimation>();
		}
		anim->GetInitTransform()->SetLocalScale(Vector3(268, 38.4, 1.0));
		targetTransform = ui_selectButton.lock()->transform->Clone();
		targetTransform->SetLocalScale(Vector3(402, 57.6, 1.0));

		anim->SetTargetTransform(targetTransform);
		anim->SetSpeed(0.08);
		anim->SetReverse(true);


	}
}
