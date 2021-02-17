#pragma once

#include "stdafx.h"
#include"Header/Resources/Vertex.h"
#include"Header/Device/Helper/MeshHelper.h"
#include"Header/GameParts/ResourceContainer.h"
#include"Header/Device/ModelFileConverter.h"
#include "..\..\Header\GameParts\Application.h"
#include"..\..\Header/Scene/SceneManager_Edit.h"
#include"Header/GameParts/GraphicDevice_Dx12.h"
#include"Header/GameParts/SoundManager.h"

#include"../../Header/Common/Window.h"

#include "Header/Scene/ComponentsLoader.h"
std::string ButiEngine::GlobalSettings::resourceDirectory = "Resources/";
ButiEngine::Vector3 ButiEngine::Vector3::XAxis = ButiEngine::Vector3(1, 0, 0);
ButiEngine::Vector3 ButiEngine::Vector3::YAxis = ButiEngine::Vector3(0, 1, 0);
ButiEngine::Vector3 ButiEngine::Vector3::ZAxis = ButiEngine::Vector3(0, 0, 1);
ButiEngine::Vector3 ButiEngine::Vector3::Zero = ButiEngine::Vector3(0, 0, 0);


const float frame_min = (1.0f / 60.0f) * 1000;

void ButiEngine::Application::CreateInstances(const std::string windowName, const WindowPopType arg_windowPopType, const UINT windowWidth , const UINT windowHeight, const bool isFullScreen, const bool isEditor)
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
		
		auto temp = ObjectFactory::Create<ResourceContainer>();
		
		if (Util::CheckFileExistence(GlobalSettings::GetResourceDirectory() + "Application/resourceLoadData.resource")) {

			InputCereal(temp, shp_graphicDevice);
			shp_resourceContainer = temp;
		}
		else {

			temp->SetGraphicDevice(shp_graphicDevice);
			shp_resourceContainer = temp;
		}
	}
	
	if (!shp_sceneManager) {
		if (isEditor) {
			shp_sceneManager = std::make_unique<SceneManager_Edit>(GetThis<IApplication>());
		}else
		shp_sceneManager = std::make_unique<SceneManager>(GetThis<IApplication>());
	}
	shp_sceneManager->Initialize();
	if (!shp_gameObjTagManager) {
		shp_gameObjTagManager = std::make_shared<GameObjectTagManager>();

		if (Util::CheckFileExistence(GlobalSettings::GetResourceDirectory() + "Application/GameObjectTagManager.tagmanager"))
			InputCereal(shp_gameObjTagManager, "Application/GameObjectTagManager.tagmanager");
	}

	if (!shp_soundManager) {
		shp_soundManager = ObjectFactory::Create<SoundManager>(GetThis<IApplication>());
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

std::shared_ptr<ButiEngine::GameObjectTagManager> ButiEngine::Application::GetGameObjectTagManager()
{
	return shp_gameObjTagManager;
}

std::shared_ptr<ButiEngine::ISoundManager> ButiEngine::Application::GetSoundManager()
{
	return shp_soundManager;
}

bool ButiEngine::Application::Update()
{
	unq_imguiController->Start();

	shp_sceneManager->Update();
	shp_soundManager->Update();
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


		MeshHelper::CreateSphere(Vector3(1.0f, 1.0f, 1.0f), 24, colors, testVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, normalVertices);
		container->LoadMesh("Sphere_Normal", normalVertices);


		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);
		container->LoadMesh("Sphere_UV_Normal", uv_normalVertices);
		testVertices.Clear();
		uvVertices.Clear();
		normalVertices.Clear();
		uv_normalVertices.Clear();

		MeshHelper::CreateCube(Vector3(1.0f, 1.0f, 1.0f), colors, testVertices, false);
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

		MeshHelper::CreateCapsule(Vector3(1, 1, 1),  Vector3(0, -0.5, 0), Vector3(0, 0.5, 0), 12, false, std::vector<Vector4>(), testVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);
		container->LoadMesh("Capsule_UV_Normal", uv_normalVertices);
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


		MeshHelper::CreateCameraFrustum(60, 1280,360, 0.1, 2.5, testVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);

		container->LoadMesh("Camera", uv_normalVertices);
		
	}

	shp_resourceContainer->Reload();



	{
	/*	
	std::vector<std::string> vec_pixelShaderPath = {
		{"Shader/Compiled/VertexUVNormalColorMeshPS"},
		{"Shader/Compiled/VertexUVColorMeshPS"},
		{"Shader/Compiled/VertexUVMeshPS"},
		{"Shader/Compiled/TextMeshPS"},
		{"Shader/Compiled/GlidPS"},
		{"Shader/Compiled/AmbientPS"},
		{"Shader/Compiled/AmbientFogPS"},
		{"Shader/Compiled/AmbientPS_Alpha"},
		{"Shader/Compiled/DefaultMeshPS"},
	};
	Application::GetResourceContainer()->LoadPixelShader(vec_pixelShaderPath);

	std::vector<std::string> vec_vertexShaderPath = {
		{"Shader/Compiled/SingleBoneVS"},
		{"Shader/Compiled/QuadBoneVS"},
		{"Shader/Compiled/PMXVS"},
		{"Shader/Compiled/PMX_AttributeVS"},
		{"Shader/Compiled/DefaultMeshVS"},
		{"Shader/Compiled/DefaultMeshFogVS"},
		{"Shader/Compiled/VertexUVMeshVS"},
		{"Shader/Compiled/VertexUVNormalColorMeshVS"},
		{"Shader/Compiled/VertexUVNormalAttributeVS"},
		{"Shader/Compiled/ParticleVS"},
		{"Shader/Compiled/Particle3DVS"},
		{"Shader/Compiled/ParticleSpray3DVS"},
		{"Shader/Compiled/ImmediateParticleVS"},
	};
	Application::GetResourceContainer()->LoadVertexShader(vec_vertexShaderPath);

	std::vector<std::string> vec_geometryShaderPath = {
		{"Shader/Compiled/TestGS"},
		{"Shader/Compiled/PointToPlaneGS"},
		{"Shader/Compiled/PointToCubeGS"},
		{"Shader/Compiled/PointToPyramidGS"},
	};
	Application::GetResourceContainer()->LoadGeometryShader(vec_geometryShaderPath);

	std::vector<ResourceContainer::ShaderName> vec_names = {
		{"Shader/Compiled/DefaultMesh","Shader/Compiled/DefaultMeshVS","Shader/Compiled/DefaultMeshPS"},
		{"Shader/Compiled/OnlyMaterial","Shader/Compiled/DefaultMeshFogVS","Shader/Compiled/AmbientFogPS"},
		{"Shader/Compiled/GSMesh","Shader/Compiled/VertexUVNormalAttributeVS","Shader/Compiled/DefaultMeshPS","Shader/Compiled/TestGS"},
		{"Shader/Compiled/SingleBoneModel","Shader/Compiled/SingleBoneVS","Shader/Compiled/DefaultMeshPS"},
		{"Shader/Compiled/PMXModel","Shader/Compiled/PMXVS","Shader/Compiled/DefaultMeshPS"},
		{"Shader/Compiled/PMXModel_GS","Shader/Compiled/PMX_AttributeVS","Shader/Compiled/DefaultMeshPS","Shader/Compiled/TestGS"},
		{"Shader/Compiled/QuadModel","Shader/Compiled/QuadBoneVS","Shader/Compiled/DefaultMeshPS"},
		{"Shader/Compiled/Glid","Shader/Compiled/VertexUVMeshVS","Shader/Compiled/GlidPS"},

		{"Shader/Compiled/GSParticle_Standard","Shader/Compiled/ParticleVS","Shader/Compiled/VertexUVColorMeshPS","Shader/Compiled/PointToPlaneGS" },
		
		{"Shader/Compiled/GSParticle_Cube","Shader/Compiled/Particle3DVS","Shader/Compiled/VertexUVNormalColorMeshPS","Shader/Compiled/PointToCubeGS" },
		
		{"Shader/Compiled/GSParticle_Pyramid","Shader/Compiled/Particle3DVS","Shader/Compiled/VertexUVNormalColorMeshPS","Shader/Compiled/PointToPyramidGS" },
		{"Shader/Compiled/ImmdeiateParticle_Plane","Shader/Compiled/ImmediateParticleVS","Shader/Compiled/VertexUVNormalColorMeshPS","Shader/Compiled/PointToCubeGS" },
		{"Shader/Compiled/UVMesh","Shader/Compiled/VertexUVMeshVS","Shader/Compiled/VertexUVMeshPS"},

		{"Shader/Compiled/OnlyMaterial_Alpha","Shader/Compiled/DefaultMeshVS","Shader/Compiled/AmbientPS_Alpha"},
		{"Shader/Compiled/OnlyMaterialNoFog","Shader/Compiled/DefaultMeshVS","Shader/Compiled/AmbientPS"},

		{"Shader/Compiled/GSParticle_CubeSpray","Shader/Compiled/ParticleSpray3DVS","Shader/Compiled/VertexUVNormalColorMeshPS","Shader/Compiled/PointToCubeGS" },
		{"Shader/Compiled/TextMesh","Shader/Compiled/VertexUVMeshVS","Shader/Compiled/TextMeshPS"},
	};

	Application::GetResourceContainer()->LoadShader(vec_names);

	ModelFileConverter::FBXtoB3M("ball.fbx", "sphere.b3m", "Model/FBX/");
	ModelFileConverter::FBXtoB3M("sango_another.fbx", "sango_another.b3m", "Model/FBX/");
	ModelFileConverter::FBXtoB3M("gun.fbx", "gun.b3m", "Model/Maguro/");
	ModelFileConverter::PMXtoB3M("インパクトドライバー.pmx", "driver.b3m", "Model/");

	ModelFileConverter::PMXtoB3M("畳.pmx", "tatami.b3m", "Model/畳/");
	ModelFileConverter::VMDtoBMD("Alicia_run.vmd", "slash.bmd", "Motion/");

	std::vector<std::string>vec_modelPath;

	Application::GetResourceContainer()->LoadModel(vec_modelPath);

	std::vector<std::string> vec_motionPath = {
		{"Motion/bakutyuu.bmd"},
		{"Motion/slash.bmd"},
		{"Motion/ojigi.bmd"},
	};

	Application::GetResourceContainer()->LoadMotion(vec_motionPath);

	std::vector<std::string> vec_texturePath = {
		{"Texture/black.png"},
	};



	auto vec_textureTag = Application::GetResourceContainer()->LoadTexture(vec_texturePath);


	std::vector<IResourceContainer::MaterialLoadInfo> vec_materialLoad = {
		IResourceContainer::MaterialLoadInfo("Material/childMaterial.bma"),
		IResourceContainer::MaterialLoadInfo("Material/particleMaterial.bma"),
	};

	Application::GetResourceContainer()->LoadMaterial(vec_materialLoad);


	std::vector<std::string> vec_soundPath = {
		{"Sound/bgm.wav"},
	};

	Application::GetResourceContainer()->LoadSound(vec_soundPath);
	*/

	}
}

void ButiEngine::Application::Exit()
{

	shp_soundManager->Release();
	shp_sceneManager->Release();
	unq_window->Release();
	OutputCereal(shp_resourceContainer->GetThis<ResourceContainer>());
	OutputCereal(shp_gameObjTagManager, "Application/GameObjectTagManager.tagmanager");
	shp_resourceContainer->Release();
	unq_imguiController->Release();
	shp_graphicDevice->Release();

	ComponentsLoader::GetInstance()->Release();
}