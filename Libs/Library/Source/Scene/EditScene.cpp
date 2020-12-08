#pragma once
#include"stdafx.h"
#include"Header/GameParts/SoundManager.h"
#include"Header/GameParts/Renderer.h"


#include"Header/Resources/ModelAnimation.h"
#include"Header/Scene/ComponentsLoader.h"
#include"Header/Scene/EditScene.h"



void ButiEngine::EditScene::Update() {

	shp_gameObjectManager->RegistNewGameObject();
	if (isActive) {

		shp_gameObjectManager->Update();
		if (shp_collisionManager)
			shp_collisionManager->Update();

		OnUpdate();
		shp_soundManager->Update();
	}

	UIUpdate();
}

void ButiEngine::EditScene::Set()
{
	shp_gameObjectManager->Start();
}

void ButiEngine::EditScene::OnSet()
{
}

void ButiEngine::EditScene::OnInitialize()
{
	
}

void ButiEngine::EditScene::OnUpdate()
{

}

void ButiEngine::EditScene::UIUpdate()
{
	GUI::Begin("top");
	if (GUI::ArrowButton("##play", GUI::GuiDir_Right)) {
		isActive =!isActive;

		GetCamera("edit").lock()->SetActive(!GetCamera("edit").lock()->GetActive());
		GetCamera("main").lock()->SetActive(!GetCamera("main").lock()->GetActive());
		isPlaying = true;
		if (isActive) {
			startCount++;
			if (startCount==1) {
				OutputCereal(shp_gameObjectManager,GlobalSettings::GetResourceDirectory()+ "Scene/" + sceneInformation->GetSceneName()+"/objects.gameObjectManager" );
				OutputCereal(shp_renderingInfo, GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation->GetSceneName() + "/rendering.renderingInfo");

				shp_gameObjectManager->Start();
			}
		}

	};
	static char sceneName[128] = {};
	GUI::SameLine();
	(GUI::InputTextWithHint("##SceneName",sceneInformation->GetSceneName(), sceneName, sizeof(sceneName)));
	GUI::SameLine();
	if (GUI::Button("Change")) {
		GetSceneManager().lock()->RemoveScene(sceneName);
		GetSceneManager().lock()->LoadScene_EditMode(sceneName);
		GetSceneManager().lock()->ChangeScene(sceneName);
		memset(sceneName, 0, sizeof(sceneName));
	}
	GUI::SameLine();
	if (GUI::Button("Reload")) {
		GetSceneManager().lock()->ReloadScene();
	}

	if (!isPlaying) {

		GUI::SameLine();
		if (GUI::Button("Save!!!!")|| ((GameDevice::GetInput()->CheckKey(Keys::LeftCtrl) || GameDevice::GetInput()->CheckKey(Keys::RightCtrl)) && GameDevice::GetInput()->CheckKey(Keys::S))) {

			OutputCereal(shp_gameObjectManager, GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation->GetSceneName() + "/objects.gameObjectManager");
			OutputCereal(shp_renderingInfo, GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation->GetSceneName() + "/rendering.renderingInfo");
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


	GUI::End();


	/*GUI::Begin("Bara Bara Slider");
	GUI::SliderFloat("pushPower", &t, 0, 1.0f);
	GUI::End();*/
	if(showHeirarcy)
	shp_gameObjectManager->ShowUI();



	if (showContainer) {
		GetResourceContainer()->ShowGUI();

	}
	if (showInspector) {

		auto selectedGameObject = shp_gameObjectManager->GetSelectedUI();
		GUI::Begin("Inspector");
		if (selectedGameObject.lock()) {


			if ((GameDevice::GetInput()->CheckKey(Keys::LeftCtrl) || GameDevice::GetInput()->CheckKey(Keys::RightCtrl)) && GameDevice::GetInput()->CheckKey(Keys::F)) {
				GetCamera("edit").lock()->shp_transform->SetLookAtRotation(selectedGameObject.lock()->transform->GetWorldPosition());
			}

			GUI::InputTextWithHint("Name", selectedGameObject.lock()->GetGameObjectName().c_str(), GUI::objectName, 64, 64);
			GUI::SameLine();

			if (GUI::Button("Change")) {
				selectedGameObject.lock()->SetObjectName(GUI::objectName);
				GUI::ObjectNameReset();

			}


			selectedGameObject.lock()->ShowUI();

			auto addComponent = ComponentsLoader::GetInstance()-> ShowAddGameComponentUI();

			if (addComponent)
				selectedGameObject.lock()->AddGameComponent_Insert(addComponent);

			auto addBehavior = ComponentsLoader::GetInstance()->ShowAddBehaviorUI();
			if (addBehavior)
				selectedGameObject.lock()->AddBehavior_Insert(addBehavior);


			if (GUI::Button("Save!", Vector2(200, 30))) {
				OutputCereal(selectedGameObject.lock());
			}
		}



		GUI::End();
	}

	if(!isActive&&!GUI::IsWindowHovered(GUI::GuiHoveredFlags_AnyWindow))
	EditCamera();
}

void ButiEngine::EditScene::EditCamera()
{
	auto editCam = GetCamera("edit").lock();
	if (GameDevice::GetInput()->GetMouseButton(MouseButtons::RightClick)) {
		Vector2 move = GameDevice::GetInput()->GetMouseMove();
		editCam->shp_transform->RollWorldRotationY_Degrees(-move.x);
		editCam->shp_transform->RollLocalRotationX_Degrees(-move.y);
	}
	if (GameDevice::GetInput()->GetMouseButton(MouseButtons::WheelButton)) {
		Vector2 move = GameDevice::GetInput()->GetMouseMove();

		Vector3 velocity = editCam->shp_transform->GetRight() * move.x * 0.01f + editCam->shp_transform->GetUp() * move.y*-1 * 0.01f;

		editCam->shp_transform->Translate (velocity);
	}
	if (GameDevice::GetInput()->GetMouseWheel()) {
		Vector3 velocity = editCam->shp_transform->GetFront() * 0.001f * GameDevice::GetInput()->GetMouseWheelMove();

		editCam->shp_transform->Translate(velocity);
	}
}

void ButiEngine::EditScene::Draw()
{

	shp_renderer->RenderingStart();


	shp_renderer->BefRendering();

	std::vector<std::shared_ptr<ICamera>> cams;
	for (auto cameraItr = vec_cameras.begin(); cameraItr != vec_cameras.end(); cameraItr++) {
		if (!(*cameraItr)->GetActive()) {
			continue;
		}

		cams.push_back(*cameraItr);
	}

	for (auto cameraItr = cams.begin(); cameraItr != cams.end(); cameraItr++) {

		(*cameraItr)->Start();

		(*cameraItr)->Draw();
		
		if(cameraItr+1== cams.end())
		shp_sceneManager->GetApplication().lock()->GetGUIController()->Draw();

		(*cameraItr)->Stop();
	}



	shp_renderer->RenderingEnd();
}


ButiEngine::EditScene::EditScene(std::weak_ptr<ISceneManager> arg_wkp_sceneManager, std::shared_ptr< SceneInformation> arg_SceneInformation)
	:sceneInformation ( arg_SceneInformation)
{
	
	shp_sceneManager = arg_wkp_sceneManager.lock();
}

void ButiEngine::EditScene::Release()
{
	
	shp_gameObjectManager->Release();
	shp_soundManager->Release();
	shp_renderer->ReleaseFogBuffer();
	shp_renderer->Release();
	(shp_collisionManager) = nullptr;
	shp_sceneManager = nullptr;
}


void ButiEngine::EditScene::Initialize()
{


	shp_renderer = ObjectFactory::Create<Renderer>(GetThis<IScene>());
	{

		std::string fullRenderingInfoPath = GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation->GetSceneName() + "/rendering.renderingInfo";
		if (Util::IsFileExistence(fullRenderingInfoPath)) {
			shp_renderingInfo = std::make_shared<SceneRenderingInformation>(); 
			InputCereal (shp_renderingInfo, fullRenderingInfoPath);
		}
		else {

			shp_renderingInfo = std::make_shared<SceneRenderingInformation>();

			auto windowSize = GetWindow()->GetSize();
			auto prop = CameraProjProperty(windowSize.x, windowSize.y, 0, 0);
			prop.farClip = 200.0f;
			prop.cameraName = "main";
			shp_renderingInfo->vec_cameraProperty.push_back(prop);
			prop.cameraName = "edit";
			shp_renderingInfo->vec_cameraProperty.push_back(prop);


			shp_renderingInfo->vec_cameraTransform.push_back(ObjectFactory::Create<Transform>());
			auto editTransform = ObjectFactory::Create<Transform>(Vector3(5,5,-5));editTransform-> SetLookAtRotation(Vector3());
			shp_renderingInfo->vec_cameraTransform.push_back(editTransform);

			_mkdir((GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation->GetSceneName() + "/").c_str());
		}
	}
	for (int i = 0; i < shp_renderingInfo->layerCount; i++) {

		shp_renderer->AddLayer();
	}
	shp_soundManager = ObjectFactory::Create<SoundManager>(GetThis<IScene>());

	ActiveCollision(1);

	{

		std::string fullGameObjectManagerPath = GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation->GetSceneName() + "/objects.gameObjectManager";
		if (Util::IsFileExistence(fullGameObjectManagerPath)) {
			shp_gameObjectManager = ObjectFactory::CreateFromCereal<GameObjectManager>(fullGameObjectManagerPath);

			shp_gameObjectManager->SetScene(GetThis<IScene>());
			shp_gameObjectManager->Initialize_cereal();
		}
		else {
			auto defaultPath = GlobalSettings::GetResourceDirectory() + "Scene/" + "DefaultScene" + "/objects.gameObjectManager";
			shp_gameObjectManager = ObjectFactory::CreateFromCereal<GameObjectManager>(defaultPath);

			shp_gameObjectManager->SetScene(GetThis<IScene>());
			shp_gameObjectManager->Initialize_cereal();

			_mkdir((GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation->GetSceneName() + "/").c_str());
		}
	} 

	bool isContainEditCam = false;
	for (int i = 0; i < shp_renderingInfo->vec_cameraProperty.size(); i++) {
		auto prop = shp_renderingInfo->vec_cameraProperty[i];
		AddCamera(prop, prop.cameraName, true).lock()->shp_transform=shp_renderingInfo->vec_cameraTransform[i];
		if (prop.cameraName == "edit") {
			isContainEditCam = true;
		}
	}
	if (!isContainEditCam) {
		auto windowSize = GetWindow()->GetSize();
		auto prop = CameraProjProperty(windowSize.x, windowSize.y, 0, 0);
		prop.cameraName = "edit";
		AddCamera(prop, "edit", true);
		shp_renderingInfo->vec_cameraProperty.push_back(prop);
		shp_renderingInfo->vec_cameraTransform.push_back(ObjectFactory::Create<Transform>());
	}
	GetCamera("edit").lock()->SetActive(true);
	GetCamera("main").lock()->SetActive(false);

	//auto prop3 = CameraProjProperty(windowSize.x, windowSize.y, 0, 0, true, 2);
	//AddCamera(prop3, "backGround", true);
	OnInitialize();

}

void ButiEngine::EditScene::ActiveCollision(const UINT arg_layerCount)
{
	if (!shp_collisionManager) {
		shp_collisionManager = ObjectFactory::Create<Collision::CollisionManager>(arg_layerCount);
	}
	else if(arg_layerCount != shp_collisionManager->GetLayerCount())
	shp_collisionManager = ObjectFactory::Create<Collision::CollisionManager>(arg_layerCount);
}

void ButiEngine::EditScene::PreInitialize()
{
}

std::unique_ptr<ButiEngine::IWindow>& ButiEngine::EditScene::GetWindow()
{
	return shp_sceneManager->GetApplication().lock()->GetWindow();
}

std::weak_ptr<ButiEngine::ICamera> ButiEngine::EditScene::GetCamera(const std::string& arg_camName)
{

	for (auto itr = vec_cameras.begin(); itr != vec_cameras.end(); itr++) {
		if ((*itr)->GetName() == arg_camName) {
			return (*itr);
		}
	}
	std::cout << "this camera name is wrong." << std::endl;
	return vec_cameras.at(0);
}

std::weak_ptr<ButiEngine::ICamera> ButiEngine::EditScene::GetCamera(const UINT arg_camNum)
{
	return vec_cameras.at(arg_camNum);
}

std::weak_ptr<ButiEngine::ICamera> ButiEngine::EditScene::AddCamera(const CameraProjProperty& arg_prop, const std::string arg_cameraName, const bool arg_initActive)
{


	auto out = CameraCreater::CreateCamera(arg_prop, arg_cameraName, arg_initActive, shp_renderer, shp_sceneManager->GetApplication().lock()->GetGraphicDevice());
	vec_cameras.push_back(out);
	return out;
}

void ButiEngine::EditScene::RemoveCamera(const std::string& arg_camName)
{
	auto itr = vec_cameras.begin();
	while (itr != vec_cameras.end()) {
		if ((*itr)->GetName() == arg_camName) {
			itr = vec_cameras.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void ButiEngine::EditScene::RemoveCamera(const UINT arg_camNum)
{
	auto itr = vec_cameras.begin();
	for (int i = 0; i < arg_camNum; i++) {
		itr++;
	}


	vec_cameras.erase(itr);
}

void ButiEngine::EditScene::SceneEnd()
{
	OnSceneEnd();
}

std::shared_ptr<ButiEngine::IResourceContainer> ButiEngine::EditScene::GetResourceContainer()
{
	return shp_sceneManager->GetApplication().lock()->GetResourceContainer();
}

std::shared_ptr<ButiEngine::IRenderer> ButiEngine::EditScene::GetRenderer()
{
	return shp_renderer;
}

std::shared_ptr<ButiEngine::ISoundManager> ButiEngine::EditScene::GetSoundManager()
{
	return shp_soundManager;
}

std::weak_ptr< ButiEngine::ISceneManager> ButiEngine::EditScene::GetSceneManager()
{
	return shp_sceneManager;
}

std::weak_ptr<ButiEngine::Collision::CollisionManager> ButiEngine::EditScene::GetCollisionManager()
{
	return shp_collisionManager;
}

std::shared_ptr< ButiEngine::SceneInformation > ButiEngine::EditScene::GetSceneInformation()
{
	return sceneInformation;
}

std::shared_ptr<ButiEngine::SceneChangeInformation> ButiEngine::EditScene::GetSceneChangeInformation()
{
	return shp_changeInfo;
}

std::shared_ptr<ButiEngine::SceneRenderingInformation> ButiEngine::EditScene::GetSceneRenderingInformation()
{
	return shp_renderingInfo;
	
}

