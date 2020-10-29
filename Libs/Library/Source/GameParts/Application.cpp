#pragma once
#include "stdafx.h"
#include"Header/Resources/Vertex.h"
#include"Header/Device/Helper/MeshHelper.h"
#include"Header/GameParts/ResourceContainer.h"
#include"Header/Device/ModelFileConverter.h"

std::string ButiEngine::GlobalSettings::resourceDirectory = "Resources/";
ButiEngine::Matrix4x4 ButiEngine::Transform::x90Rotate;
void ButiEngine::Application::CreateInstances()
{
	if (!unq_window) {
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
		unq_window = std::make_unique<Window>();
		unq_window->Initialize(720, 480);
	}

	shp_graphicDevice = ObjectFactory::Create<GraphicDevice_Dx12>(GetThis<Application>());
	
	if (!shp_resourceContainer) {
		shp_resourceContainer = ObjectFactory::Create<ResourceContainer>(shp_graphicDevice);
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
}

std::unique_ptr<ButiEngine::Window>& ButiEngine::Application::GetWindow()
{
	return unq_window;
}

void ButiEngine::Application::InitLoadResources()
{
	{
		BackupData<Vertex::Vertex_UV_Normal> uv_normalVertices;
		BackupData<Vertex::Vertex_UV_Normal_Color> testVertices;


		std::vector<Color> colors;


		MeshHelper::CreateCapsule(Vector3(0.5f, 0.5f, 0.5f), Vector3(0, 0, 0), Vector3(0, 0, 0), 12, false, colors, testVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);
		Application::GetResourceContainer()->LoadMesh("TestCapsule", uv_normalVertices);

		MeshHelper::CreateCube(Vector3(0.5f, 0.5f, 0.5f), colors, testVertices, true);

		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);

		Application::GetResourceContainer()->LoadMesh("TestCube", uv_normalVertices);


		MeshHelper::CreatePlane(Vector2(1, 1), colors, false, testVertices);
		Vertex::VertexHelper::VertexConvert(testVertices, uv_normalVertices);

		Application::GetResourceContainer()->LoadMesh("Floor", uv_normalVertices);
		testVertices.Clear();

		uv_normalVertices.Clear();
	}

	std::vector<std::pair<std::string, std::string>> vec_pixelShaderPath = {
		{"VertexUVNormalColorMeshPS", "Shader/Compiled/"},
		{"GlidPS", "Shader/Compiled/"},
		{"AmbientPS", "Shader/Compiled/"},
		{"DefaultMeshPS", "Shader/Compiled/"},
		{"HalfAlphaPS", "Shader/Compiled/"},
	};
	Application::GetResourceContainer()->LoadPixelShader(vec_pixelShaderPath);

	std::vector<std::pair<std::string, std::string>> vec_vertexShaderPath = {
		{"SingleBoneVS", "Shader/Compiled/"},
		{"QuadBoneVS", "Shader/Compiled/"},
		{"PMXVS", "Shader/Compiled/"},
		{"PMX_AttributeVS", "Shader/Compiled/"},
		{"LookAtInverseMeshVS", "Shader/Compiled/"},
		{"DefaultMeshVS", "Shader/Compiled/"},
		{"VertexUVMeshVS", "Shader/Compiled/"},
		{"VertexUVNormalColorMeshVS", "Shader/Compiled/"},
		{"VertexUVNormalAttributeVS", "Shader/Compiled/"},
	};
	Application::GetResourceContainer()->LoadVertexShader(vec_vertexShaderPath);
	
	std::vector<std::pair<std::string, std::string>> vec_geometryShaderPath = {
		{"TestGS", "Shader/Compiled/"},
		{"PointToPolygonGS", "Shader/Compiled/"},
	};
	Application::GetResourceContainer()->LoadGeometryShader(vec_geometryShaderPath);

	std::vector<ResourceContainer::ShaderName> vec_names = {
		{"DefaultMesh","DefaultMeshVS","DefaultMeshPS", "Shader/Compiled/", "Shader/Compiled/"},
		{"DefaultMesh_test","VertexUVNormalAttributeVS","DefaultMeshPS", "Shader/Compiled/", "Shader/Compiled/","PointToPolygonGS","Shader/Compiled/"},
		{"BillBoardMesh","LookAtInverseMeshVS","DefaultMeshPS", "Shader/Compiled/", "Shader/Compiled/"},
		{"OnlyMaterial","DefaultMeshVS","AmbientPS", "Shader/Compiled/", "Shader/Compiled/"},
		{"HalfAlphaPMX","PMXVS","HalfAlphaPS", "Shader/Compiled/", "Shader/Compiled/"},
		{"GSMesh","VertexUVNormalAttributeVS","DefaultMeshPS", "Shader/Compiled/", "Shader/Compiled/","TestGS","Shader/Compiled/"},
		{"SingleBoneModel","SingleBoneVS","DefaultMeshPS", "Shader/Compiled/", "Shader/Compiled/"},
		{"PMXModel","PMXVS","DefaultMeshPS", "Shader/Compiled/", "Shader/Compiled/"},
		{"PMXModel_GS","PMX_AttributeVS","DefaultMeshPS", "Shader/Compiled/", "Shader/Compiled/","TestGS","Shader/Compiled/"},
		{"QuadModel","QuadBoneVS","DefaultMeshPS", "Shader/Compiled/", "Shader/Compiled/"},
		{"Glid","DefaultMeshVS","GlidPS", "Shader/Compiled/", "Shader/Compiled/"},
	}; 

	Application::GetResourceContainer()->LoadShader(vec_names);



	//ModelFileConverter::FBXtoB3M("rock.fbx", "rock.b3m", "Model/FBX/");
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
		//{"maguro.b3m", "Model/Maguro/"},
		//{"gun.b3m", "Model/Maguro/"},
	};

	Application::GetResourceContainer()->LoadModel(vec_modelPath);

	std::vector<std::pair<std::string, std::string>> vec_motionPath = {
		{"bakutyuu.bmd","Motion/"},
		{"slash.bmd","Motion/"},
		{"ojigi.bmd","Motion/"},
	};

	Application::GetResourceContainer()->LoadMotion(vec_motionPath);

	std::vector<std::pair<std::string, std::string>> vec_texturePath = {
		{"particle.png","Texture/"},
		{"black.png","Texture/"},
		{"sorena.png","Texture/"},
		{"haikei.png","Texture/"},
	};



	auto vec_textureTag= Application::GetResourceContainer()->LoadTexture(vec_texturePath);


	std::vector<std::pair<std::string, std::string>> vec_materialPath = {
		{"blueMaterial.bma", "Material/"},
		{"redMaterial.bma", "Material/"},
		{"yellowMaterial.bma", "Material/"},
	};

	Application::GetResourceContainer()->LoadMaterial(vec_materialPath);

	MaterialVariable materialVar;

	Application::GetResourceContainer()->LoadMaterial(materialVar, vec_textureTag.at(2), "sorena");
	Application::GetResourceContainer()->LoadMaterial(materialVar,vec_textureTag.at(3),"haikei");

	std::vector<std::pair<std::string, std::string>> vec_soundPath = {
		{"se_maoudamashii_jingle13.wav","Sound/"},
		{"bullet_hit.wav","Sound/"},
	};

	Application::GetResourceContainer()->LoadSound(vec_soundPath);


}

void ButiEngine::Application::Exit()
{
	shp_sceneManager->Release();
	unq_window->Release();
	shp_resourceContainer->Release();
	shp_graphicDevice->Release();
}