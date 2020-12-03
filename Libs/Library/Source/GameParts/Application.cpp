#pragma once

#include "stdafx.h"
#include"Header/Resources/Vertex.h"
#include"Header/Device/Helper/MeshHelper.h"
#include"Header/GameParts/ResourceContainer.h"
#include"Header/Device/ModelFileConverter.h"
#include "..\..\Header\GameParts\Application.h"
#include"..\..\Header/Scene/SceneManager.h"
#include"Header/GameParts/GraphicDevice_Dx12.h"

#include"../../Header/Common/Window.h"

#include "Header/Scene/ComponentsLoader.h"
std::string ButiEngine::GlobalSettings::resourceDirectory = "Resources/";
ButiEngine::Vector3 ButiEngine::Vector3::XAxis = ButiEngine::Vector3(1, 0, 0);
ButiEngine::Vector3 ButiEngine::Vector3::YAxis = ButiEngine::Vector3(0, 1, 0);
ButiEngine::Vector3 ButiEngine::Vector3::ZAxis = ButiEngine::Vector3(0, 0, 1);


const float frame_min = (1.0f / 60.0f) * 1000;

void ButiEngine::Application::CreateInstances(const std::string windowName, const WindowPopType arg_windowPopType, const UINT windowWidth , const UINT windowHeight, const bool isFullScreen )
{
	if (!unq_window) {
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
		unq_window = std::make_unique<Window>();
		unq_window->Initialize(windowName,arg_windowPopType,isFullScreen, windowWidth, windowHeight);
	}
	shp_graphicDevice = ObjectFactory::Create<GraphicDevice_Dx12>(GetThis<IApplication>());

	unq_imguiController = std::make_unique<ImguiController>(unq_window, shp_graphicDevice->GetThis<GraphicDevice_Dx12>());

	
	if (!shp_resourceContainer) {
		
		shp_resourceContainer = ObjectFactory::Create<ResourceContainer>();
		
		shp_resourceContainer->SetGraphicDevice(shp_graphicDevice);

		//InputCereal(shp_resourceContainer, shp_graphicDevice);
	}
	
	if (!shp_sceneManager) {
		shp_sceneManager = std::make_unique<SceneManager>(GetThis<IApplication>());
	}

	if (!unq_gameObjTagManager) {
		unq_gameObjTagManager = std::make_unique<GameObjectTagManager>();
	}

	ButiRandom::Initialize();


	std::timespec_get(&befTs, TIME_UTC);

	timeBeginPeriod(1);

	ComponentsLoader::CreateInstance();

}


std::unique_ptr<ButiEngine::IWindow>& ButiEngine::Application::GetWindow()
{
	return unq_window;
}

std::shared_ptr<ButiEngine::ISceneManager> ButiEngine::Application::GetSceneManager()
{
	return shp_sceneManager;
}

std::shared_ptr<ButiEngine::GraphicDevice> ButiEngine::Application::GetGraphicDevice()
{
	return shp_graphicDevice;
}

std::shared_ptr<ButiEngine::IResourceContainer> ButiEngine::Application::GetResourceContainer()
{
	return shp_resourceContainer;
}

std::unique_ptr<ButiEngine::ImguiController>& ButiEngine::Application::GetGUIController()
{
	return unq_imguiController;
}

std::unique_ptr<ButiEngine::GameObjectTagManager>& ButiEngine::Application::GetGameObjectTagManager()
{
	return unq_gameObjTagManager;
}

bool ButiEngine::Application::Update()
{
	unq_imguiController->Start();
	shp_sceneManager->Update();
	shp_sceneManager->Draw();
	return unq_window->Update();
}

int ButiEngine::Application::Run()
{
	while (Update())
	{
		GameDevice::GetInput()->PadUpdate();
		GameDevice::GetInput()->MouseUpdate();

		if (GameDevice::GetInput()->CheckKey(Keys::Esc)) {
			return 1;
		}

		std::timespec_get(&nowTs, TIME_UTC);
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::nanoseconds{ nowTs.tv_nsec }) - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::nanoseconds{ befTs.tv_nsec });
		if (ms.count() > 0) {
			auto sleepTime = frame_min - ms.count();
			if (sleepTime > 0) {
				Sleep(sleepTime);
			}
			else {
			}
		}
		std::timespec_get(&befTs, TIME_UTC);
	}
	return 0;
}

void ButiEngine::Application::InitLoadResources()
{
	{
		BackupData<Vertex::Vertex_UV_Normal> uv_normalVertices;
		BackupData<Vertex::Vertex_UV_Color> uv_colorVertices;
		BackupData<Vertex::Vertex_UV> uvVertices;
		BackupData<Vertex::Vertex_Normal> normalVertices;
		BackupData<Vertex::Vertex_UV_Normal_Color> testVertices;


		std::vector<Color> colors;

		auto container = Application::GetResourceContainer()->GetThis<ResourceContainer>();

		MeshHelper::CreateSphereForParticle(Vector3(0.5f, 0.5f, 0.5f), 8, colors, testVertices);

		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);
		container->LoadMesh("SphereForParticle_UV_Normal", uv_normalVertices);


		MeshHelper::CreateSphere(Vector3(1.0f, 1.0f, 1.0f), 12, colors, testVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, normalVertices);
		container->LoadMesh("Sphere_Normal", normalVertices);


		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);
		container->LoadMesh("Sphere_UV_Normal", uv_normalVertices);
		testVertices.Clear();
		uvVertices.Clear();
		normalVertices.Clear();
		uv_normalVertices.Clear();

		MeshHelper::CreateCube(Vector3(0.5f, 0.5f, 0.5f), colors, testVertices, true);
		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);
		container->LoadMesh("Cube_UV_Normal", uv_normalVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, normalVertices);
		container->LoadMesh("Cube_Normal", normalVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uvVertices);
		container->LoadMesh("Cube_UV", uvVertices);
		testVertices.Clear();
		uvVertices.Clear();
		normalVertices.Clear();
		uv_normalVertices.Clear();


		MeshHelper::CreatePlane(Vector2(1, 1), Vector3(),0, 1.0f, 1, 1, colors, false, testVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);
		container->LoadMesh("Plane_UV_Normal", uv_normalVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, normalVertices);
		container->LoadMesh("Plane_Normal", normalVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uvVertices);
		container->LoadMesh("Plane_UV", uvVertices);

		testVertices.Clear();
		uvVertices.Clear();
		normalVertices.Clear();
		uv_normalVertices.Clear();

		MeshHelper::CreatePlane(Vector2(1, 1),Vector3(),0, 10.0f, 2, 2, colors, false, testVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);
		container->LoadMesh("Plane_UV_Normal_max10", uv_normalVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uvVertices);
		container->LoadMesh("Plane_UV_max10", uvVertices);

		testVertices.Clear();
		uvVertices.Clear();
		normalVertices.Clear();
		uv_normalVertices.Clear();

		MeshHelper::CreatePlane(Vector2(1, 1),Vector3(),0, 100.0f, 2, 2, colors, false, testVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);
		container->LoadMesh("Plane_UV_Normal_max100", uv_normalVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uvVertices);
		container->LoadMesh("Plane_UV_max100", uvVertices);
		testVertices.Clear();
		uvVertices.Clear();
		normalVertices.Clear();
		uv_normalVertices.Clear();


		MeshHelper::CreateImmediateMeshForParticle(2048, testVertices);
		container->LoadRealTimeMesh("Particle", testVertices);

		testVertices.Clear();
		MeshHelper::CreateSphereForParticle(Vector3(0.5f, 0.5f, 0.5f),4, colors, testVertices);

		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);
		container->LoadMesh("SphereForParticle_UV_Normal_min", uv_normalVertices);


		testVertices.Clear();
		uv_normalVertices.Clear();
		MeshHelper::CreatePlane(Vector2(1, 1), Vector3(0.5,0,0),0.1 ,1.0f, 1, 1, colors, false, testVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);
		container->LoadMesh("Bar", uv_normalVertices);

	}

	//Application::GetResourceContainer()->Reload();



	{
	std::vector<std::pair<std::string, std::string>> vec_pixelShaderPath = {
		{"VertexUVNormalColorMeshPS", "Shader/Compiled/"},
		{"VertexUVColorMeshPS", "Shader/Compiled/"},
		{"VertexUVMeshPS", "Shader/Compiled/"},
		{"TextMeshPS", "Shader/Compiled/"},
		{"GlidPS", "Shader/Compiled/"},
		{"AmbientPS", "Shader/Compiled/"},
		{"AmbientFogPS", "Shader/Compiled/"},
		{"AmbientPS_Alpha", "Shader/Compiled/"},
		{"DefaultMeshPS", "Shader/Compiled/"},
	};
	Application::GetResourceContainer()->LoadPixelShader(vec_pixelShaderPath);

	std::vector<std::pair<std::string, std::string>> vec_vertexShaderPath = {
		{"SingleBoneVS", "Shader/Compiled/"},
		{"QuadBoneVS", "Shader/Compiled/"},
		{"PMXVS", "Shader/Compiled/"},
		{"PMX_AttributeVS", "Shader/Compiled/"},
		{"DefaultMeshVS", "Shader/Compiled/"},
		{"DefaultMeshFogVS", "Shader/Compiled/"},
		{"VertexUVMeshVS", "Shader/Compiled/"},
		{"VertexUVNormalColorMeshVS", "Shader/Compiled/"},
		{"VertexUVNormalAttributeVS", "Shader/Compiled/"},
		{"ParticleVS", "Shader/Compiled/"},
		{"Particle3DVS", "Shader/Compiled/"},
		{"ParticleSpray3DVS", "Shader/Compiled/"},
		{"ImmediateParticleVS", "Shader/Compiled/"},
	};
	Application::GetResourceContainer()->LoadVertexShader(vec_vertexShaderPath);

	std::vector<std::pair<std::string, std::string>> vec_geometryShaderPath = {
		{"TestGS", "Shader/Compiled/"},
		{"PointToPlaneGS", "Shader/Compiled/"},
		{"PointToCubeGS", "Shader/Compiled/"},
		{"PointToPyramidGS", "Shader/Compiled/"},
	};
	Application::GetResourceContainer()->LoadGeometryShader(vec_geometryShaderPath);

	std::vector<ResourceContainer::ShaderName> vec_names = {
		{"DefaultMesh","DefaultMeshVS","DefaultMeshPS", "Shader/Compiled/", "Shader/Compiled/"},
		{"OnlyMaterial","DefaultMeshFogVS","AmbientFogPS", "Shader/Compiled/", "Shader/Compiled/"},
		{"GSMesh","VertexUVNormalAttributeVS","DefaultMeshPS", "Shader/Compiled/", "Shader/Compiled/","TestGS","Shader/Compiled/"},
		{"SingleBoneModel","SingleBoneVS","DefaultMeshPS", "Shader/Compiled/", "Shader/Compiled/"},
		{"PMXModel","PMXVS","DefaultMeshPS", "Shader/Compiled/", "Shader/Compiled/"},
		{"PMXModel_GS","PMX_AttributeVS","DefaultMeshPS", "Shader/Compiled/", "Shader/Compiled/","TestGS","Shader/Compiled/"},
		{"QuadModel","QuadBoneVS","DefaultMeshPS", "Shader/Compiled/", "Shader/Compiled/"},
		{"Glid","VertexUVMeshVS","GlidPS", "Shader/Compiled/", "Shader/Compiled/"},

		{ "GSParticle_Standard","ParticleVS","VertexUVColorMeshPS", "Shader/Compiled/", "Shader/Compiled/","PointToPlaneGS","Shader/Compiled/" },
		
		{ "GSParticle_Cube","Particle3DVS","VertexUVNormalColorMeshPS", "Shader/Compiled/", "Shader/Compiled/","PointToCubeGS","Shader/Compiled/" },
		
		{ "GSParticle_Pyramid","Particle3DVS","VertexUVNormalColorMeshPS", "Shader/Compiled/", "Shader/Compiled/","PointToPyramidGS","Shader/Compiled/" },
		{ "ImmdeiateParticle_Plane","ImmediateParticleVS","VertexUVNormalColorMeshPS", "Shader/Compiled/", "Shader/Compiled/","PointToCubeGS","Shader/Compiled/" },
		{"UVMesh","VertexUVMeshVS","VertexUVMeshPS", "Shader/Compiled/", "Shader/Compiled/"},

		{"OnlyMaterial_Alpha","DefaultMeshVS","AmbientPS_Alpha", "Shader/Compiled/", "Shader/Compiled/"},
		{"OnlyMaterialNoFog","DefaultMeshVS","AmbientPS", "Shader/Compiled/", "Shader/Compiled/"},

		{ "GSParticle_CubeSpray","ParticleSpray3DVS","VertexUVNormalColorMeshPS", "Shader/Compiled/", "Shader/Compiled/","PointToCubeGS","Shader/Compiled/" },
		{"TextMesh","VertexUVMeshVS","TextMeshPS", "Shader/Compiled/", "Shader/Compiled/"},
	};

	Application::GetResourceContainer()->LoadShader(vec_names);

	//ModelFileConverter::FBXtoB3M("ball.fbx", "sphere.b3m", "Model/FBX/");
	//ModelFileConverter::FBXtoB3M("sango_another.fbx", "sango_another.b3m", "Model/FBX/");
	//ModelFileConverter::FBXtoB3M("coin.fbx", "coin.b3m", "Model/FBX/");
	//ModelFileConverter::FBXtoB3M("gun.fbx", "gun.b3m", "Model/Maguro/");
	//ModelFileConverter::PMXtoB3M("インパクトドライバー.pmx", "driver.b3m", "Model/");
	//ModelFileConverter::PMXtoB3M("ウルトラマンヒカリ._準ボーン.pmx", "hikari.b3m", "Model/aomoti式_ウルトラマンヒカリ/");

	//ModelFileConverter::PMXtoB3M("畳.pmx", "tatami.b3m", "Model/畳/");
	//ModelFileConverter::VMDtoBMD("Alicia_run.vmd", "slash.bmd", "Motion/");

	std::vector<std::pair<std::string, std::string>> vec_modelPath = {
		//{"tatami.b3m", "Model/畳/"},
		//{"driver.b3m", "Model/"},
		{"hikari.b3m", "Model/aomoti式_ウルトラマンヒカリ/"},
		{"maguro.b3m", "Model/Maguro/"},
		{"aircraft.b3m", "Model/AirBattle/"},
		{"block.b3m", "Model/AirBattle/"},
		{"enemy.b3m", "Model/AirBattle/"},
		{"sword.b3m", "Model/AirBattle/"},
		{"cloud.b3m", "Model/AirBattle/"},
	};

	Application::GetResourceContainer()->LoadModel(vec_modelPath);

	std::vector<std::pair<std::string, std::string>> vec_motionPath = {
		{"bakutyuu.bmd","Motion/"},
		{"slash.bmd","Motion/"},
		{"ojigi.bmd","Motion/"},
	};

	Application::GetResourceContainer()->LoadMotion(vec_motionPath);

	std::vector<std::pair<std::string, std::string>> vec_texturePath = {
		{"num_0.png","Texture/"},
		{"num_1.png","Texture/"},
		{"num_2.png","Texture/"},
		{"num_3.png","Texture/"},
		{"num_4.png","Texture/"},
		{"num_5.png","Texture/"},
		{"num_6.png","Texture/"},
		{"num_7.png","Texture/"},
		{"num_8.png","Texture/"},
		{"num_9.png","Texture/"},
		{"num_comma.png","Texture/"},
		{"black.png","Texture/"},
	};



	auto vec_textureTag = Application::GetResourceContainer()->LoadTexture(vec_texturePath);


	std::vector<std::pair<std::string, std::string>> vec_materialPath = {
		{"childMaterial.bma", "Material/"},
		{"particleMaterial.bma", "Material/"},
	};

	Application::GetResourceContainer()->LoadMaterial(vec_materialPath);

	MaterialVariable materialVar;

	materialVar.diffuse = Vector4(1.0, 1.0, 1.0, 1.0);
	for(int i=0;i<10;i++)
		Application::GetResourceContainer()->LoadMaterial(materialVar, vec_textureTag.at(i), "num_" + std::to_string(i));
	Application::GetResourceContainer()->LoadMaterial(materialVar, vec_textureTag.at(10), "num_comma");

	materialVar.diffuse = Vector4(0.9, 0.1, 0.1, 1.0);
	Application::GetResourceContainer()->LoadMaterial(materialVar, vec_textureTag.at(11), "hpBar_red");
	materialVar.diffuse = Vector4(0.1, 0.9, 0.1, 1.0);
	Application::GetResourceContainer()->LoadMaterial(materialVar, vec_textureTag.at(11), "hpBar_green");

	std::vector<std::pair<std::string, std::string>> vec_soundPath = {
		{"bgm.wav","Sound/"},
		{"se_Damege.wav","Sound/"},
		{"se_Bomb.wav","Sound/"},
		{"se_Decision.wav","Sound/"},
		{"se_Select.wav","Sound/"},
		{"se_Slash.wav","Sound/"},
	};

	Application::GetResourceContainer()->LoadSound(vec_soundPath);
	vec_modelPath.clear();
	vec_modelPath = {
		{"enemy_2.b3m", "Model/AirBattle/"},
		{"enemy_3.b3m", "Model/AirBattle/"},
		{"Building.b3m", "Model/AirBattle/"},
		{"BorderLine.b3m", "Model/AirBattle/"},
	};

	Application::GetResourceContainer()->LoadModel(vec_modelPath);

	materialVar.diffuse = Vector4(0.0, 0.0, 0.0, 0.6);

	auto blackTag = vec_textureTag.at(11);

	Application::GetResourceContainer()->LoadMaterial(materialVar, blackTag, "blackMaterial");
	vec_texturePath.clear();
	vec_texturePath = {
		{"UI_Score.png","Texture/Re_UI_Images/"},
		{"UI_Push_A.png","Texture/Re_UI_Images/"},
		{"UI_HighScore.png","Texture/Re_UI_Images/"},
		{"UI_Failed.png","Texture/Re_UI_Images/"},
		{"UI_title.png","Texture/Re_UI_Images/"},
		{"UI_Stage3.png","Texture/Re_UI_Images/"},
		{"UI_Stage2.png","Texture/Re_UI_Images/"},
		{"UI_Stage1.png","Texture/Re_UI_Images/"},
		{"UI_Retry.png","Texture/Re_UI_Images/"},
		{"UI_Select.png","Texture/Re_UI_Images/"},
		{"UI_Clear.png","Texture/Re_UI_Images/"},
	};


	vec_textureTag = Application::GetResourceContainer()->LoadTexture(vec_texturePath);

	materialVar.diffuse = Vector4(1.0, 1.0, 1.0, 1.0);
	Application::GetResourceContainer()->LoadMaterial(materialVar, vec_textureTag.at(0), "scoreUI");
	Application::GetResourceContainer()->LoadMaterial(materialVar, vec_textureTag.at(1), "UI_Push_A");
	Application::GetResourceContainer()->LoadMaterial(materialVar, vec_textureTag.at(2), "UI_HighScore");
	Application::GetResourceContainer()->LoadMaterial(materialVar, vec_textureTag.at(3), "UI_Failed");
	Application::GetResourceContainer()->LoadMaterial(materialVar, vec_textureTag.at(4), "UI_title");
	Application::GetResourceContainer()->LoadMaterial(materialVar, vec_textureTag.at(5), "UI_Stage3");
	Application::GetResourceContainer()->LoadMaterial(materialVar, vec_textureTag.at(6), "UI_Stage2");
	Application::GetResourceContainer()->LoadMaterial(materialVar, vec_textureTag.at(7), "UI_Stage1");
	Application::GetResourceContainer()->LoadMaterial(materialVar, vec_textureTag.at(8), "UI_Retry");
	Application::GetResourceContainer()->LoadMaterial(materialVar, vec_textureTag.at(9), "UI_Select");
	Application::GetResourceContainer()->LoadMaterial(materialVar, vec_textureTag.at(10), "UI_Clear");

	materialVar.diffuse = Vector4(0.95, 0, 0, 0.7);

	Application::GetResourceContainer()->LoadMaterial(materialVar, blackTag, "warn_Red");
}
}

void ButiEngine::Application::Exit()
{
	shp_sceneManager->Release();
	unq_window->Release();
	OutputCereal(shp_resourceContainer->GetThis<ResourceContainer>());
	shp_resourceContainer->Release();
	unq_imguiController->Release();
	shp_graphicDevice->Release();

	ComponentsLoader::GetInstance()->Release();
}