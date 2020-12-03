#include "stdafx.h"
#include "SceneChanger.h"
#include"Header/GameObjects/DefaultGameComponent/TransformAnimation.h"
#include"Header/GameObjects/DefaultGameComponent/LookAtComponent.h"
#include"Header/GameParts/SoundManager.h"
#include"Header/GameParts/ResourceContainer.h"

void ButiEngine::SceneChanger::Start()
{
	auto bgmTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("bgm.wav", "Sound/");

	gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->PlayBGM(bgmTag,0.025);

}

void ButiEngine::SceneChanger::OnUpdate()
{

	if (GameDevice::GetInput()->GetAnyButton() || GameDevice::GetInput()->CheckKey(Keys::Space)) {


		auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("se_Decision.wav", "Sound/");

		gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.05);

		auto manager = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock();
		manager->RemoveScene(removeScene);
		manager->LoadScene(loadScene);

		auto wkp_cam = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("camera");
		auto look= wkp_cam.lock()->GetGameComponent<LookAtComponent>();
		if (look) {
			look->Detach();
		}
		auto anim = wkp_cam.lock()->AddGameComponent<TransformAnimation>();
		auto targetTransform = wkp_cam.lock()->transform->Clone();
		targetTransform->RollLocalRotationX_Degrees(-90);
		targetTransform->TranslateY(10);
		anim->SetTargetTransform(targetTransform);
		anim->SetSpeed(0.05);
		SetIsRemove(true);
		gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->ChangeScene(nextScene,changeInterval);



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

void ButiEngine::SceneChanger::OnShowUI()
{
	GUI::BulletText("SceneChangeFrame");
	GUI::DragInt("##SceneChangeFrame", &changeInterval, 1, 0, 600);
	if (GUI::TreeNode("RemoveScene")) {

		GUI::InputTextWithHint("##RemoveName", removeScene.c_str(), GUI::objectName, 64, 64);
		GUI::SameLine();

		if (GUI::Button("Change")) {
			removeScene = GUI::objectName;
			GUI::ObjectNameReset();

		}
		GUI::TreePop();
	}
	if (GUI::TreeNode("ChangeScene")) {

		GUI::InputTextWithHint("##ChangeName", nextScene.c_str(), GUI::objectName, 64, 64);
		GUI::SameLine();

		if (GUI::Button("Change")) {
			nextScene = GUI::objectName;
			GUI::ObjectNameReset();

		}
		GUI::TreePop();
	}
	if (GUI::TreeNode("LoadScene")) {

		GUI::InputTextWithHint("##LoadName", loadScene.c_str(), GUI::objectName, 64, 64);
		GUI::SameLine();

		if (GUI::Button("Change")) {
			loadScene = GUI::objectName;
			GUI::ObjectNameReset();

		}
		GUI::TreePop();
	}
}
