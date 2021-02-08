#pragma once
#include"stdafx.h"
#include "..\..\Header\Scene\Scene.h"
#include"Header/GameParts/SoundManager.h"
#include"Header/GameParts/Renderer.h"
#include"Header/GameParts/CollisionManager.h"
#include"Header/Scene/ComponentsLoader.h"

void ButiEngine::Scene::Update() {
	shp_gameObjectManager->RegistNewGameObject();
	shp_gameObjectManager->Update();
	if(shp_collisionManager)
	shp_collisionManager->Update();
	OnUpdate();
	shp_soundManager->Update();
}

void ButiEngine::Scene::UIUpdate()
{
	shp_sceneManager->GetApplication().lock()->GetGUIController()->Update();
}

void ButiEngine::Scene::EditCameraUpdate()
{
	auto editCam = GetCamera("edit").lock();
	if (GameDevice::GetInput()->GetMouseButton(MouseButtons::RightClick)) {
		Vector2 move = GameDevice::GetInput()->GetMouseMove();
		editCam->shp_transform->RollWorldRotationY_Degrees(-move.x);
		editCam->shp_transform->RollLocalRotationX_Degrees(-move.y);
	}
	if (GameDevice::GetInput()->GetMouseButton(MouseButtons::WheelButton)) {
		Vector2 move = GameDevice::GetInput()->GetMouseMove();

		Vector3 velocity = editCam->shp_transform->GetRight() * move.x * 0.01f + editCam->shp_transform->GetUp() * move.y * -1 * 0.01f;

		editCam->shp_transform->Translate(velocity);
	}
	if (GameDevice::GetInput()->GetMouseWheel()) {
		Vector3 velocity = editCam->shp_transform->GetFront() * 0.001f * GameDevice::GetInput()->GetMouseWheelMove();

		editCam->shp_transform->Translate(velocity);
	}
}

void ButiEngine::Scene::RegistGameObjects()
{

	shp_gameObjectManager->RegistNewGameObject();
}

void ButiEngine::Scene::Set()
{
}

void ButiEngine::Scene::BefDraw()
{
	shp_renderer->BefRendering();
	auto camEnd = vec_cameras.end();
	for (auto cameraItr = vec_cameras.begin(); cameraItr != camEnd; cameraItr++) {
		if (!(*cameraItr)->GetActive()) {
			continue;
		}
		(*cameraItr)->BefDraw();
	}
}

void ButiEngine::Scene::OnSet()
{
}

void ButiEngine::Scene::Draw()
{


	shp_renderer->RenderingStart();


	std::vector<std::shared_ptr<ICamera>> cams;
	auto camEnd = vec_cameras.end();
	for (auto cameraItr = vec_cameras.begin(); cameraItr != camEnd; cameraItr++) {
		if (!(*cameraItr)->GetActive()) {
			continue;
		}

		cams.push_back(*cameraItr);
	}

	for (auto cameraItr = cams.begin(); cameraItr != cams.end(); cameraItr++) {

		(*cameraItr)->Start();

		(*cameraItr)->Draw();

		if (cameraItr + 1 == cams.end())
			shp_sceneManager->GetApplication().lock()->GetGUIController()->Draw();

		(*cameraItr)->Stop();
	}



	shp_renderer->RenderingEnd();

	for (auto cameraItr = cams.begin(); cameraItr != cams.end(); cameraItr++) {

		(*cameraItr)->End();
	}
}

ButiEngine::Scene::Scene(std::weak_ptr<ISceneManager> arg_wkp_sceneManager, std::shared_ptr< SceneInformation> argSceneInformation):sceneInformation(argSceneInformation)
{
	shp_sceneManager = arg_wkp_sceneManager.lock();
}

void ButiEngine::Scene::Release()
{
	shp_gameObjectManager->Release();
	shp_soundManager->Release();
	shp_renderer->ReleaseFogBuffer();
	shp_renderer->Release();
	shp_sceneManager = nullptr;
}


void ButiEngine::Scene::Initialize()
{
	shp_renderer = ObjectFactory::Create<Renderer>(GetThis<IScene>());
	{

		std::string fullRenderingInfoPath = GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation->GetSceneName() + "/rendering.renderingInfo";
		if (Util::IsFileExistence(fullRenderingInfoPath)) {
			shp_renderingInfo = std::make_shared<SceneRenderingInformation>();
			InputCereal(shp_renderingInfo, fullRenderingInfoPath);
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
			auto editTransform = ObjectFactory::Create<Transform>(Vector3(5, 5, -5)); editTransform->SetLookAtRotation(Vector3());
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

	for (int i = 0; i < shp_renderingInfo->vec_cameraProperty.size(); i++) {
		auto prop = shp_renderingInfo->vec_cameraProperty[i];
		AddCamera(prop, prop.cameraName, true).lock()->shp_transform = shp_renderingInfo->vec_cameraTransform[i];
		
	}
	GetCamera("main").lock()->SetActive(true);
	auto editCam = GetCamera("edit");

	if (editCam.lock()) {
		editCam.lock()->SetActive(false);
	}

	//auto prop3 = CameraProjProperty(windowSize.x, windowSize.y, 0, 0, true, 2);
	//AddCamera(prop3, "backGround", true);
	OnInitialize();
}

void ButiEngine::Scene::ActiveCollision(const UINT arg_layerCount)
{
	if (!shp_collisionManager) {
		std::string path = GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation->GetSceneName() + "/collision.collisionManager";
		if (Util::IsFileExistence(path)) {
			auto temp = ObjectFactory::Create<Collision::CollisionManager>();
			InputCereal(temp, path);
			temp->ReCreateLayers();
			shp_collisionManager = temp;
		}
		else
		{
			shp_collisionManager = ObjectFactory::Create<Collision::CollisionManager>(arg_layerCount);
		}
	}
}

void ButiEngine::Scene::PreInitialize()
{
}

std::unique_ptr<ButiEngine::IWindow>& ButiEngine::Scene::GetWindow()
{
	return shp_sceneManager->GetApplication().lock()->GetWindow();
}

std::weak_ptr<ButiEngine::ICamera> ButiEngine::Scene::GetCamera(const std::string& arg_camName)
{


	for (auto itr = vec_cameras.begin(); itr != vec_cameras.end(); itr++) {
		if ((*itr)->GetName() == arg_camName) {
			return (*itr);
		}
	}
	std::cout << "this camera name is wrong." << std::endl;
	return vec_cameras.at(0);
}

std::weak_ptr<ButiEngine::ICamera> ButiEngine::Scene::GetCamera(const UINT arg_camNum)
{
	return vec_cameras.at(arg_camNum);
}

std::weak_ptr<ButiEngine::ICamera> ButiEngine::Scene::AddCamera(CameraProjProperty& arg_prop, const std::string& arg_cameraName, const bool arg_initActive)
{
	bool isContain = true;
	int count = 0;
	while (isContain) {
		for (auto itr = vec_cameras.begin(); itr != vec_cameras.end(); itr++) {
			if ((*itr)->GetName() == arg_cameraName || (*itr)->GetName() == arg_cameraName + "_" + std::to_string(count)) {
				count++;
				isContain = true;
				break;
			}
		}
		isContain = false;
	}
	if (count > 0) {
		arg_prop.cameraName += "_""_" + std::to_string(count);
	}
	auto out = CameraCreater::CreateCamera(arg_prop, arg_prop.cameraName, arg_initActive, shp_renderer, shp_sceneManager->GetApplication().lock()->GetGraphicDevice());
	vec_cameras.push_back(out);
	return out;
}

void ButiEngine::Scene::RemoveCamera(const std::string& arg_camName)
{
	auto itr = vec_cameras.begin();
	while ( itr != vec_cameras.end() ) {
		if ((*itr)->GetName() == arg_camName) {
			itr = vec_cameras.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void ButiEngine::Scene::RemoveCamera(const UINT arg_camNum)
{
	auto itr = vec_cameras.begin();
	for (int i = 0; i < arg_camNum; i++) {
		itr++;
	}
	

	vec_cameras.erase(itr);
}

void ButiEngine::Scene::SceneEnd()
{
	OnSceneEnd();
}

std::shared_ptr<ButiEngine::IResourceContainer> ButiEngine::Scene::GetResourceContainer()
{
	return shp_sceneManager->GetApplication().lock()->GetResourceContainer();
}

std::shared_ptr<ButiEngine::IRenderer> ButiEngine::Scene::GetRenderer()
{
	return shp_renderer;
}

std::shared_ptr<ButiEngine::ISoundManager> ButiEngine::Scene::GetSoundManager()
{
	return shp_soundManager;
}

std::weak_ptr< ButiEngine::ISceneManager> ButiEngine::Scene::GetSceneManager()
{
	return shp_sceneManager;
}

std::weak_ptr<ButiEngine::ICollisionManager> ButiEngine::Scene::GetCollisionManager()
{
	return shp_collisionManager;
}

std::shared_ptr< ButiEngine::SceneInformation >ButiEngine::Scene::GetSceneInformation()
{
	return sceneInformation;
}

std::shared_ptr<ButiEngine::SceneChangeInformation> ButiEngine::Scene::GetSceneChangeInformation()
{
	return shp_changeInfo;
}

std::shared_ptr<ButiEngine::SceneRenderingInformation> ButiEngine::Scene::GetSceneRenderingInformation()
{
	return shp_renderingInfo;
}

void ButiEngine::Scene::Save()
{
	OutputCereal(shp_gameObjectManager, GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation->GetSceneName() + "/objects.gameObjectManager");
	shp_renderingInfo->vec_cameraProperty.clear();
	shp_renderingInfo->vec_cameraTransform.clear();
	for (int i = 0; i < vec_cameras.size(); i++) {
		shp_renderingInfo->vec_cameraProperty.push_back(vec_cameras[i]->GetCameraProperty());
		shp_renderingInfo->vec_cameraTransform.push_back(vec_cameras[i]->shp_transform);
	}
	shp_renderingInfo->layerCount = shp_renderer->GetLayerCount();

	OutputCereal(shp_renderingInfo, GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation->GetSceneName() + "/rendering.renderingInfo");

	OutputCereal(shp_collisionManager->GetThis<Collision::CollisionManager>(), GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation->GetSceneName() + "/collision.collisionManager");
	
}

void ButiEngine::Scene::Start()
{
	shp_gameObjectManager->Start();
}

void ButiEngine::Scene::ShowGameObjectManagerUI()
{
	shp_gameObjectManager->ShowUI();
}

void ButiEngine::Scene::CameraActivation(const bool arg_status)
{
	for (auto camItr = vec_cameras.begin(); camItr != vec_cameras.end(); camItr++) {
		(*camItr)->SetActive(arg_status);
	}
	GetCamera("edit").lock()->SetActive(!arg_status);
}

void ButiEngine::Scene::ShowRenderingUI()
{

	(GUI::Begin("Camera"));

	if (GUI::Button("Add Layer")) {
		shp_renderer->AddLayer();
	}
	GUI::SameLine();
	GUI::Text(std::to_string(shp_renderer->GetLayerCount()));
	int i = 0;
	for (auto camItr = vec_cameras.begin(); camItr != vec_cameras.end(); ) {

		if (GUI::TreeNode(((*camItr)->GetName() + "##" + std::to_string(i)))) {
			static char inputCameraName[128] = {};
			GUI::InputTextWithHint("##cameraNameInput" + std::to_string(i), (*camItr)->GetName(), inputCameraName, sizeof(inputCameraName));
			GUI::SameLine();

			if (GUI::Button("Rename")) {
				(*camItr)->SetName(inputCameraName);
				memset(inputCameraName, 0, sizeof(inputCameraName));
			}
			GUI::SameLine();
			bool isRemove = false;
			if (GUI::Button("Remove")) {
				isRemove = true;
			}
			else {
			}

			(*camItr)->ShowUI();
			GUI::TreePop();
			if (isRemove) {

				camItr = vec_cameras.erase(camItr);
			}
			else {
				i++;
				camItr++;
			}
		}
		else {
			i++;
			camItr++;
		}
	}

	if (GUI::Button("Add Camera")) {
		auto prop = CameraProjProperty();
		prop.cameraName = "Camera";
		auto size = GetSceneManager().lock()->GetApplication().lock()->GetWindow()->GetSize();
		prop.width = size.x;
		prop.height = size.y;
		prop.layer = 0;
		AddCamera(prop, "Camera", true);
	}

	GUI::End();
}

void ButiEngine::Scene::ShowInspectorUI()
{

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

		auto addComponent = ComponentsLoader::GetInstance()->ShowAddGameComponentUI();

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

void ButiEngine::Scene::ShowHeirarcyUI()
{
	shp_gameObjectManager->ShowUI();
}

