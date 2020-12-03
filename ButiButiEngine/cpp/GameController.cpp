#include "stdafx.h"
#include "..\include\GameController.h"
#include"EnemySpawner.h"
#include"ScoreUI.h"
#include"../include/PlayerBehavior.h"
#include"CloudMover.h"
#include"BuldingMover.h"
#include"Result.h"
#include"Header/GameObjects/DefaultGameComponent/TransformAnimation.h"
#include"Header/GameObjects/DefaultGameComponent/LookAtComponent.h"
#include"Header/GameObjects/DefaultGameComponent/SplineCurveMover.h"
#include"PlayerLookingCamera.h"
#include"Header/GameParts/ResourceContainer.h"

void ButiEngine::GameController::Start()
{
	auto manager = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock();
	manager->RemoveScene(befStage);
	shp_scoreUIObject = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("ScoreUI").lock();
	shp_scoreUI = shp_scoreUIObject->GetGameComponent<ScoreUI>();

	shp_remainUIObject = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("UI_Remain").lock();
	shp_remainUI = shp_remainUIObject->GetGameComponent<RemainUI>();

	shp_combotimer = ObjectFactory::Create<RelativeTimer>(30);
	CloudMover::startZ = 130;
	BuildingMover::startZ = 230;
}

void ButiEngine::GameController::OnUpdate()
{
	if (!init) {
		init = true;
		shp_remainUI->SetScore(targetScore);
	}
	if (shp_timer->Update()) {
		auto manager = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock();
		manager->RemoveScene(nextStage);
		manager->LoadScene(nextStage,ObjectFactory::Create< SceneInformation>(nextStage));
		manager->ChangeScene(nextStage,60);
		SetIsActive(false);
		auto wkp_cam = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("camera");
		wkp_cam.lock()->GetGameComponent<PlayerLookingCamera>()->SetIsActive(false);
		auto transformanim = wkp_cam.lock()->GetGameComponent<TransformAnimation>();
		if (transformanim) {
			return;
		}
		auto anim = wkp_cam.lock()->AddGameComponent<TransformAnimation>();
		auto targetTransform = wkp_cam.lock()->transform->Clone();
		targetTransform->RollLocalRotationX_Degrees(-90);
		targetTransform->TranslateY(10);
		anim->SetTargetTransform(targetTransform);
		anim->SetSpeed(0.07);

		auto uibase = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("UIBase");
		if (uibase.lock()) {
			auto uianim = uibase.lock()->AddGameComponent<TransformAnimation>();
			float y = gameObject.lock()->GetApplication().lock()->GetWindow()->GetSize().y;
			auto uiTarget = ObjectFactory::Create<Transform>(Vector3(0, -(y), 0));
			uianim->SetSpeed(0.05);
			uianim->SetTargetTransform(uiTarget);
		}

	}
	if (shp_combotimer->Update()) {
		combo = 0;
		shp_combotimer->Stop();
	}
}

void ButiEngine::GameController::OnSet()
{
	if (!shp_timer)
		shp_timer = ObjectFactory::Create<RelativeTimer>(60);
}

int ButiEngine::GameController::AddScore(int arg_score)
{
	combo++;


	auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("se_Bomb.wav", "Sound/");

	gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.025);

	nowScore += arg_score*combo;

	shp_combotimer->Start();
	shp_combotimer->SetCount(0);

	if (nowScore >= Result::GetInstance()->GetMaxScore(gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName())) {
		shp_scoreUI->SetColor(Vector4(0.92, 0.84, 0.06, 1.0));
	}
	shp_scoreUI->SetScore(nowScore);
	auto scoreAnim= shp_scoreUIObject->GetGameComponent<TransformAnimation>();
	if (!scoreAnim) {
		scoreAnim = shp_scoreUIObject->AddGameComponent<TransformAnimation>();
		scoreAnim->SetSpeed(0.07);
		scoreAnim->SetEaseType(Easing::EasingType::Parabola);

		auto target= shp_scoreUIObject->transform->Clone();

		scoreAnim->SetTargetTransform(target);
	}

	scoreAnim->GetTargetTransform()->SetLocalScale() *= 1.1;
	scoreAnim->SetTime(0);
	
	enemyCount++;


	shp_remainUI->SetScore(max (0,targetScore-enemyCount));


	auto remainAnim = shp_remainUIObject->GetGameComponent<TransformAnimation>();
	if (!remainAnim) {
		remainAnim = shp_remainUIObject->AddGameComponent<TransformAnimation>();
		remainAnim->SetSpeed(0.07);
		remainAnim->SetEaseType(Easing::EasingType::Parabola);

		auto target = shp_remainUIObject->transform->Clone();

		remainAnim->SetTargetTransform(target);
	}

	remainAnim->GetTargetTransform()->SetLocalScale() *= 1.1;
	remainAnim->SetTime(0);


	if (enemyCount >= targetScore&&!isSpawnBoss) {

		if (gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("Boss_" + gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName()).lock()) {
			return nowScore;
		}
		isSpawnBoss = true;
		auto spawner = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("EnemySpawner").lock()->GetGameComponent<EnemySpawner>();

		spawner->SpawnBoss();

	}

	return nowScore;
	
}

void ButiEngine::GameController::DestroyBoss(int arg_score)
{
	combo++;



	nowScore += arg_score * combo;

	shp_combotimer->Start();
	shp_combotimer->SetCount(0);

	shp_scoreUI->SetScore(nowScore); 

	if (nowScore >= Result::GetInstance()->GetMaxScore(gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName())) {
		shp_scoreUI->SetColor(Vector4(0.92, 0.84, 0.06, 1.0));
	}

	auto scoreAnim = shp_scoreUIObject->GetGameComponent<TransformAnimation>();
	if (!scoreAnim) {
		scoreAnim = shp_scoreUIObject->AddGameComponent<TransformAnimation>();
		scoreAnim->SetSpeed(0.05);
		scoreAnim->SetEaseType(Easing::EasingType::Parabola);

		auto target = shp_scoreUIObject->transform->Clone();

		scoreAnim->SetTargetTransform(target);
	}

	scoreAnim->GetTargetTransform()->SetLocalScale() *= 1.1;
	scoreAnim->SetTime(0);

	if (enemyCount >= targetScore) {


		auto spawner = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("EnemySpawner").lock()->GetGameComponent<EnemySpawner>();

		spawner->SetIsActive(false);

		nextStage = "ClearScene";
		shp_timer->Start();
		auto sceneName = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName();
		Result::GetInstance()->SetScore(sceneName, nowScore);
		Result::GetInstance()->SetReplayScene(sceneName);
		auto cam = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("camera").lock();
		auto playerLooker =cam-> GetGameComponent<PlayerLookingCamera>();

		playerLooker->SetPer(1.0);
		


		auto playerGameObj = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("Player").lock();


		auto player = playerGameObj->GetBehavior<PlayerBehavior>();
		if(player)
		player->SetIsInvisible(true);


		auto splineMover = cam->AddGameComponent<SplineCurveMover>();
		splineMover->SetSpeed(0.005);

		SplineCurve curve;
		if (cam->transform->GetWorldPosition().z > playerGameObj->transform->GetWorldPosition().z)

			curve = SplineCurve(cam->transform->GetWorldPosition(), playerGameObj->transform->GetWorldPosition() + Vector3(-3, 0, -5), { playerGameObj->transform->GetWorldPosition() + Vector3(0,8,8),playerGameObj->transform->GetWorldPosition() + Vector3(-5,5,5),playerGameObj->transform->GetWorldPosition() + Vector3(-8,3,3) });
		else {
			curve = SplineCurve(cam->transform->GetWorldPosition(), playerGameObj->transform->GetWorldPosition() + Vector3(3, 1, 5), { playerGameObj->transform->GetWorldPosition() + Vector3(0,8,-8),playerGameObj->transform->GetWorldPosition() + Vector3(5,5,-5),playerGameObj->transform->GetWorldPosition() + Vector3(8,3,-3) });

		}
		splineMover->SetSplineCurve(curve);


		GameDevice::WorldSpeed = 0.125f;
		
	}

	return ;
}

void ButiEngine::GameController::Failed()
{
	//gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->ChangeScene("FailedScene",60);

	auto manager = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock();
	nextStage = "FailedScene";
	shp_timer->Start();
	auto playerGameObj = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("Player").lock();
	auto player = playerGameObj->GetBehavior<PlayerBehavior>();
	player->SetIsRemove(true);
	auto sceneName = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName();
	Result::GetInstance()->SetCurrentScore(sceneName, nowScore);
	Result::GetInstance()->SetReplayScene(sceneName);


	auto cam = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("camera").lock();

	auto splineMover= cam->AddGameComponent<SplineCurveMover>();
	splineMover->SetSpeed(0.005);

	SplineCurve curve;
	if (cam->transform->GetWorldPosition().z > playerGameObj->transform->GetWorldPosition().z)

		curve = SplineCurve(cam->transform->GetWorldPosition(), playerGameObj->transform->GetWorldPosition() + Vector3(-3, 0, -5), { playerGameObj->transform->GetWorldPosition() + Vector3(0,8,8),playerGameObj->transform->GetWorldPosition() + Vector3(-5,5,5),playerGameObj->transform->GetWorldPosition() + Vector3(-8,3,3) });
	else {
		curve = SplineCurve(cam->transform->GetWorldPosition(), playerGameObj->transform->GetWorldPosition() + Vector3(3, 1, 5), { playerGameObj->transform->GetWorldPosition() + Vector3(0,8,-8),playerGameObj->transform->GetWorldPosition() + Vector3(5,5,-5),playerGameObj->transform->GetWorldPosition() + Vector3(8,3,-3) });

	}
	splineMover->SetSplineCurve(curve);

	auto playerLooker = cam->GetGameComponent<PlayerLookingCamera>();
	playerLooker->SetPer(1.0);

	GameDevice::WorldSpeed = 0.125f;
}

void ButiEngine::GameController::OnShowUI()
{
	GUI::BulletText("StageRange_Max");
	GUI::DragFloat3("##max", &stageMax.x, 0.1f, -80.0f, 80.0f);
	GUI::BulletText("StageRange_Min");
	GUI::DragFloat3("##min", &stageMin.x, 0.1f, -80.0f, 80.0f);
	GUI::BulletText("TargetScore");
	GUI::DragInt("##TargetScore", &targetScore, 1.0, 0, 10000000000);
	GUI::BulletText("NowScore");
	GUI::DragInt("##NowScore", &nowScore, 1.0, 0, 10000000000);
	GUI::BulletText("NextStageName");

	GUI::InputTextWithHint("##Name", nextStage.c_str(), GUI::objectName, 64, 64);
	GUI::SameLine();

	if (GUI::Button("Change")) {
		nextStage = GUI::objectName;
		GUI::ObjectNameReset();

	}
	GUI::BulletText("BefStageName");
	GUI::InputTextWithHint("##befName", befStage.c_str(), GUI::objectName, 64, 64);
	GUI::SameLine();

	if (GUI::Button("Change##befName")) {
		befStage = GUI::objectName;
		GUI::ObjectNameReset();

	}

	GUI::BulletText("Timer");
	shp_timer->ShowGUI();
}
