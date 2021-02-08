#include"stdafx.h"
#include "..\..\Header\Scene\SceneManager_Edit.h"
#include"..\..\Header\Scene/Scene.h"

ButiEngine::SceneManager_Edit::SceneManager_Edit(std::weak_ptr<IApplication> arg_wkp_app)
	:SceneManager(arg_wkp_app)
{
}

void ButiEngine::SceneManager_Edit::Initialize()
{
	currentScene->CameraActivation(false);
}

void SceneUpdate_ed(std::shared_ptr<ButiEngine::IScene> currentScene) {
	currentScene->Update();
}

void SceneDraw_ed(std::shared_ptr<ButiEngine::IScene> currentScene) {
	currentScene->Draw();
}

void ButiEngine::SceneManager_Edit::Update()
{

	if (isReload) {

		{
			RemoveScene(currentScene->GetSceneInformation()->GetSceneName());
			LoadScene(currentScene->GetSceneInformation()->GetSceneName());

			newScene = map_iscene.at(currentScene->GetSceneInformation()->GetSceneName());

			if (currentScene != nullptr)
				currentScene->SceneEnd();
			currentScene = newScene;
			newScene = nullptr;
			isPlaying = false;
			isActive = false;
			startCount = 0;
		}
		isReload = false;
	}


	if (sceneChangeTimer->Update())
	{
		RenewalScene();
		sceneChangeTimer->Stop();
	}

	GUI::SetState( GUI::GUIState::noActive);
	UIUpdate();


	if (isActive) {

		currentScene->UIUpdate();

		GUI::SetState(GUI::GUIState::enable);
		currentScene->BefDraw();

		auto updateThread = std::thread(SceneUpdate_ed,currentScene);

		currentScene->Draw();
		updateThread.join();
	}
	else {
		GUI::SetState(GUI::GUIState::enable);

		currentScene->RegistGameObjects();
		currentScene->BefDraw();
		currentScene->Draw();
		if ( !GUI::IsWindowHovered(GUI::GuiHoveredFlags_AnyWindow)) {
			currentScene->EditCameraUpdate();
		}
	}

}

void ButiEngine::SceneManager_Edit::UIUpdate()
{
	GUI::Begin("top");
	if (GUI::ArrowButton("##play", GUI::GuiDir_Right) || (GameDevice::GetInput()->TriggerKey(Keys::F5) && GameDevice::GetInput()->CheckKey(Keys::LeftShift))) {
		isActive = !isActive;

		isPlaying = true;
		if (isActive) {
			currentScene->CameraActivation(true);

			startCount++;
			if (startCount == 1) {
				currentScene->Save();
				currentScene->Start();
			}
		}
		else {

			currentScene->CameraActivation(false);
		}

	};
	static char sceneName[128] = {};
	GUI::SameLine();
	(GUI::InputTextWithHint("##SceneName", currentScene->GetSceneInformation()->GetSceneName(), sceneName, sizeof(sceneName)));
	GUI::SameLine();
	if (GUI::Button("Change")) {
		RemoveScene(sceneName);
		LoadScene(sceneName);
		ChangeScene(sceneName);
		memset(sceneName, 0, sizeof(sceneName));
	}
	GUI::SameLine();
	if (GUI::Button("Reload")) {
		ReloadScene();
		isPlaying = false;
	}

	if (!isPlaying) {

		GUI::SameLine();
		if (GUI::Button("Save!!!!") || ((GameDevice::GetInput()->CheckKey(Keys::LeftCtrl) || GameDevice::GetInput()->CheckKey(Keys::RightCtrl)) && GameDevice::GetInput()->CheckKey(Keys::S))) {

			currentScene->Save();

		};
	}

	if (isActive) {
		GUI::SameLine();
		GUI::BulletText("Update...");
	}

	GUI::Checkbox("ShowHierarchy", &showHeirarcy);

	GUI::SameLine();
	GUI::Checkbox("ShowInspector", &showInspector);

	GUI::SameLine();
	GUI::Checkbox("ShowContainer", &showContainer);
	GUI::SameLine();
	GUI::Checkbox("ShowCameraEditor", &showCamera);
	GUI::SameLine();
	GUI::Checkbox("ShowCollisionManager", &showCollisionManager);


	GUI::End();


	if (showHeirarcy) {

		currentScene->ShowHeirarcyUI();
	}
		



	if (showContainer) {
		currentScene-> GetResourceContainer()->ShowGUI();

	}
	if (showInspector) {
		currentScene->ShowInspectorUI();
	}

	if (showCamera) {
		currentScene->ShowRenderingUI();
	}

	if (showCollisionManager) {
		currentScene->GetCollisionManager().lock()->ShowGUI();
	}


}

void ButiEngine::SceneManager_Edit::ChangeScene(const std::string& arg_sceneName, float sceneChangeDalay)
{
	if (!map_iscene.count(arg_sceneName)) {
		return;
	}
	newScene = map_iscene.at(arg_sceneName);
	sceneChangeTimer = ObjectFactory::Create<AbsoluteTimer>(sceneChangeDalay);
	sceneChangeTimer->Start();
}

void ButiEngine::SceneManager_Edit::LoadScene(const std::string& arg_sceneName, std::shared_ptr<SceneInformation> shp_scene)
{
	SceneManager::LoadScene(arg_sceneName, shp_scene);

}

void ButiEngine::SceneManager_Edit::LoadScene_Init(const std::string& arg_sceneName, std::shared_ptr<SceneInformation> shp_sceneInfo)
{
	if (!map_iscene.count(arg_sceneName)) {
		if (!shp_sceneInfo) {
			shp_sceneInfo = ObjectFactory::Create<SceneInformation>(arg_sceneName);
		}

		SetScene_Init(arg_sceneName, ObjectFactory::Create<Scene>(GetThis<ISceneManager>(), shp_sceneInfo));
		currentScene->Set();
	}
}

