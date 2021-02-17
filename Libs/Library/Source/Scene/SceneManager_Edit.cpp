#include"stdafx.h"
#include "..\..\Header\Scene\SceneManager_Edit.h"
#include"..\..\Header\Scene/Scene.h"
#include <thread>
#include"Header/Resources/DrawData/DrawData_Dx12.h"

ButiEngine::SceneManager_Edit::SceneManager_Edit(std::weak_ptr<IApplication> arg_wkp_app)
	:SceneManager(arg_wkp_app)
{
}

void ButiEngine::SceneManager_Edit::Initialize()
{
	auto windowSize = wkp_app.lock()->GetWindow()->GetSize();
	screenTextureTag = wkp_app.lock()->GetResourceContainer()->LoadTexture(":/_editorScreen/"+std::to_string(windowSize.x)+"/"+std::to_string(windowSize.y));
	MaterialVariable materialVar;

	screenMaterialTag = wkp_app.lock()->GetResourceContainer()->LoadMaterial(materialVar, {screenTextureTag},"_editorScreenMaterial");
	wkp_app.lock()->GetGraphicDevice()->SetDefaultRenderTarget(screenTextureTag);
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
			CreateScreenDrawData();
			currentScene->CameraEditActivation();
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
		screenDrawData->DrawBefore();

		auto updateThread = std::thread(SceneUpdate_ed,currentScene);

		currentScene->Draw();

		shp_camera->BefDraw();
		shp_camera->Start();
		GetApplication().lock()->GetGUIController()->Draw();
		shp_camera->Stop();
		shp_camera->End();

		shp_camera->BefDraw();
		shp_camera->Start();
		screenDrawData->Draw();
		shp_camera->Stop();
		shp_camera->End();

		wkp_app.lock()->GetGraphicDevice()->Update();

		wkp_app.lock()->GetGraphicDevice()->ResourceUploadRelease();
		updateThread.join();
	}
	else {
		GUI::SetState(GUI::GUIState::enable);

		currentScene->RegistGameObjects();
		currentScene->BefDraw();
		screenDrawData->DrawBefore();
		currentScene->Draw();

		shp_camera->BefDraw();
		shp_camera->Start();

		GetApplication().lock()->GetGUIController()->Draw();

		shp_camera->Stop();
		shp_camera->End();

		shp_camera->BefDraw();
		shp_camera->Start();
		screenDrawData->Draw();
		shp_camera->Stop();
		shp_camera->End();

		wkp_app.lock()->GetGraphicDevice()->Update();

		wkp_app.lock()->GetGraphicDevice()->ResourceUploadRelease();

	}

}

void ButiEngine::SceneManager_Edit::UIUpdate()
{
	GUI::Begin("top");
	if (GUI::ArrowButton("##play", GUI::GuiDir_Right) || (GameDevice::GetInput()->TriggerKey(Keys::F5) && GameDevice::GetInput()->CheckKey(Keys::LeftShift))) {
		isActive = !isActive;

		isPlaying = true;
		if (isActive) {
			currentScene->CameraActivation();

			startCount++;
			if (startCount == 1) {
				currentScene->Save();
				currentScene->Start();
			}
		}
		else {

			currentScene->CameraEditActivation();
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


	GUI::End();



	currentScene->ShowHeirarcyUI();

	currentScene->GetResourceContainer()->ShowGUI();

	currentScene->ShowInspectorUI();
	currentScene->ShowRenderingUI();
	
	currentScene->GetCollisionManager().lock()->ShowGUI();
	
	GUI::GuiWindowFlags window_flags =0;
	window_flags |=  GUI::GuiWindowFlags_NoCollapse ;
	window_flags |= GUI::GuiWindowFlags_NoBringToFrontOnFocus | GUI::GuiWindowFlags_NoNavFocus;
	GUI::PushStyleVar(GUI::GuiStyleVar_WindowRounding, 0.0f);
	GUI::PushStyleVar(GUI::GuiStyleVar_WindowBorderSize, 0.0f);
	GUI::PushStyleVar(GUI::GuiStyleVar_WindowPadding, Vector2(0.0f, 0.0f));

	GUI::Begin("Scene", nullptr,window_flags);

	GUI::PopStyleVar(3);
	
	auto w = GUI::GetWindowWidth();
	auto h= GUI::GetWindowHeight();

	auto pos= GUI::GetWindowPos();
	auto windowSize = wkp_app.lock()->GetWindow()->GetSize();
	if (w > (h - 20) * 1920 / 1080) {
		screenDrawData->shp_transform->SetLocalScale(Vector3((h-20)*1920/1080, h - 20, 1));
	}
	else {
		screenDrawData->shp_transform->SetLocalScale(Vector3(w, w*1080/1920, 1));
	}
	screenDrawData->shp_transform->SetLocalPosition(Vector3(pos.x-windowSize.x/2+w/2,-10 -pos.y+windowSize.y/2-h/2,-10));
	if (GUI::IsWindowHovered()) {
		currentScene->EditCameraUpdate();
	}

	GUI::End();

}

void ButiEngine::SceneManager_Edit::RenewalScene()
{
	if (currentScene != nullptr)
		currentScene->SceneEnd();
	currentScene = newScene;
	newScene = nullptr;
	currentScene->Set();
	if(isPlaying)
	currentScene->Start();

	CreateScreenDrawData();
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

		currentScene->CameraEditActivation();

		CreateScreenDrawData();
	}
}

void ButiEngine::SceneManager_Edit::Release()
{
	wkp_app.lock()->GetResourceContainer()->UnLoadMaterial(screenMaterialTag);
	wkp_app.lock()->GetResourceContainer()->UnLoadTexture(screenTextureTag);
	screenDrawData = nullptr;
	auto itr = map_iscene.begin();
	while (itr != map_iscene.end())
	{
		itr->second->Release();
		itr = map_iscene.erase(itr);
	}
}

void ButiEngine::SceneManager_Edit::CreateScreenDrawData()
{
	auto renderer = currentScene->GetRenderer();

	{
		auto meshTag = wkp_app.lock()->GetResourceContainer()->GetMeshTag("Plane_UV");
		auto shaderTag = wkp_app.lock()->GetResourceContainer()->GetShaderTag("UVMesh");
		auto shp_drawInfo = ObjectFactory::Create<DrawInformation>();
		auto lightBuffer_Dx12 = ObjectFactory::Create<CBuffer_Dx12<LightVariable>>(3);

		shp_drawInfo->vec_exCBuffer.push_back(lightBuffer_Dx12);

		auto light = LightVariable();
		light.lightDir = Vector4(Vector3(-1.0f, -1.0f, 0.0f), 1);
		lightBuffer_Dx12->SetExName("LightBuffer");
		lightBuffer_Dx12->Get() = light;

		auto endItr = shp_drawInfo->vec_exCBuffer.end();
		for (auto itr = shp_drawInfo->vec_exCBuffer.begin(); itr != endItr; itr++) {
			(*itr)->Initialize();
		}
		screenDrawData = ObjectFactory::Create<MeshDrawData_Dx12>(meshTag, shaderTag, screenMaterialTag, renderer, wkp_app.lock()->GetGraphicDevice()->GetThis<GraphicDevice_Dx12>(), shp_drawInfo, ObjectFactory::Create<Transform>(Vector3(0,0,-1), Vector3(), Vector3(960, 540, 1)));
		screenDrawData->CommandSet();
		screenDrawData->shp_drawInfo->isDepth = false;
	}
	auto windowSize = wkp_app.lock()->GetWindow()->GetSize();
	auto cameraProp = CameraProjProperty(windowSize.x, windowSize.y, 0, 0);
	cameraProp.farClip = 200.0f;
	cameraProp.cameraName = "editorMain";
	cameraProp.isEditActive = false;
	cameraProp.isInitActive = true;
	cameraProp.isPararell = true;
	shp_camera = CameraCreater::CreateCamera(cameraProp, cameraProp.cameraName, true, currentScene->GetRenderer(), wkp_app.lock()->GetGraphicDevice());
	shp_camera->shp_transform->TranslateZ(-20.0);
}

