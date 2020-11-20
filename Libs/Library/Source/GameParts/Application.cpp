#pragma once
#include "stdafx.h"
#include"Header/Resources/Vertex.h"
#include"Header/Device/Helper/MeshHelper.h"
#include"Header/GameParts/ResourceContainer.h"
#include"Header/Device/ModelFileConverter.h"

std::string ButiEngine::GlobalSettings::resourceDirectory = "Resources/";
ButiEngine::Matrix4x4 ButiEngine::Transform::x90Rotate;
ButiEngine::Vector3 ButiEngine::Vector3::XAxis = ButiEngine::Vector3(1, 0, 0);
ButiEngine::Vector3 ButiEngine::Vector3::YAxis = ButiEngine::Vector3(0, 1, 0);
ButiEngine::Vector3 ButiEngine::Vector3::ZAxis = ButiEngine::Vector3(0, 0, 1);


void ButiEngine::Application::CreateInstances(const std::string windowName, const WindowPopType arg_windowPopType, const UINT windowWidth , const UINT windowHeight, const bool isFullScreen )
{
	if (!unq_window) {
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
		unq_window = std::make_unique<Window>();
		unq_window->Initialize(windowName,arg_windowPopType, windowWidth, windowHeight);
	}

	shp_graphicDevice = ObjectFactory::Create<GraphicDevice_Dx12>(GetThis<Application>());

	unq_imguiController = std::make_unique<ButiimguiController>(unq_window, shp_graphicDevice->GetThis<GraphicDevice_Dx12>());

	
	if (!shp_resourceContainer) {
		
		shp_resourceContainer = ObjectFactory::Create<ResourceContainer>();
		
		shp_resourceContainer->SetGraphicDevice(shp_graphicDevice);

		//InputCereal(shp_resourceContainer, shp_graphicDevice);
	}
	
	if (!shp_sceneManager) {
		shp_sceneManager = std::make_unique<SceneManager>(GetThis<Application>());
	}
	Transform::x90Rotate= DirectX::XMMatrixRotationX(
		DirectX::XMConvertToRadians(90)
	) *
		DirectX::XMMatrixRotationY(
			DirectX::XMConvertToRadians(0)
		) *
		DirectX::XMMatrixRotationZ(
			DirectX::XMConvertToRadians(0)
		);
	std::timespec_get(&befTs, TIME_UTC);
	timeBeginPeriod(1);
}


std::unique_ptr<ButiEngine::Window>& ButiEngine::Application::GetWindow()
{
	return unq_window;
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


		MeshHelper::CreateSphereForParticle(Vector3(0.5f, 0.5f, 0.5f), 12, colors, testVertices);

		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);
		Application::GetResourceContainer()->LoadMesh("SphereForParticle_UV_Normal", uv_normalVertices);


		MeshHelper::CreateSphere(Vector3(1.0f, 1.0f, 1.0f), 12, colors, testVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, normalVertices);
		Application::GetResourceContainer()->LoadMesh("Sphere_Normal", normalVertices);


		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);
		Application::GetResourceContainer()->LoadMesh("Sphere_UV_Normal", uv_normalVertices);
		testVertices.Clear();
		uvVertices.Clear();
		normalVertices.Clear();
		uv_normalVertices.Clear();

		MeshHelper::CreateCube(Vector3(0.5f, 0.5f, 0.5f), colors, testVertices, true);
		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);
		Application::GetResourceContainer()->LoadMesh("Cube_UV_Normal", uv_normalVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, normalVertices);
		Application::GetResourceContainer()->LoadMesh("Cube_Normal", normalVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uvVertices);
		Application::GetResourceContainer()->LoadMesh("Cube_UV", uvVertices);
		testVertices.Clear();
		uvVertices.Clear();
		normalVertices.Clear();
		uv_normalVertices.Clear();


		MeshHelper::CreatePlane(Vector2(1, 1), 1.0f, 1, 1, colors, false, testVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);
		Application::GetResourceContainer()->LoadMesh("Plane_UV_Normal", uv_normalVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, normalVertices);
		Application::GetResourceContainer()->LoadMesh("Plane_Normal", normalVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uvVertices);
		Application::GetResourceContainer()->LoadMesh("Plane_UV", uvVertices);

		testVertices.Clear();
		uvVertices.Clear();
		normalVertices.Clear();
		uv_normalVertices.Clear();

		MeshHelper::CreatePlane(Vector2(1, 1), 10.0f, 2, 2, colors, false, testVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);
		Application::GetResourceContainer()->LoadMesh("Plane_UV_Normal_max10", uv_normalVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uvVertices);
		Application::GetResourceContainer()->LoadMesh("Plane_UV_max10", uvVertices);

		testVertices.Clear();
		uvVertices.Clear();
		normalVertices.Clear();
		uv_normalVertices.Clear();

		MeshHelper::CreatePlane(Vector2(1, 1), 100.0f, 2, 2, colors, false, testVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);
		Application::GetResourceContainer()->LoadMesh("Plane_UV_Normal_max100", uv_normalVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uvVertices);
		Application::GetResourceContainer()->LoadMesh("Plane_UV_max100", uvVertices);
		testVertices.Clear();
		uvVertices.Clear();
		normalVertices.Clear();
		uv_normalVertices.Clear();


		MeshHelper::CreateImmediateMeshForParticle(1024, testVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uv_colorVertices);
		Application::GetResourceContainer()->LoadRealTimeMesh("Particle", uv_colorVertices);

		testVertices.Clear();
		MeshHelper::CreateSphereForParticle(Vector3(0.5f, 0.5f, 0.5f),4, colors, testVertices);

		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);
		Application::GetResourceContainer()->LoadMesh("SphereForParticle_UV_Normal_min", uv_normalVertices);
	}

	//Application::GetResourceContainer()->Reload();



	{
	std::vector<std::pair<std::string, std::string>> vec_pixelShaderPath = {
		{"VertexUVNormalColorMeshPS", "Shader/Compiled/"},
		{"VertexUVColorMeshPS", "Shader/Compiled/"},
		{"GlidPS", "Shader/Compiled/"},
		{"AmbientPS", "Shader/Compiled/"},
		{"DefaultMeshPS", "Shader/Compiled/"},
	};
	Application::GetResourceContainer()->LoadPixelShader(vec_pixelShaderPath);

	std::vector<std::pair<std::string, std::string>> vec_vertexShaderPath = {
		{"SingleBoneVS", "Shader/Compiled/"},
		{"QuadBoneVS", "Shader/Compiled/"},
		{"PMXVS", "Shader/Compiled/"},
		{"PMX_AttributeVS", "Shader/Compiled/"},
		{"DefaultMeshVS", "Shader/Compiled/"},
		{"VertexUVMeshVS", "Shader/Compiled/"},
		{"VertexUVNormalColorMeshVS", "Shader/Compiled/"},
		{"VertexUVNormalAttributeVS", "Shader/Compiled/"},
		{"ParticleVS", "Shader/Compiled/"},
		{"Particle3DVS", "Shader/Compiled/"},
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
		{"OnlyMaterial","DefaultMeshVS","AmbientPS", "Shader/Compiled/", "Shader/Compiled/"},
		{"GSMesh","VertexUVNormalAttributeVS","DefaultMeshPS", "Shader/Compiled/", "Shader/Compiled/","TestGS","Shader/Compiled/"},
		{"SingleBoneModel","SingleBoneVS","DefaultMeshPS", "Shader/Compiled/", "Shader/Compiled/"},
		{"PMXModel","PMXVS","DefaultMeshPS", "Shader/Compiled/", "Shader/Compiled/"},
		{"PMXModel_GS","PMX_AttributeVS","DefaultMeshPS", "Shader/Compiled/", "Shader/Compiled/","TestGS","Shader/Compiled/"},
		{"QuadModel","QuadBoneVS","DefaultMeshPS", "Shader/Compiled/", "Shader/Compiled/"},
		{"Glid","VertexUVMeshVS","GlidPS", "Shader/Compiled/", "Shader/Compiled/"},

		{ "GSParticle_Standard","ParticleVS","VertexUVColorMeshPS", "Shader/Compiled/", "Shader/Compiled/","PointToPlaneGS","Shader/Compiled/" },
		
		{ "GSParticle_Cube","Particle3DVS","VertexUVNormalColorMeshPS", "Shader/Compiled/", "Shader/Compiled/","PointToCubeGS","Shader/Compiled/" },
		
		{ "GSParticle_Pyramid","Particle3DVS","VertexUVNormalColorMeshPS", "Shader/Compiled/", "Shader/Compiled/","PointToPyramidGS","Shader/Compiled/" },
		{ "ImmdeiateParticle_Plane","ImmediateParticleVS","VertexUVColorMeshPS", "Shader/Compiled/", "Shader/Compiled/","PointToPlaneGS","Shader/Compiled/" },
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
	};

	Application::GetResourceContainer()->LoadModel(vec_modelPath);

	std::vector<std::pair<std::string, std::string>> vec_motionPath = {
		{"bakutyuu.bmd","Motion/"},
		{"slash.bmd","Motion/"},
		{"ojigi.bmd","Motion/"},
	};

	Application::GetResourceContainer()->LoadMotion(vec_motionPath);

	std::vector<std::pair<std::string, std::string>> vec_texturePath = {
		{"black.png","Texture/"},
		{"sorena.png","Texture/"},
		{"haikei.png","Texture/"},
	};



	auto vec_textureTag = Application::GetResourceContainer()->LoadTexture(vec_texturePath);


	std::vector<std::pair<std::string, std::string>> vec_materialPath = {
		{"childMaterial.bma", "Material/"},
		{"particleMaterial.bma", "Material/"},
	};

	Application::GetResourceContainer()->LoadMaterial(vec_materialPath);

	//MaterialVariable materialVar;

	//Application::GetResourceContainer()->LoadMaterial(materialVar, vec_textureTag.at(2), "sorena");
	//Application::GetResourceContainer()->LoadMaterial(materialVar,vec_textureTag.at(3),"haikei");

	std::vector<std::pair<std::string, std::string>> vec_soundPath = {
		{"se_maoudamashii_jingle13.wav","Sound/"},
		{"bullet_hit.wav","Sound/"},
	};

	Application::GetResourceContainer()->LoadSound(vec_soundPath);
}
}

void ButiEngine::Application::Exit()
{
	shp_sceneManager->Release();
	unq_window->Release();
	OutputCereal(shp_resourceContainer);
	shp_resourceContainer->Release();
	unq_imguiController->Release();
	shp_graphicDevice->Release();
}