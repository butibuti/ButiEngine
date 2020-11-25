#include"stdafx.h"
#include"Header/GameParts/SoundManager.h"
#include"Header/GameParts/Renderer.h"


#include"include/HitTestBehavior.h"
#include"Header/GameParts/ResourceContainer.h"
#include"Header/Resources/ModelAnimation.h"
#include"Header/Scene/ComponentsLoader.h"




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
	ImGui::Begin("top");
	if (ImGui::ArrowButton("##play", ImGuiDir_Right)) {
		isActive =!isActive;
		isPlaying = true;
		if (isActive) {
			startCount++;
			if (startCount==1) {
				OutputCereal(shp_gameObjectManager,GlobalSettings::GetResourceDirectory()+ "Scene/" + sceneInformation.GetSceneName()+"/objects.gameObjectManager" );
				shp_gameObjectManager->Start();
			}
		}

	};
	ImGui::SameLine();
	if (ImGui::Button("Reset")) {
		isPlaying = false;
		isActive = false;
		startCount = 0;

		shp_gameObjectManager = nullptr;
		shp_soundManager->Release();
		shp_renderer->Release();
		vec_cameras.clear();

		{
			if (shp_collisionManager) {
				shp_collisionManager = nullptr;
			}

			shp_renderer = ObjectFactory::Create<Renderer>(GetThis<IScene>());


			shp_renderer->AddLayer();
			shp_soundManager = ObjectFactory::Create<SoundManager>(GetThis<IScene>());

			auto windowSize = GetWindow()->GetSize();
			std::string fullGameObjectManagerPath = GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation.GetSceneName() + "/objects.gameObjectManager";
			if (Util::IsFileExistence(fullGameObjectManagerPath)) {
				shp_gameObjectManager = ObjectFactory::CreateFromCereal<GameObjectManager>(fullGameObjectManagerPath);

				shp_gameObjectManager->SetScene(GetThis<IScene>());
				shp_gameObjectManager->Initialize_cereal();
			}
			else {
				_mkdir((GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation.GetSceneName() + "/").c_str());
				shp_gameObjectManager = ObjectFactory::Create<GameObjectManager>(GetThis<IScene>());
			}
			/*
			shp_renderer->AddLayer();
			shp_renderer->AddLayer();
			auto prop2 = CameraProjProperty(windowSize.x, windowSize.y, 0, 0,true,1);
			AddCamera(prop2, "backGround", true);*/

			auto prop = CameraProjProperty(windowSize.x, windowSize.y, 0, 0);
			prop.farClip = 150.0f;
			AddCamera(prop, "main", true);
			AddCamera(prop, "edit", true);

			GetCamera("edit").lock()->shp_transform->SetLocalPosition(Vector3(5, 5, -5));

			GetCamera("edit").lock()->shp_transform->SetLookAtRotation(Vector3(0, 0, 0));
		}
	}

	if (!isPlaying) {

		ImGui::SameLine();
		if (ImGui::Button("Save!!!!")|| ((GameDevice::input.CheckKey(Keys::LeftCtrl) || GameDevice::input.CheckKey(Keys::RightCtrl)) && GameDevice::input.CheckKey(Keys::S))) {

			OutputCereal(shp_gameObjectManager, GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation.GetSceneName() + "/objects.gameObjectManager");

		};
	}
	ImGui::Checkbox("ShowHierarchy", &showHeirarcy);

	ImGui::SameLine();
	ImGui::Checkbox("ShowInspector", &showInspector);

	ImGui::SameLine();
	ImGui::Checkbox("ShowContainer", &showContainer);


	ImGui::End();


	/*ImGui::Begin("Bara Bara Slider");
	ImGui::SliderFloat("pushPower", &t, 0, 1.0f);
	ImGui::End();*/
	if(showHeirarcy)
	shp_gameObjectManager->ShowUI();



	if (showContainer) {
		GetResourceContainer()->ShowGUI();

	}
	if (showInspector) {

		auto selectedGameObject = shp_gameObjectManager->GetSelectedUI();
		ImGui::Begin("Inspector");
		if (selectedGameObject.lock()) {


			if ((GameDevice::input.CheckKey(Keys::LeftCtrl) || GameDevice::input.CheckKey(Keys::RightCtrl)) && GameDevice::input.CheckKey(Keys::F)) {
				GetCamera("edit").lock()->shp_transform->SetLookAtRotation(selectedGameObject.lock()->transform->GetWorldPosition());
			}

			ImGui::InputTextWithHint("Name", selectedGameObject.lock()->GetGameObjectName().c_str(), CallBacks::objectName, 64, 64, CallBacks::ImguiCallBack);
			ImGui::SameLine();

			if (ImGui::Button("Change")) {
				selectedGameObject.lock()->SetObjectName(CallBacks::objectName);
				CallBacks::ObjectNameReset();

			}


			selectedGameObject.lock()->ShowUI();

			auto addComponent = shp_componentsLoader->ShowAddGameComponentUI();

			if (addComponent)
				selectedGameObject.lock()->AddGameComponent_Insert(addComponent);

			auto addBehavior = shp_componentsLoader->ShowAddBehaviorUI();
			if (addBehavior)
				selectedGameObject.lock()->AddBehavior_Insert(addBehavior);


			if (ImGui::Button("Save!", ImVec2(200, 30))) {
				OutputCereal(selectedGameObject.lock());
			}
		}



		ImGui::End();
	}

	if(!isActive&&!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
	EditCamera();
}

void ButiEngine::EditScene::EditCamera()
{
	auto editCam = GetCamera("edit").lock();
	if (GameDevice::input.GetMouseButton(MouseButtons::RightClick)) {
		Vector2 move = GameDevice::input.GetMouseMove();
		editCam->shp_transform->RollWorldRotationY_Degrees(-move.x);
		editCam->shp_transform->RollLocalRotationX_Degrees(-move.y);
	}
	if (GameDevice::input.GetMouseButton(MouseButtons::WheelButton)) {
		Vector2 move = GameDevice::input.GetMouseMove();

		Vector3 velocity = editCam->shp_transform->GetRight() * move.x * 0.01f + editCam->shp_transform->GetUp() * move.y*-1 * 0.01f;

		editCam->shp_transform->Translate (velocity);
	}
	if (GameDevice::input.GetMouseWheel()) {
		Vector3 velocity = editCam->shp_transform->GetFront() * 0.001f * GameDevice::input.GetMouseWheelMove();

		editCam->shp_transform->Translate(velocity);
	}
}

void ButiEngine::EditScene::Draw()
{

	shp_renderer->RenderingStart();


	shp_renderer->BefRendering();


	for (auto cameraItr = vec_cameras.begin(); cameraItr != vec_cameras.end(); cameraItr++) {
		if (!(*cameraItr)->GetActive()) {
			continue;
		}

		if ((*cameraItr)->GetName() == "main"&&!isActive) {
			continue;
		}
		else if ((*cameraItr)->GetName() == "edit" && isActive) {
			continue;
		}

		(*cameraItr)->Start();

		(*cameraItr)->Draw();
		
		if(cameraItr+1== vec_cameras.end())
		shp_sceneManager->GetApplication().lock()->GetGUIController()->Draw();

		(*cameraItr)->Stop();
	}



	shp_renderer->RenderingEnd();
}


ButiEngine::EditScene::EditScene(std::weak_ptr<ISceneManager> arg_wkp_sceneManager, SceneInformation arg_SceneInformation, std::shared_ptr<ComponentsLoader> arg_shp_componentsLoader)
	:sceneInformation ( arg_SceneInformation)
{
	shp_componentsLoader = arg_shp_componentsLoader;
	
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

	shp_renderer->AddLayer();
	shp_soundManager = ObjectFactory::Create<SoundManager>(GetThis<IScene>());

	ActiveCollision(1);

	auto windowSize = GetWindow()->GetSize();
	std::string fullGameObjectManagerPath = GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation.GetSceneName() + "/objects.gameObjectManager";
	if (Util::IsFileExistence(fullGameObjectManagerPath)) {
		shp_gameObjectManager = ObjectFactory::CreateFromCereal<GameObjectManager>(fullGameObjectManagerPath);

		shp_gameObjectManager->SetScene(GetThis<IScene>());
		shp_gameObjectManager->Initialize_cereal();
	}
	else {
		auto defaultPath = GlobalSettings::GetResourceDirectory() + "Scene/" +"DefaultScene"+ "/objects.gameObjectManager";
		shp_gameObjectManager = ObjectFactory::CreateFromCereal<GameObjectManager>(defaultPath);

		shp_gameObjectManager->SetScene(GetThis<IScene>());
		shp_gameObjectManager->Initialize_cereal();

		_mkdir((GlobalSettings::GetResourceDirectory() + "Scene/" + sceneInformation.GetSceneName() + "/").c_str());
	}
	

	auto prop = CameraProjProperty(windowSize.x, windowSize.y, 0, 0);
	prop.farClip = 200.0f;
	AddCamera(prop, "main", true);
	auto prop2 = CameraProjProperty(windowSize.x, windowSize.y, 0, 0,true,1);
	AddCamera(prop, "edit", true);
	AddCamera(prop2, "ui", true);

	GetCamera("edit").lock()->shp_transform->SetLocalPosition(Vector3(5, 5, -5));

	GetCamera("edit").lock()->shp_transform->SetLookAtRotation(Vector3(0, 0, 0));

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

std::unique_ptr<ButiEngine::Window>& ButiEngine::EditScene::GetWindow()
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

std::shared_ptr<ButiEngine::ResourceContainer> ButiEngine::EditScene::GetResourceContainer()
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

ButiEngine::SceneInformation ButiEngine::EditScene::GetSceneInformation()
{
	return sceneInformation;
}

