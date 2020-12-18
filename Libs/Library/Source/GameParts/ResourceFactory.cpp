#include"stdafx.h"

#include"Header/GameParts/ResourceContainer.h"
#include "..\..\Header\GameParts\ResourceFactory.h"
ButiEngine::ResourceFactory::TextureResourceData ButiEngine::ResourceFactory::GetTextureResourceDataFromFile(const std::string& filePath)
{
	TextureResourceData output;
	Microsoft::WRL::ComPtr<IWICBitmapDecoder>decoder = nullptr;
	std::wstring wstrPath;
	wstrPath = Util::StringToWString(filePath);
	auto hr = wkp_graphicDevice.lock()->GetImageFactory().CreateDecoderFromFilename(wstrPath.c_str(), 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, decoder.GetAddressOf());

	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode>frame = nullptr;
	decoder->GetFrame(0, &frame);

	UINT width, height;
	frame->GetSize(&width, &height);

	WICPixelFormatGUID pixelFormat;
	frame->GetPixelFormat(&pixelFormat);
	std::vector<BYTE>buffer;
	buffer.resize(width * height * 4);

	if (pixelFormat != GUID_WICPixelFormat32bppBGRA) {
		Microsoft::WRL::ComPtr<IWICFormatConverter>formatConverter = nullptr;
		wkp_graphicDevice.lock()->GetImageFactory().CreateFormatConverter(&formatConverter);

		formatConverter->Initialize(frame.Get(), GUID_WICPixelFormat32bppBGRA, WICBitmapDitherTypeErrorDiffusion, 0, 0, WICBitmapPaletteTypeCustom);
		formatConverter->CopyPixels(0, width * 4, width * height * 4, buffer.data());

	}
	else
	{
		frame->CopyPixels(0, width * 4, width * height * 4, buffer.data());
	}
	output.width = width;
	output.height = height;
	output.rawData = buffer;
	return output;
}

std::shared_ptr<ButiEngine::Resource_Material> ButiEngine::ResourceFactory::CreateMaterial(const std::string& arg_materialPath, const std::string& arg_fileDirectory)
{
	MaterialVariable var;
	BinaryReader materialReader;
	auto fullPath = GlobalSettings::GetResourceDirectory() + StringHelper::GetDirectory(arg_fileDirectory) + arg_materialPath;
	materialReader.ReadStart(fullPath);
	std::vector< TextureTag> textureTags;
	std::string magic = materialReader.ReadCharactor(16);
	if (magic != "ButiMaterialData") {
		std::cout << "ファイルが違います" << std::endl;
		throw ButiException(L"file type is wrong", L"", L"");
	}

	float version = materialReader.ReadVariable<float>();

	//0でstd::wstring ,1でstd::string
	char encodeType = materialReader.ReadVariable<char>();

	std::string materialName;
	std::wstring materialNameW;
	//マテリアル名
	{
		auto materialNameCount = materialReader.ReadVariable<int>();
		if (encodeType) {
			materialName = materialReader.ReadCharactor(materialNameCount);

		}
		else
		{
			materialNameW = materialReader.ReadWCharactor(materialNameCount);
		}
	}
	std::string materialEngName;
	std::wstring materialEngNameW;
	//マテリアル名英
	{
		auto materialNameCount = materialReader.ReadVariable<int>();
		if (encodeType) {
			materialEngName = materialReader.ReadCharactor(materialNameCount);
		}
		else
		{
			materialEngNameW = materialReader.ReadWCharactor(materialNameCount);
		}
	}
	//テクスチャ
	char textureCount = materialReader.ReadVariable<char>();
	auto resourceContainer = wkp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer();
	for (int i = 0; i < textureCount; i++) {
		auto texNameCount = materialReader.ReadVariable<int>();
		if (texNameCount < 0) {
			textureTags.push_back(resourceContainer->LoadTexture("black.png", "Texture/"));
		}
		else if (encodeType) {
			std::string textureName = materialReader.ReadCharactor(texNameCount);

			textureTags.push_back(resourceContainer->LoadTexture(textureName,StringHelper::GetDirectory( arg_fileDirectory+arg_materialPath)+"../"));
		}
		else
		{
			std::wstring textureNameW = materialReader.ReadWCharactor(texNameCount);
			std::string textureName;
			StringHelper::WStringToSafetyConvert(textureNameW);
			textureName = Util::WStringToString(textureNameW);
			textureTags.push_back(resourceContainer->LoadTexture(textureName, StringHelper::GetDirectory(arg_fileDirectory + arg_materialPath)+"../"));
		}
	}

	//マテリアル
	var.diffuse = materialReader.ReadVariable<Vector4>();
	var.specular = materialReader.ReadVariable<Vector4>();
	var.ambient = materialReader.ReadVariable<Vector4>();
	var.emissive = materialReader.ReadVariable<Vector4>();

	char byteFlag = materialReader.ReadVariable<char>();
	char sphereFlag = materialReader.ReadVariable<char>();


	//コメント
	std::string comment;
	std::wstring commentW;
	{
		auto materialNameCount = materialReader.ReadVariable<int>();
		if (encodeType) {
			comment = materialReader.ReadCharactor(materialNameCount);
		}
		else
		{
			commentW = materialReader.ReadWCharactor(materialNameCount);
		}
	}

	materialReader.ReadEnd();
	auto material = CreateMaterial(var, textureTags);

	material->SetMaterialName(materialName);
	material->SetMaterialNameEng(materialEngName);
	material->SetComment(comment);


	return material;
}

std::shared_ptr<ButiEngine::Resource_Model> ButiEngine::ResourceFactory::CreateModel(const std::string& modelPath,const std::string& fileDirectory)
{


	auto resource_model = ObjectFactory::Create<Resource_Model>();
	BinaryReader modelReader;
	modelReader.ReadStart(GlobalSettings::GetResourceDirectory() + fileDirectory + modelPath);

	{
		auto headerStr = modelReader.ReadCharactor(15);
		if (headerStr != "Buti3DModelData") {
			std::cout << "ファイルが違います\n";
			return nullptr;
		}
	}

	float version = modelReader.ReadVariable<float>();

	resource_model->SetVersion(version);

	//0でstd::wstring ,1でstd::string
	char encodeType = modelReader.ReadVariable<char>();

	int textCount = modelReader.ReadVariable<int>();
	//制作者の名前の読み込み
	{

		std::wstring nameWStr;
		if (encodeType) {
			Util::MultiBytetoWString(modelReader.ReadCharactor(textCount), nameWStr);

		}
		else {

			nameWStr = modelReader.ReadWCharactor(textCount);
		}
		resource_model->SetName(nameWStr);
	}

	//制作者の名前の読み込み(eng)
	{
		textCount = modelReader.ReadVariable<int>();
		std::wstring nameWStr;
		if (encodeType) {
			Util::MultiBytetoWString(modelReader.ReadCharactor(textCount), nameWStr);

		}
		else {

			nameWStr = modelReader.ReadWCharactor(textCount);
		}
		resource_model->SetEngName(nameWStr);
	}
	//モデル名
	{
		textCount = modelReader.ReadVariable<int>();
		std::wstring nameWStr;
		if (encodeType) {
			Util::MultiBytetoWString(modelReader.ReadCharactor(textCount), nameWStr);

		}
		else {

			nameWStr = modelReader.ReadWCharactor(textCount);
		}
		resource_model->SetModelName(nameWStr);
	}
	//モデル名英
	{
		textCount = modelReader.ReadVariable<int>();
		std::wstring nameWStr;
		if (encodeType) {
			Util::MultiBytetoWString(modelReader.ReadCharactor(textCount), nameWStr);

		}
		else {

			nameWStr = modelReader.ReadWCharactor(textCount);
		}
		resource_model->SetEngModelName(nameWStr);
	}
	//コメント
	{
		textCount = modelReader.ReadVariable<int>(); std::wstring nameWStr;
		if (encodeType) {
			Util::MultiBytetoWString(modelReader.ReadCharactor(textCount), nameWStr);

		}
		else {

			nameWStr = modelReader.ReadWCharactor(textCount);
		}
		resource_model->SetComment(nameWStr);
	}
	//コメント英
	{
		textCount = modelReader.ReadVariable<int>(); std::wstring nameWStr;
		if (encodeType) {
			Util::MultiBytetoWString(modelReader.ReadCharactor(textCount), nameWStr);

		}
		else {

			nameWStr = modelReader.ReadWCharactor(textCount);
		}
		resource_model->SetEngComment(nameWStr);
	}
	//頂点インデックスのバイト数
	char vertexIndexByteSize = modelReader.ReadVariable<char>();
	//マテリアルインデックスのバイト数
	char materialIndexByteSize = modelReader.ReadVariable<char>();
	//ボーンインデックスのバイト数
	char boneIndexByteCount = modelReader.ReadVariable<char>();
	//モーフインデックスのバイト数
	char morphIndexByteSize = modelReader.ReadVariable<char>();

	//頂点構造体の列挙型
	Vertex::VertexType type = (Vertex::VertexType)modelReader.ReadVariable<char>();

	char uvExCount = modelReader.ReadVariable<char>();



	UINT vertexCount = modelReader.ReadUINT(vertexIndexByteSize);

	MeshTag meshTag;
	auto resourceContainer = wkp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer()->GetThis<ResourceContainer>();
	switch (type)
	{
	case Vertex::VertexType::Vertex_UV_Normal:
	{
		switch (uvExCount)
		{
		case 0: {
			BackupData<Vertex::Vertex_UV_Normal> vec_vertex;
			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);
			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);
			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 1: {
			BackupData<Vertex::Vertex_UV_Normal_UVex01> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 2: {
			BackupData<Vertex::Vertex_UV_Normal_UVex02> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 3: {
			BackupData<Vertex::Vertex_UV_Normal_UVex03> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 4: {
			BackupData<Vertex::Vertex_UV_Normal_UVex04> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		default:
			break;
		}
	}
	break;
	case Vertex::VertexType::Vertex_Model_SingleBone:
	{
		switch (uvExCount)
		{
		case 0: {
			BackupData<Vertex::ModelVertex::Vertex_Model_SingleBone> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 1: {
			BackupData<Vertex::ModelVertex::Vertex_Model_SingleBone_UVex01> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 2: {
			BackupData<Vertex::ModelVertex::Vertex_Model_SingleBone_UVex02> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 3: {
			BackupData<Vertex::ModelVertex::Vertex_Model_SingleBone_UVex03> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 4: {
			BackupData<Vertex::ModelVertex::Vertex_Model_SingleBone_UVex04> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		default:
			break;
		}




	}
	break;
	case Vertex::VertexType::Vertex_Model_DoubleBone:
	{
		switch (uvExCount)
		{
		case 0: {
			BackupData<Vertex::ModelVertex::Vertex_Model_DoubleBone> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 1: {
			BackupData<Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex01> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 2: {
			BackupData<Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex02> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 3: {
			BackupData<Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex03> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 4: {
			BackupData<Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex04> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		default:
			break;
		}


	}
	break;
	case Vertex::VertexType::Vertex_Model_QuadBone:
	{

		switch (uvExCount)
		{
		case 0: {
			BackupData<Vertex::ModelVertex::Vertex_Model_QuadBone> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 1: {
			BackupData<Vertex::ModelVertex::Vertex_Model_QuadBone_UVex01> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 2: {
			BackupData<Vertex::ModelVertex::Vertex_Model_QuadBone_UVex02> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 3: {
			BackupData<Vertex::ModelVertex::Vertex_Model_QuadBone_UVex03> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 4: {
			BackupData<Vertex::ModelVertex::Vertex_Model_QuadBone_UVex04> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		default:
			break;
		}
	}
	break;

	case Vertex::VertexType::Vertex_Model_SDEFBone:
	{

		switch (uvExCount)
		{
		case 0: {
			BackupData<Vertex::ModelVertex::Vertex_Model_SdefBone> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 1: {
			BackupData<Vertex::ModelVertex::Vertex_Model_SdefBone_UVex01> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 2: {
			BackupData<Vertex::ModelVertex::Vertex_Model_SdefBone_UVex02> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 3: {
			BackupData<Vertex::ModelVertex::Vertex_Model_SdefBone_UVex03> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		case 4: {
			BackupData<Vertex::ModelVertex::Vertex_Model_SdefBone_UVex04> vec_vertex;

			BoxSurface boxSurface;
			ReadVertex(vec_vertex.vertices, vertexCount, modelReader, boxSurface);
			vec_vertex.SetBoxSurfaces(boxSurface);

			std::vector<UINT> indices;
			ReadIndex(indices, vertexIndexByteSize, modelReader);

			vec_vertex.indices = indices;
			meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
		}
			  break;
		default:
			break;
		}

	}

	break;
	case Vertex::VertexType::Vertex_Model_Mix:
	{switch (uvExCount)
	{
	case 0: {
		BackupData<Vertex::ModelVertex::Vertex_Model_PMX> vec_vertex;
		vec_vertex.vertices.resize(vertexCount);
		for (UINT i = 0; i < vertexCount; i++) {
			Vertex::ModelVertex::Vertex_Model_PMX vertex;


			auto vertexType = (Vertex::VertexType)(modelReader.ReadVariable<char>());


			switch (vertexType)
			{
			case Vertex::VertexType::Vertex_Model_SingleBone: {

				Vertex::ModelVertex::Vertex_Model_SingleBone copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_SingleBone>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex;

				vertex.weight_01 = 1.0f;
				vertex.weight_02 = -1;

				vertex.weight_03 = -1;
				vertex.weight_04 = -1;
			}
															break;
			case Vertex::VertexType::Vertex_Model_DoubleBone: {

				Vertex::ModelVertex::Vertex_Model_DoubleBone copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_DoubleBone>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex_1;

				vertex.boneIndex_2 = copyVertex.boneIndex_2;

				vertex.weight_01 = copyVertex.weight;
				vertex.weight_02 = 1 - vertex.weight_01;

				vertex.weight_03 = -1;
				vertex.weight_04 = -1;

			}
															break;
			case Vertex::VertexType::Vertex_Model_QuadBone: {
				Vertex::ModelVertex::Vertex_Model_QuadBone copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_QuadBone>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex_1;

				vertex.boneIndex_2 = copyVertex.boneIndex_2;

				vertex.boneIndex_3 = copyVertex.boneIndex_3;


				vertex.boneIndex_4 = copyVertex.boneIndex_4;


				vertex.weight_01 = copyVertex.weight_01;
				vertex.weight_02 = copyVertex.weight_02;
				vertex.weight_03 = copyVertex.weight_03;
				vertex.weight_04 = copyVertex.weight_04;
			}
														  break;
			case Vertex::VertexType::Vertex_Model_SDEFBone: {
				Vertex::ModelVertex::Vertex_Model_SdefBone copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_SdefBone>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex_1;

				vertex.boneIndex_2 = copyVertex.boneIndex_2;


				vertex.weight_01 = copyVertex.weight;
				vertex.weight_02 = 1 - vertex.weight_01;
				vertex.weight_03 = -1;
				vertex.weight_04 = -1;


				vertex.SDEF_C = copyVertex.SDEF_C;

				vertex.SDEF_R0 = copyVertex.SDEF_R0;

				vertex.SDEF_R1 = copyVertex.SDEF_R1;
			}
														  break;
			default:
				break;
			}

			vec_vertex.vertices.at(i) = vertex;
		}
		std::vector<UINT> indices;
		ReadIndex(indices, vertexIndexByteSize, modelReader);
		vec_vertex.indices = indices;
		meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
	}
		  break;
	case 1: {
		BackupData<Vertex::ModelVertex::Vertex_Model_PMX_UVex01> vec_vertex;
		vec_vertex.vertices.resize(vertexCount);
		for (UINT i = 0; i < vertexCount; i++) {
			Vertex::ModelVertex::Vertex_Model_PMX_UVex01 vertex;


			auto vertexType = (Vertex::VertexType)(modelReader.ReadVariable<char>());


			switch (vertexType)
			{
			case Vertex::VertexType::Vertex_Model_SingleBone: {

				Vertex::ModelVertex::Vertex_Model_SingleBone_UVex01 copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_SingleBone_UVex01>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.SetUV_ex1(copyVertex.GetUV_ex1());
				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex;

				vertex.weight_01 = 1.0f;
				vertex.weight_02 = -1;

				vertex.weight_03 = -1;
				vertex.weight_04 = -1;
			}
															break;
			case Vertex::VertexType::Vertex_Model_DoubleBone: {

				Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex01 copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex01>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.SetUV_ex1(copyVertex.GetUV_ex1());
				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex_1;

				vertex.boneIndex_2 = copyVertex.boneIndex_2;

				vertex.weight_01 = copyVertex.weight;
				vertex.weight_02 = 1 - vertex.weight_01;

				vertex.weight_03 = -1;
				vertex.weight_04 = -1;

			}
															break;
			case Vertex::VertexType::Vertex_Model_QuadBone: {
				Vertex::ModelVertex::Vertex_Model_QuadBone_UVex01 copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_QuadBone_UVex01>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.SetUV_ex1(copyVertex.GetUV_ex1());
				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex_1;

				vertex.boneIndex_2 = copyVertex.boneIndex_2;

				vertex.boneIndex_3 = copyVertex.boneIndex_3;


				vertex.boneIndex_4 = copyVertex.boneIndex_4;

				vertex.weight_01 = copyVertex.weight_01;
				vertex.weight_02 = copyVertex.weight_02;
				vertex.weight_03 = copyVertex.weight_03;
				vertex.weight_04 = copyVertex.weight_04;
			}
														  break;
			case Vertex::VertexType::Vertex_Model_SDEFBone: {
				Vertex::ModelVertex::Vertex_Model_SdefBone_UVex01 copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_SdefBone_UVex01>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.SetUV_ex1(copyVertex.GetUV_ex1());

				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex_1;

				vertex.boneIndex_2 = copyVertex.boneIndex_2;


				vertex.weight_01 = copyVertex.weight;
				vertex.weight_02 = 1 - vertex.weight_01;
				vertex.weight_03 = -1;
				vertex.weight_04 = -1;


				vertex.SDEF_C = copyVertex.SDEF_C;

				vertex.SDEF_R0 = copyVertex.SDEF_R0;

				vertex.SDEF_R1 = copyVertex.SDEF_R1;
			}
														  break;
			default:
				break;
			}


			vec_vertex.vertices.at(i) = vertex;
		}
		std::vector<UINT> indices;
		ReadIndex(indices, vertexIndexByteSize, modelReader);
		vec_vertex.indices = indices;
		meshTag = resourceContainer->GetThis<ResourceContainer>()-> LoadMesh(modelPath, vec_vertex);
	}
		  break;
	case 2: {
		BackupData<Vertex::ModelVertex::Vertex_Model_PMX_UVex02> vec_vertex;
		vec_vertex.vertices.resize(vertexCount);
		for (UINT i = 0; i < vertexCount; i++) {
			Vertex::ModelVertex::Vertex_Model_PMX_UVex02 vertex;


			auto vertexType = (Vertex::VertexType)(modelReader.ReadVariable<char>());


			switch (vertexType)
			{
			case Vertex::VertexType::Vertex_Model_SingleBone: {

				Vertex::ModelVertex::Vertex_Model_SingleBone_UVex02 copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_SingleBone_UVex02>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.SetUV_ex1(copyVertex.GetUV_ex1());
				vertex.SetUV_ex2(copyVertex.GetUV_ex2());
				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex;

				vertex.weight_01 = 1.0f;
				vertex.weight_02 = -1;

				vertex.weight_03 = -1;
				vertex.weight_04 = -1;
			}
															break;
			case Vertex::VertexType::Vertex_Model_DoubleBone: {

				Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex02 copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex02>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.SetUV_ex1(copyVertex.GetUV_ex1());
				vertex.SetUV_ex2(copyVertex.GetUV_ex2());
				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex_1;

				vertex.boneIndex_2 = copyVertex.boneIndex_2;

				vertex.weight_01 = copyVertex.weight;
				vertex.weight_02 = 1 - vertex.weight_01;

				vertex.weight_03 = -1;
				vertex.weight_04 = -1;

			}
															break;
			case Vertex::VertexType::Vertex_Model_QuadBone: {
				Vertex::ModelVertex::Vertex_Model_QuadBone_UVex02 copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_QuadBone_UVex02>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.SetUV_ex1(copyVertex.GetUV_ex1());
				vertex.SetUV_ex2(copyVertex.GetUV_ex2());
				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex_1;

				vertex.boneIndex_2 = copyVertex.boneIndex_2;

				vertex.boneIndex_3 = copyVertex.boneIndex_3;


				vertex.boneIndex_4 = copyVertex.boneIndex_4;

				vertex.weight_01 = copyVertex.weight_01;
				vertex.weight_02 = copyVertex.weight_02;
				vertex.weight_03 = copyVertex.weight_03;
				vertex.weight_04 = copyVertex.weight_04;
			}
														  break;
			case Vertex::VertexType::Vertex_Model_SDEFBone: {
				Vertex::ModelVertex::Vertex_Model_SdefBone_UVex02 copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_SdefBone_UVex02>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.SetUV_ex1(copyVertex.GetUV_ex1());
				vertex.SetUV_ex2(copyVertex.GetUV_ex2());

				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex_1;

				vertex.boneIndex_2 = copyVertex.boneIndex_2;


				vertex.weight_01 = copyVertex.weight;
				vertex.weight_02 = 1 - vertex.weight_01;
				vertex.weight_03 = -1;
				vertex.weight_04 = -1;


				vertex.SDEF_C = copyVertex.SDEF_C;

				vertex.SDEF_R0 = copyVertex.SDEF_R0;

				vertex.SDEF_R1 = copyVertex.SDEF_R1;
			}
														  break;
			default:
				break;
			}


			vec_vertex.vertices.at(i) = vertex;
		}
		std::vector<UINT> indices;
		ReadIndex(indices, vertexIndexByteSize, modelReader);
		vec_vertex.indices = indices;
		meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
	}
		  break;
	case 3: {
		BackupData<Vertex::ModelVertex::Vertex_Model_PMX_UVex03> vec_vertex;
		vec_vertex.vertices.resize(vertexCount);
		for (UINT i = 0; i < vertexCount; i++) {
			Vertex::ModelVertex::Vertex_Model_PMX_UVex03 vertex;


			auto vertexType = (Vertex::VertexType)(modelReader.ReadVariable<char>());


			switch (vertexType)
			{
			case Vertex::VertexType::Vertex_Model_SingleBone: {

				Vertex::ModelVertex::Vertex_Model_SingleBone_UVex03 copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_SingleBone_UVex03>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.SetUV_ex1(copyVertex.GetUV_ex1());
				vertex.SetUV_ex2(copyVertex.GetUV_ex2());
				vertex.SetUV_ex3(copyVertex.GetUV_ex3());
				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex;

				vertex.weight_01 = 1.0f;
				vertex.weight_02 = -1;

				vertex.weight_03 = -1;
				vertex.weight_04 = -1;
			}
															break;
			case Vertex::VertexType::Vertex_Model_DoubleBone: {

				Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex03 copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex03>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.SetUV_ex1(copyVertex.GetUV_ex1());
				vertex.SetUV_ex2(copyVertex.GetUV_ex2());
				vertex.SetUV_ex3(copyVertex.GetUV_ex3());
				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex_1;

				vertex.boneIndex_2 = copyVertex.boneIndex_2;

				vertex.weight_01 = copyVertex.weight;
				vertex.weight_02 = 1 - vertex.weight_01;

				vertex.weight_03 = -1;
				vertex.weight_04 = -1;

			}
															break;
			case Vertex::VertexType::Vertex_Model_QuadBone: {
				Vertex::ModelVertex::Vertex_Model_QuadBone_UVex03 copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_QuadBone_UVex03>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.SetUV_ex1(copyVertex.GetUV_ex1());
				vertex.SetUV_ex2(copyVertex.GetUV_ex2());
				vertex.SetUV_ex3(copyVertex.GetUV_ex3());
				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex_1;

				vertex.boneIndex_2 = copyVertex.boneIndex_2;

				vertex.boneIndex_3 = copyVertex.boneIndex_3;


				vertex.boneIndex_4 = copyVertex.boneIndex_4;

				vertex.weight_01 = copyVertex.weight_01;
				vertex.weight_02 = copyVertex.weight_02;
				vertex.weight_03 = copyVertex.weight_03;
				vertex.weight_04 = copyVertex.weight_04;
			}
														  break;
			case Vertex::VertexType::Vertex_Model_SDEFBone: {
				Vertex::ModelVertex::Vertex_Model_SdefBone_UVex03 copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_SdefBone_UVex03>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.SetUV_ex1(copyVertex.GetUV_ex1());
				vertex.SetUV_ex2(copyVertex.GetUV_ex2());
				vertex.SetUV_ex3(copyVertex.GetUV_ex3());

				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex_1;

				vertex.boneIndex_2 = copyVertex.boneIndex_2;


				vertex.weight_01 = copyVertex.weight;
				vertex.weight_02 = 1 - vertex.weight_01;
				vertex.weight_03 = -1;
				vertex.weight_04 = -1;


				vertex.SDEF_C = copyVertex.SDEF_C;

				vertex.SDEF_R0 = copyVertex.SDEF_R0;

				vertex.SDEF_R1 = copyVertex.SDEF_R1;
			}
														  break;
			default:
				break;
			}


			vec_vertex.vertices.at(i) = vertex;
		}
		std::vector<UINT> indices;
		ReadIndex(indices, vertexIndexByteSize, modelReader);
		meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
	}
		  break;
	case 4: {
		BackupData<Vertex::ModelVertex::Vertex_Model_PMX_UVex04> vec_vertex;
		vec_vertex.vertices.resize(vertexCount);
		for (UINT i = 0; i < vertexCount; i++) {
			Vertex::ModelVertex::Vertex_Model_PMX_UVex04 vertex;


			auto vertexType = (Vertex::VertexType)(modelReader.ReadVariable<char>());


			switch (vertexType)
			{
			case Vertex::VertexType::Vertex_Model_SingleBone: {

				Vertex::ModelVertex::Vertex_Model_SingleBone_UVex04 copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_SingleBone_UVex04>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.SetUV_ex1(copyVertex.GetUV_ex1());
				vertex.SetUV_ex2(copyVertex.GetUV_ex2());
				vertex.SetUV_ex3(copyVertex.GetUV_ex3());
				vertex.SetUV_ex4(copyVertex.GetUV_ex4());
				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex;

				vertex.weight_01 = 1.0f;
				vertex.weight_02 = -1;

				vertex.weight_03 = -1;
				vertex.weight_04 = -1;
			}
															break;
			case Vertex::VertexType::Vertex_Model_DoubleBone: {

				Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex04 copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex04>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.SetUV_ex1(copyVertex.GetUV_ex1());
				vertex.SetUV_ex2(copyVertex.GetUV_ex2());
				vertex.SetUV_ex3(copyVertex.GetUV_ex3());
				vertex.SetUV_ex4(copyVertex.GetUV_ex4());
				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex_1;

				vertex.boneIndex_2 = copyVertex.boneIndex_2;

				vertex.weight_01 = copyVertex.weight;
				vertex.weight_02 = 1 - vertex.weight_01;

				vertex.weight_03 = -1;
				vertex.weight_04 = -1;

			}
															break;
			case Vertex::VertexType::Vertex_Model_QuadBone: {
				Vertex::ModelVertex::Vertex_Model_QuadBone_UVex04 copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_QuadBone_UVex04>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.SetUV_ex1(copyVertex.GetUV_ex1());
				vertex.SetUV_ex2(copyVertex.GetUV_ex2());
				vertex.SetUV_ex3(copyVertex.GetUV_ex3());
				vertex.SetUV_ex4(copyVertex.GetUV_ex4());
				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex_1;

				vertex.boneIndex_2 = copyVertex.boneIndex_2;

				vertex.boneIndex_3 = copyVertex.boneIndex_3;


				vertex.boneIndex_4 = copyVertex.boneIndex_4;

				vertex.weight_01 = copyVertex.weight_01;
				vertex.weight_02 = copyVertex.weight_02;
				vertex.weight_03 = copyVertex.weight_03;
				vertex.weight_04 = copyVertex.weight_04;
			}
														  break;
			case Vertex::VertexType::Vertex_Model_SDEFBone: {
				Vertex::ModelVertex::Vertex_Model_SdefBone_UVex04 copyVertex = modelReader.ReadVariable<Vertex::ModelVertex::Vertex_Model_SdefBone_UVex04>();

				vertex.position = copyVertex.position;
				vertex.SetUV(copyVertex.GetUV());
				vertex.SetUV_ex1(copyVertex.GetUV_ex1());
				vertex.SetUV_ex2(copyVertex.GetUV_ex2());
				vertex.SetUV_ex3(copyVertex.GetUV_ex3());
				vertex.SetUV_ex4(copyVertex.GetUV_ex4());

				vertex.normal = copyVertex.normal;

				vertex.boneIndex_1 = copyVertex.boneIndex_1;

				vertex.boneIndex_2 = copyVertex.boneIndex_2;


				vertex.weight_01 = copyVertex.weight;
				vertex.weight_02 = 1 - vertex.weight_01;
				vertex.weight_03 = -1;
				vertex.weight_04 = -1;


				vertex.SDEF_C = copyVertex.SDEF_C;

				vertex.SDEF_R0 = copyVertex.SDEF_R0;

				vertex.SDEF_R1 = copyVertex.SDEF_R1;
			}
														  break;
			default:
				break;
			}


			vec_vertex.vertices.at(i) = vertex;
		}
		std::vector<UINT> indices;
		ReadIndex(indices, vertexIndexByteSize, modelReader);
		vec_vertex.indices = indices;
		meshTag = resourceContainer->LoadMesh(modelPath, vec_vertex);
	}
		  break;
	default:
		break;
	}


	}

	break;
	default:
		std::cout << "読み込み失敗" << std::endl;
		break;
	}

	resource_model->SetMeshTag(meshTag);




	//マテリアルの読み込み
	int materialCount;

	materialCount = modelReader.ReadInt(materialIndexByteSize);

	std::vector<UINT> vec_subset;

	vec_subset.resize(materialCount);

	for (int i = 0; i < materialCount; i++) {
		int fileNameCount = modelReader.ReadVariable<int>();
		MaterialTag tag;
		if (encodeType) {
			std::string materialFilePath = modelReader.ReadCharactor(fileNameCount);
			tag = resourceContainer->LoadMaterial(materialFilePath, fileDirectory + StringHelper::GetDirectory(modelPath));
		}
		else
		{
			std::wstring materialFilePath = modelReader.ReadWCharactor(fileNameCount);
			tag = resourceContainer->LoadMaterial(materialFilePath, fileDirectory+StringHelper::GetDirectory(modelPath));
		}
		vec_subset.at(i) = (UINT)(modelReader.ReadVariable<int>());
		resource_model->AddMaterial(tag);
	}
	resource_model->SetSubset(vec_subset);

	//ボーンの読み込み
	int boneCount = modelReader.ReadInt(boneIndexByteCount);

	for (int i = 0; i < boneCount; i++) {
		Bone bone;
		//ボーン名
		{
			int boneNameCount = modelReader.ReadVariable<int>();
			if (encodeType) {
				std::wstring name;
				Util::MultiBytetoWString(modelReader.ReadCharactor(boneNameCount), name);
				bone.boneName = name;
			}
			else
			{
				bone.boneName = modelReader.ReadWCharactor(boneNameCount);
			}
		}
		//ボーン名英
		{
			int boneNameCount = modelReader.ReadVariable<int>();
			if (encodeType) {
				std::wstring name;
				name = Util::StringToWString(modelReader.ReadCharactor(boneNameCount));
				bone.boneNameEng = name;
			}
			else
			{
				bone.boneNameEng = modelReader.ReadWCharactor(boneNameCount);
			}
		}
		bone.parentBoneIndex = modelReader.ReadInt(boneIndexByteCount);
		bone.transformFloor = modelReader.ReadVariable<int>();
		bone.position = modelReader.ReadVariable<Vector3>();

		short boneFlag = (short)modelReader.ReadInt(2);


		if (Util::GetBitFlag(boneFlag, 0)) {
			bone.jointBone = modelReader.ReadInt(boneIndexByteCount);
		}
		else {
			bone.jointPos = modelReader.ReadVariable<Vector3>();
		}

		if (Util::GetBitFlag(boneFlag, 1)) {
			bone.canRotate = true;
		}

		if (Util::GetBitFlag(boneFlag, 2)) {
			bone.canMove = true;
		}

		if (Util::GetBitFlag(boneFlag, 3)) {

		}

		if (Util::GetBitFlag(boneFlag, 4)) {
			bone.canControll = true;
		}


		if (Util::GetBitFlag(boneFlag, 7)) {
			bone.addLocal = true;
		}

		if (Util::GetBitFlag(boneFlag, 8)) {
			bone.addRotate = true;
		}

		if (Util::GetBitFlag(boneFlag, 9)) {

			bone.addMove = true;
		}
		if (bone.addMove || bone.addRotate) {
			bone.addBoneIndex = modelReader.ReadInt(boneIndexByteCount);
			bone.addPercentage = modelReader.ReadVariable<float>();
		}

		if (Util::GetBitFlag(boneFlag, 10)) {
			bone.isFreezeAxis = true;

			bone.freezeAxis = modelReader.ReadVariable<Vector3>();
		}

		if (Util::GetBitFlag(boneFlag, 11)) {
			//localAxis
			bone.localXaxis = modelReader.ReadVariable<Vector3>();
			bone.localXaxis = modelReader.ReadVariable<Vector3>();
		}

		if (Util::GetBitFlag(boneFlag, 12)) {
			bone.isPhisicsAfterTransform = true;
		}

		if (Util::GetBitFlag(boneFlag, 13)) {
			bone.outParentTransform = modelReader.ReadVariable<int>();
		}

		if (Util::GetBitFlag(boneFlag, 5)) {
			//IK
			IKData data;
			data.targetBoneIndex = modelReader.ReadInt(boneIndexByteCount);
			data.loopCount = modelReader.ReadVariable<int>();
			data.maxRadian = modelReader.ReadVariable<float>();

			int ikLinkCount = modelReader.ReadVariable<int>();

			for (int j = 0; j < ikLinkCount; j++) {
				IKLink link;
				link.linkBone = modelReader.ReadInt(boneIndexByteCount);
				char isLimmit = modelReader.ReadVariable<char>();
				if (isLimmit) {
					link.radianLimit = true;
					link.downLimmit = modelReader.ReadVariable<Vector3>();
					link.upLimmit = modelReader.ReadVariable<Vector3>();
				}
				data.links.push_back(link);
			}
			bone.ikDatas.push_back(data);
		}
		resource_model->AddBone(bone);
	}
	//モーフの読み込み

	int morphCount = modelReader.ReadInt(morphIndexByteSize);
	for (int i = 0; i < morphCount; i++) {
		std::wstring morphName;
		std::wstring morphNameEng;
		std::shared_ptr<Morph::Morph> morph;
		int morphNameCount = modelReader.ReadVariable<int>();
		if (encodeType) {
			Util::MultiBytetoWString(modelReader.ReadCharactor(morphNameCount), morphName);
		}
		else
		{
			morphName = modelReader.ReadWCharactor(morphNameCount);
		}
		morphNameCount = modelReader.ReadVariable<int>();
		if (encodeType) {
			Util::MultiBytetoWString(modelReader.ReadCharactor(morphNameCount), morphNameEng);
		}
		else
		{
			morphNameEng = modelReader.ReadWCharactor(morphNameCount);
		}
		char category = modelReader.ReadVariable<char>();
		Morph::MorphType morphType = (Morph::MorphType) modelReader.ReadVariable<char>();

		int offset = modelReader.ReadVariable<int>();

		switch (morphType)
		{
		case Morph::MorphType::Group:
		{
			auto groupMorph = ObjectFactory::Create<Morph::Group_Morph>();
			for (int j = 0; j < offset; j++)
			{
				Morph::Group_Morph::MorphVar var;
				var.morphIndex = modelReader.ReadInt(morphIndexByteSize);
				var.morphPer = modelReader.ReadVariable<float>();
				groupMorph->vec_vars.push_back(var);
			}
			morph = groupMorph;
		}
		break;
		case Morph::MorphType::Vertex:
		{
			auto vertexMorph = ObjectFactory::Create<Morph::Vertex_Morph>();
			for (int j = 0; j < offset; j++)
			{
				Morph::Vertex_Morph::MorphVar var = modelReader.ReadVariable<Morph::Vertex_Morph::MorphVar>();
				vertexMorph->vec_vars.push_back(var);
			}
			morph = vertexMorph;
		}
		break;
		case Morph::MorphType::Bone:
		{
			auto  boneMorph = ObjectFactory::Create<Morph::Bone_Morph>();
			for (int j = 0; j < offset; j++)
			{
				Morph::Bone_Morph::MorphVar var = modelReader.ReadVariable<Morph::Bone_Morph::MorphVar>();
				boneMorph->vec_vars.push_back(var);
			}
			morph = boneMorph;
		}
		break;
		case Morph::MorphType::UV:
		{
			auto uvMorph = ObjectFactory::Create<Morph::UV_Morph>();
			for (int j = 0; j < offset; j++)
			{
				Morph::UV_Morph::MorphVar var = modelReader.ReadVariable<Morph::UV_Morph::MorphVar>();
				uvMorph->vec_vars.push_back(var);
			}
			morph = uvMorph;
		}
		break;
		case Morph::MorphType::UVex01:
		{
			auto uvMorph = ObjectFactory::Create<Morph::UV_Morph>();
			for (int j = 0; j < offset; j++)
			{
				Morph::UV_Morph::MorphVar var = modelReader.ReadVariable<Morph::UV_Morph::MorphVar>();
				uvMorph->vec_vars.push_back(var);
			}
			morph = uvMorph;
		}
		break;
		case Morph::MorphType::UVex02:
		{
			auto uvMorph = ObjectFactory::Create<Morph::UV_Morph>();
			for (int j = 0; j < offset; j++)
			{
				Morph::UV_Morph::MorphVar var = modelReader.ReadVariable<Morph::UV_Morph::MorphVar>();
				uvMorph->vec_vars.push_back(var);
			}
			morph = uvMorph;
		}
		break;
		case Morph::MorphType::UVex03:
		{
			auto uvMorph = ObjectFactory::Create<Morph::UV_Morph>();
			for (int j = 0; j < offset; j++)
			{
				Morph::UV_Morph::MorphVar var = modelReader.ReadVariable<Morph::UV_Morph::MorphVar>();
				uvMorph->vec_vars.push_back(var);
			}
			morph = uvMorph;
		}
		break;
		case Morph::MorphType::UVex04:
		{
			auto uvMorph = ObjectFactory::Create<Morph::UV_Morph>();
			for (int j = 0; j < offset; j++)
			{
				Morph::UV_Morph::MorphVar var = modelReader.ReadVariable<Morph::UV_Morph::MorphVar>();
				uvMorph->vec_vars.push_back(var);
			}
			morph = uvMorph;
		}
		break;
		case Morph::MorphType::Material:
		{
			auto  materialMorph = ObjectFactory::Create<Morph::Material_Morph>();
			for (int j = 0; j < offset; j++)
			{
				Morph::Material_Morph::MorphVar var = modelReader.ReadVariable<Morph::Material_Morph::MorphVar>();
				materialMorph->vec_vars.push_back(var);
			}
			morph = materialMorph;
		}
		break;

		case Morph::MorphType::Flip:
		{
			auto flipMorph = ObjectFactory::Create<Morph::Flip_Morph>();
			for (int j = 0; j < offset; j++)
			{
				Morph::Flip_Morph::MorphVar var = modelReader.ReadVariable<Morph::Flip_Morph::MorphVar>();
				flipMorph->vec_vars.push_back(var);
			}
			morph = flipMorph;
		}
		break;
		default:
			break;
		}
		morph->SetName(morphName);
		morph->SetEngName(morphNameEng);
		resource_model->AddMorph(morph);
	}

	return resource_model;
}

std::shared_ptr<ButiEngine::Resource_Motion> ButiEngine::ResourceFactory::CreateMotion(const std::string& modelPath,const std::string& fileDirectory)
{
	auto output = ObjectFactory::Create<Resource_Motion>();

	BinaryReader bmdReader = BinaryReader();
	bmdReader.ReadStart(GlobalSettings::GetResourceDirectory() + fileDirectory + modelPath);

	auto header = bmdReader.ReadCharactor(14);

	if (header != "ButiMotionData") {
		std::cout << "file is not BMD Format." << std::endl;
		return nullptr;
	}

	float ver = bmdReader.ReadVariable<float>();

	int boneCount = bmdReader.ReadVariable<UINT>();

	for (int i = 0; i < boneCount; i++) {
		//ボーン名
		auto nameCount = bmdReader.ReadVariable<UINT>();
		auto name = bmdReader.ReadWCharactor(nameCount);

		//モーション数
		auto KeyFrameCount = bmdReader.ReadVariable<UINT>();
		std::vector<MotionKeyFrameData> vec_keyFrameData;
		vec_keyFrameData.reserve(KeyFrameCount);
		for (int j = 0; j < KeyFrameCount; j++) {

			vec_keyFrameData.push_back(bmdReader.ReadVariable<MotionKeyFrameData>());

		}

		output->AddKeyFrameLane(name, vec_keyFrameData);
	}

	bmdReader.ReadEnd();
	return output;
}


void ButiEngine::ResourceFactory::ReadIndex(std::vector<UINT>& indices, char arg_indexByteCount, BinaryReader& arg_reader)
{
	//インデックスの読み込み
	int indexCount = arg_reader.ReadVariable<int>();


	indices.resize(indexCount * 3);

	for (UINT i = 0; i < indexCount * 3; i++) {
		indices.at(i) = ((UINT)arg_reader.ReadUINT(arg_indexByteCount));
	}
}