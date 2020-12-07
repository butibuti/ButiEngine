#pragma once
#include "stdafx.h"
#include<limits>
#include"Header/Device/ModelFileConverter.h"
#include"Header/Device/FBXNodeStructure.h"
#include"Header/Resources/Morph.h"
#include "..\..\Header\Device\ModelFileConverter.h"

struct PMXReadStruct {
	std::wstring  modelName;
	std::wstring  modelEngName;
	std::wstring  comment;
	std::wstring  engComment;

};
std::shared_ptr<ButiEngine::FBXAnalyze::FBXNodeStructure> ButiEngine::ModelFileConverter::ReadNodeRecord(BinaryReader& fbxReader,
	std::unordered_multimap<std::string, std::shared_ptr< ButiEngine::FBXAnalyze::FBXNodeStructure>>& arg_vetor_nodeRecord, std::shared_ptr< FBXAnalyze::FBXNodeStructure> parent)
{
	std::shared_ptr<FBXAnalyze::FBXNodeStructure> out;

	UINT endOffset = fbxReader.ReadVariable<UINT>();
	UINT propertyCount = fbxReader.ReadVariable<UINT>();

	UINT propertyListLen = fbxReader.ReadVariable<UINT>();
	UINT nodeNameSize = fbxReader.ReadVariable<UCHAR>();
	std::string recordName = fbxReader.ReadCharactor(nodeNameSize);

	if (endOffset == 0x9abbcfa || endOffset == 0x08afbcfa) {
		out = ObjectFactory::Create<FBXAnalyze::FBXNode_End>();
		out->recordName = "endRecord";
		return out;
	}


	std::multimap<FBXAnalyze::FBXPropertyDataType, std::shared_ptr<FBXAnalyze::FBXProperty>> multimap_properties;
	if (propertyListLen) {
		for (int i = 0; i < propertyCount; i++) {
			char read = fbxReader.ReadVariable<char>();
			FBXAnalyze::FBXPropertyDataType type = (FBXAnalyze::FBXPropertyDataType)read;
			switch (type)
			{
			case FBXAnalyze::FBXPropertyDataType::Short: {
				auto nodeProperty = ObjectFactory::Create<FBXAnalyze::FBXNode_ShortProperty>();
				nodeProperty->nodeProperty = fbxReader.ReadVariable<short>();
				multimap_properties.emplace(type, nodeProperty);
			}
													   break;
			case FBXAnalyze::FBXPropertyDataType::Int: {
				auto nodeProperty = ObjectFactory::Create<FBXAnalyze::FBXNode_IntProperty>();
				nodeProperty->nodeProperty = fbxReader.ReadVariable<int>();


				multimap_properties.emplace(type, nodeProperty);
			}
													 break;
			case FBXAnalyze::FBXPropertyDataType::Long: {

				auto nodeProperty = ObjectFactory::Create<FBXAnalyze::FBXNode_LongProperty>();
				nodeProperty->nodeProperty = fbxReader.ReadVariable<long long int>();


				multimap_properties.emplace(type, nodeProperty);
			}
													  break;
			case FBXAnalyze::FBXPropertyDataType::Float: {
				auto nodeProperty = ObjectFactory::Create<FBXAnalyze::FBXNode_FloatProperty>();
				nodeProperty->nodeProperty = fbxReader.ReadVariable<float>();
				multimap_properties.emplace(type, nodeProperty);
			}
													   break;
			case FBXAnalyze::FBXPropertyDataType::Double: {
				auto nodeProperty = ObjectFactory::Create<FBXAnalyze::FBXNode_DoubleProperty>();
				nodeProperty->nodeProperty = fbxReader.ReadVariable<double>();
				multimap_properties.emplace(type, nodeProperty);
			}
														break;
			case FBXAnalyze::FBXPropertyDataType::String: {

				int propSize = fbxReader.ReadVariable<int>();
				auto nodeProperty = ObjectFactory::Create<FBXAnalyze::FBXNode_StringProperty>();
				nodeProperty->nodeProperty = fbxReader.ReadCharactor(propSize);

				/*if (StringHelper::Contains( nodeProperty->nodeProperty, "RotationStiffnessX")) {
					int f = 0;
				}*/
				multimap_properties.emplace(type, nodeProperty);
			}
														break;
			case FBXAnalyze::FBXPropertyDataType::RawData: {
				int propSize = fbxReader.ReadVariable<int>();

				auto nodeProperty = ObjectFactory::Create<FBXAnalyze::FBXNode_RawDataProperty>();

				nodeProperty->nodeProperty.resize(propSize);
				for (int i = 0; i < propSize; i++) {
					nodeProperty->nodeProperty.at(i) = fbxReader.ReadVariable<char>();
				}
				multimap_properties.emplace(type, nodeProperty);
			}
														 break;
			case FBXAnalyze::FBXPropertyDataType::Bool: {
				auto nodeProperty = ObjectFactory::Create<FBXAnalyze::FBXNode_BoolProperty>();
				nodeProperty->nodeProperty = (bool)fbxReader.ReadVariable<char>();
				multimap_properties.emplace(type, nodeProperty);
			}
													  break;
			case FBXAnalyze::FBXPropertyDataType::FloatArray: {
				auto nodeProperty = ObjectFactory::Create<FBXAnalyze::FBXNode_FloatArrayProperty>();
				UINT arraySize = fbxReader.ReadVariable<UINT>();
				UINT encode = fbxReader.ReadVariable<UINT>();
				UINT complessed = fbxReader.ReadVariable<UINT>();

				if (!encode) {
					nodeProperty->nodeProperty.resize(arraySize);
					for (int i = 0; i < arraySize; i++) {
						nodeProperty->nodeProperty.at(i) = fbxReader.ReadVariable<float>();
					}
				}
				else {
					fbxReader.ReadDefratedArrayData(complessed, arraySize, nodeProperty->nodeProperty);
				}
				multimap_properties.emplace(type, nodeProperty);
			}
															break;
			case FBXAnalyze::FBXPropertyDataType::DoubleArray: {
				auto nodeProperty = ObjectFactory::Create<FBXAnalyze::FBXNode_DoubleArrayProperty>();
				UINT arraySize = fbxReader.ReadVariable<UINT>();
				UINT encode = fbxReader.ReadVariable<UINT>();
				UINT complessed = fbxReader.ReadVariable<UINT>();

				if (!encode) {
					nodeProperty->nodeProperty.resize(arraySize);
					for (int i = 0; i < arraySize; i++) {
						nodeProperty->nodeProperty.at(i) = fbxReader.ReadVariable<double>();
					}
				}
				else {

					fbxReader.ReadDefratedArrayData<double>(complessed, arraySize, nodeProperty->nodeProperty);


				}
				multimap_properties.emplace(type, nodeProperty);
			}
															 break;
			case FBXAnalyze::FBXPropertyDataType::LongArray: {
				auto nodeProperty = ObjectFactory::Create<FBXAnalyze::FBXNode_LongArrayProperty>();
				UINT arraySize = fbxReader.ReadVariable<UINT>();
				UINT encode = fbxReader.ReadVariable<UINT>();
				UINT complessed = fbxReader.ReadVariable<UINT>();

				if (!encode) {
					nodeProperty->nodeProperty.resize(arraySize);
					for (int i = 0; i < arraySize; i++) {
						nodeProperty->nodeProperty.at(i) = fbxReader.ReadVariable<long long int>();
					}
				}
				else {
					fbxReader.ReadDefratedArrayData(complessed, arraySize, nodeProperty->nodeProperty);
				}
				multimap_properties.emplace(type, nodeProperty);
			}
														   break;
			case FBXAnalyze::FBXPropertyDataType::IntArray: {
				auto nodeProperty = ObjectFactory::Create<FBXAnalyze::FBXNode_IntArrayProperty>();
				UINT arraySize = fbxReader.ReadVariable<UINT>();
				UINT encode = fbxReader.ReadVariable<UINT>();
				UINT complessed = fbxReader.ReadVariable<UINT>();

				if (!encode) {
					nodeProperty->nodeProperty.resize(arraySize);
					for (int i = 0; i < arraySize; i++) {
						nodeProperty->nodeProperty.at(i) = fbxReader.ReadVariable<int>();
					}
				}
				else {
					fbxReader.ReadDefratedArrayData(complessed, arraySize, nodeProperty->nodeProperty);
				}
				multimap_properties.emplace(type, nodeProperty);
			}
														  break;
			case FBXAnalyze::FBXPropertyDataType::BoolArray: {
				auto nodeProperty = ObjectFactory::Create<FBXAnalyze::FBXNode_FloatArrayProperty>();
				UINT arraySize = fbxReader.ReadVariable<UINT>();
				UINT encode = fbxReader.ReadVariable<UINT>();
				UINT complessed = fbxReader.ReadVariable<UINT>();

				nodeProperty->nodeProperty.resize(arraySize);
				if (!encode) {
					for (int i = 0; i < arraySize; i++) {
						nodeProperty->nodeProperty.at(i) = fbxReader.ReadVariable<char>();
					}
				}
				else {
					std::vector<char> vec_readChar;
					fbxReader.ReadDefratedArrayData(complessed, arraySize, vec_readChar);

					int i = 0;
					for (auto itr = vec_readChar.begin(); itr != vec_readChar.end(); itr++, i++) {
						nodeProperty->nodeProperty.at(i) = (bool)*itr;
					}
				}
				multimap_properties.emplace(type, nodeProperty);
			}
														   break;
			default:
				out = ObjectFactory::Create<FBXAnalyze::FBXNode_End>();
				out->recordName = "endRecord";
				return out;
				break;
			}
		}
	}
	if (recordName == "NodeAttribute") {
		out = ObjectFactory::Create<FBXAnalyze::FBXAttributeNode>();
	}
	else if (recordName == "C") {
		out = ObjectFactory::Create<FBXAnalyze::FBXConnectionNode>();
	}
	else if (recordName == "Model") {
		auto itr = multimap_properties.begin();
		itr++;
		itr++;
		if (itr->second->GetThis<FBXAnalyze::FBXNode_StringProperty>()->nodeProperty == "Mesh")
			out = ObjectFactory::Create<FBXAnalyze::FBXModelNode>();
		else
			if (itr->second->GetThis<FBXAnalyze::FBXNode_StringProperty>()->nodeProperty == "LimbNode") {
				out = ObjectFactory::Create<FBXAnalyze::FBXBoneNode>();
			}
			else {
				out = ObjectFactory::Create<FBXAnalyze::FBXNodeStructure>();
			}
	}
	else if (recordName == "Material") {
		out = ObjectFactory::Create<FBXAnalyze::FBXMaterialNode>();
	}
	else if (recordName == "Deformer") {
		out = ObjectFactory::Create<FBXAnalyze::FBXDeformerNode>();
	}
	else if (recordName == "Geometry") {
		out = ObjectFactory::Create<FBXAnalyze::FBXGeometryNode>();
	}
	else if (recordName == "Texture") {
		out = ObjectFactory::Create<FBXAnalyze::FBXTextureNode>();
	}
	else if (recordName == "GlobalSettings") {
		out = ObjectFactory::Create<FBXAnalyze::FBXGlobalSettingsNode>();
	}
	else if (recordName == "PolygonVertexIndex") {
		out = ObjectFactory::Create<FBXAnalyze::FBXPolygonVertexIndexNode>();
	}
	else
	{
		out = ObjectFactory::Create<FBXAnalyze::FBXNodeStructure>();
	}
	out->parent = parent;
	out->endOffset = endOffset;
	out->propertyCount = propertyCount;
	out->propertyListLen = propertyListLen;
	out->recordName = recordName;
	out->multimap_properties = multimap_properties;
	out->NodeInitialize();
	if (!out->NullRecordCheck()) {
		arg_vetor_nodeRecord.emplace(out->recordName, out);
	}

	auto nowOffset = (((int)fbxReader.GetNowPos()));
	while (nowOffset < endOffset)
	{
		auto child = ReadNodeRecord(fbxReader, arg_vetor_nodeRecord, out);
		if (!child->NullRecordCheck()) {
			out->multimap_childNodes.emplace(child->recordName, child);
		}
		nowOffset = (((int)fbxReader.GetNowPos()));
	}


	if (out->multimap_childNodes.size()) {
		out->isParent = true;
	}
	if (out->propertyCount == 0) {
		out->isEmpty = true;
	}


	if (recordName == "NodeAttribute") {
		out->GetThis<FBXAnalyze::FBXAttributeNode>()->SetAttributeType();
	}

	return out;
}

void ButiEngine::ModelFileConverter::FBXtoB3M(const std::string& arg_fbxFileName, const std::string& arg_b3mFileName, const std::string& arg_fileDirectory)
{
	BinaryReader fbxReader;
	BinaryWriter b3mWriter;

	fbxReader.ReadStart(GlobalSettings::GetResourceDirectory() + arg_fileDirectory + arg_fbxFileName);
	b3mWriter.WriteStart(GlobalSettings::GetResourceDirectory() + arg_fileDirectory + arg_b3mFileName);
	std::string b3mMagic = "Buti3DModelData";
	b3mWriter.WriteCharactor(b3mMagic);
	b3mWriter.WriteVariable<float>(1.0f);


	auto magic = fbxReader.ReadCharactor(20);
	if (magic != "Kaydara FBX Binary  ") {
		throw ButiException(L"this file is not fbx Format.", L"", L"");
	}
	fbxReader.ReadVariable<char>();
	auto magic2 = fbxReader.ReadVariable<short>();
	auto ver = fbxReader.ReadVariable<int>();

	b3mWriter.WriteVariable<char>(1);

	auto FBXScene = ObjectFactory::Create<FBXAnalyze::FBXScene>();
	do
	{
		auto record = ReadNodeRecord(fbxReader, FBXScene->GetVec_NodeRecords());
		if (record->recordName == "endRecord") {
			break;
		}
	} while (!fbxReader.IsEOF());


	{
		std::string creatorName = FBXScene->SerchNode("FBXHeaderExtension").at(0)->GetChildNode("Creator")->GetProperty<FBXAnalyze::FBXNode_StringProperty>()->nodeProperty;
		b3mWriter.WriteVariable<int>(creatorName.size());
		b3mWriter.WriteCharactor(creatorName);
		b3mWriter.WriteVariable<int>(creatorName.size());
		b3mWriter.WriteCharactor(creatorName);
		std::string  modelName = arg_fbxFileName;

		b3mWriter.WriteVariable<int>(modelName.size());
		b3mWriter.WriteCharactor(modelName);
		b3mWriter.WriteVariable<int>(modelName.size());
		b3mWriter.WriteCharactor(modelName);

		std::string comment = "This file converted from fbx.";
		b3mWriter.WriteVariable<int>(comment.size());
		b3mWriter.WriteCharactor(comment);
		b3mWriter.WriteVariable<int>(comment.size());
		b3mWriter.WriteCharactor(comment);

	}

	auto globalSettings = FBXScene->SerchNode("GlobalSettings").at(0)->GetThis<FBXAnalyze::FBXGlobalSettingsNode>()->GetSettings();


	std::vector<std::shared_ptr<FBXAnalyze::FBXGeometryNode>> geometryMeshNodes;
	{
		auto geometryMeshes = FBXScene->SerchNodeWithStringProperty("Geometry", "Mesh", 1);
		geometryMeshNodes.reserve(geometryMeshes.size());
		for (auto itr = geometryMeshes.begin(); itr != geometryMeshes.end(); itr++) {
			geometryMeshNodes.push_back((*itr)->GetThis<FBXAnalyze::FBXGeometryNode>());
		}

	}


	std::vector<std::shared_ptr<FBXAnalyze::FBXMaterialNode>> materialNodes;
	{
		auto material = FBXScene->SerchNode("Material");

		materialNodes.reserve(material.size());
		int i = 0;
		for (auto itr = material.begin(); itr != material.end(); itr++, i++) {
			materialNodes.push_back((*itr)->GetThis<FBXAnalyze::FBXMaterialNode>());
			materialNodes.at(i)->index = i;
		}
	}
	std::vector<std::shared_ptr<FBXAnalyze::FBXDeformerNode>> deformerNode;
	{

		auto deformer = FBXScene->SerchNodeWithStringProperty("Deformer","Cluster",1);

		for (auto itr =deformer.begin(); itr != deformer.end(); itr++) {
			deformerNode.push_back((*itr)->GetThis<FBXAnalyze::FBXDeformerNode>());
		}
	}

	std::vector<std::shared_ptr<FBXAnalyze::FBXTextureNode>> textureNode;
	{
		auto texture = FBXScene->SerchNode("Texture");

		textureNode.reserve(texture.size());

		for (auto itr = texture.begin(); itr != texture.end(); itr++) {
			textureNode.push_back((*itr)->GetThis<FBXAnalyze::FBXTextureNode>());
		}
	}

	std::vector<std::shared_ptr<FBXAnalyze::FBXModelNode>> modelNodes;
	{
		auto models = FBXScene->SerchNodeWithStringProperty("Model", "Mesh", 1);

		modelNodes.reserve(models.size());

		for (auto itr = models.begin(); itr != models.end(); itr++) {
			modelNodes.push_back((*itr)->GetThis<FBXAnalyze::FBXModelNode>());
		}
	}
	std::vector<std::shared_ptr<FBXAnalyze::FBXBoneNode>> boneNodes;
	{
		auto bones = FBXScene->SerchNodeWithStringProperty("Model", "LimbNode", 1);

		modelNodes.reserve(bones.size());
		int i = 0;
		for (auto itr = bones.begin(); itr != bones.end(); itr++, i++) {
			auto bone = (*itr)->GetThis<FBXAnalyze::FBXBoneNode>();
			bone->boneIndex = i;
			boneNodes.push_back(bone);
		}
	}

	//modelNode、materialNode、geometryNodeのリンク
	{
		std::vector<std::shared_ptr<FBXAnalyze::FBXConnectionNode>> connectionNodes;

		{

			auto cNode = FBXScene->SerchNode("C");

			connectionNodes.reserve(cNode.size());

			for (auto itr = cNode.begin(); itr != cNode.end(); itr++) {
				connectionNodes.push_back((*itr)->GetThis<FBXAnalyze::FBXConnectionNode>());
			}
		}


		std::vector<std::shared_ptr< FBXAnalyze::FBXNodeLinkInfo>> geometryModelLink;
		auto itr = connectionNodes.begin();
		while (itr != connectionNodes.end()) {
			auto link = (*itr)->GetPairNode(modelNodes, geometryMeshNodes);
			if (link) {
				geometryModelLink.push_back(link);
				itr = connectionNodes.erase(itr);
			}
			else
			{
				itr++;
			}
		}

		for (auto itr = geometryModelLink.begin(); itr != geometryModelLink.end(); itr++) {
			(*itr)->resieverNode->GetThis<FBXAnalyze::FBXModelNode>()->geometryNode = (*itr)->serverNode->GetThis<FBXAnalyze::FBXGeometryNode>();
			(*itr)->serverNode->GetThis<FBXAnalyze::FBXGeometryNode>()->modelNode = (*itr)->resieverNode->GetThis<FBXAnalyze::FBXModelNode>();


		}

		std::vector<std::shared_ptr< FBXAnalyze::FBXNodeLinkInfo>> materialModelLink;
		itr = connectionNodes.begin();
		while (itr != connectionNodes.end()) {
			auto link = (*itr)->GetPairNode(modelNodes, materialNodes);
			if (link) {
				materialModelLink.push_back(link);
				itr = connectionNodes.erase(itr);
			}
			else
			{
				itr++;
			}
		}

		for (auto itr = materialModelLink.begin(); itr != materialModelLink.end(); itr++) {
			(*itr)->resieverNode->GetThis<FBXAnalyze::FBXModelNode>()->vec_materialNodes.push_back({ (*itr)->serverNode->GetThis<FBXAnalyze::FBXMaterialNode>()->index ,0 });
			(*itr)->serverNode->GetThis<FBXAnalyze::FBXMaterialNode>()->geometryNodes.push_back((*itr)->resieverNode->GetThis<FBXAnalyze::FBXModelNode>()->geometryNode);
			(*itr)->serverNode->GetThis<FBXAnalyze::FBXMaterialNode>()->modelNodes.push_back((*itr)->resieverNode->GetThis<FBXAnalyze::FBXModelNode>());


		}


		std::vector<std::shared_ptr< FBXAnalyze::FBXNodeLinkInfo>> boneLink;
		itr = connectionNodes.begin();
		while (itr != connectionNodes.end()) {
			auto link = (*itr)->GetPairNode(boneNodes, boneNodes);
			if (link) {
				boneLink.push_back(link);
				itr = connectionNodes.erase(itr);
			}
			else
			{
				itr++;
			}
		}

		for (auto itr = boneLink.begin(); itr != boneLink.end(); itr++) {
			(*itr)->serverNode->GetThis<FBXAnalyze::FBXBoneNode>()->parentBoneIndex = (*itr)->resieverNode->GetThis<FBXAnalyze::FBXBoneNode>()->boneIndex;


		}

		std::vector<std::shared_ptr< FBXAnalyze::FBXNodeLinkInfo>> materialTextureLink;
		itr = connectionNodes.begin();
		while (itr != connectionNodes.end()) {
			auto link = (*itr)->GetThis<FBXAnalyze::FBXConnectionNode>()->GetPairNode(materialNodes, textureNode);
			if (link) {
				materialTextureLink.push_back(link);
				itr = connectionNodes.erase(itr);
			}
			else
			{
				itr++;
			}
		}

		for (auto itr = materialTextureLink.begin(); itr != materialTextureLink.end(); itr++) {
			if ((*itr)->info == "DiffuseColor")
				(*itr)->resieverNode->GetThis<FBXAnalyze::FBXMaterialNode>()->diffuseTextureNode = (*itr)->serverNode->GetThis<FBXAnalyze::FBXTextureNode>();
		}

		std::vector<std::shared_ptr< FBXAnalyze::FBXNodeLinkInfo>> boneDeformerLink;
		itr = connectionNodes.begin();
		while (itr != connectionNodes.end()) {
			auto link = (*itr)->GetThis<FBXAnalyze::FBXConnectionNode>()->GetPairNode(deformerNode,boneNodes);
			if (link) {
				boneDeformerLink.push_back(link);
				itr = connectionNodes.erase(itr);
			}
			else
			{
				itr++;
			}
		}
		for (auto itr = boneDeformerLink.begin(); itr != boneDeformerLink.end(); itr++) {
			(*itr)->resieverNode->GetThis<FBXAnalyze::FBXDeformerNode>()->boneIndex = (*itr)->serverNode->GetThis<FBXAnalyze::FBXBoneNode>()->boneIndex;
		}
		int i = 0;
	}


	//マテリアルの仕様回数
	int materialCount = 0;
	{
		for (int i = 0; i < modelNodes.size(); i++) {
			materialCount += modelNodes.at(i)->vec_materialNodes.size();
		}
	}

	std::vector<int> index;



	//インデックスバイトサイズの書き込み

	int vertexCount = 0;
	for (int i = 0; i < geometryMeshNodes.size(); i++) {
		vertexCount += geometryMeshNodes.at(i)->GetPolygonVertexIndexNode()->GetPolygonIndex().size();
	}

	char vertexIndexByteSize = MathHelper::GetByteSize(vertexCount);
	b3mWriter.WriteVariable<char>(vertexIndexByteSize);
	char materialIndexByteSize = MathHelper::GetByteSize(materialCount);
	b3mWriter.WriteVariable<char>(materialIndexByteSize);
	char boneIndexByteSize = MathHelper::GetByteSize(boneNodes.size());
	b3mWriter.WriteVariable<char>(boneIndexByteSize);
	char morphIndexByteSize = 1;
	b3mWriter.WriteVariable<char>(morphIndexByteSize);

	auto typePoint = b3mWriter.GetNowPos();
	b3mWriter.WriteVariable((char)Vertex::VertexType::Vertex_UV_Normal);

	//uvEx
	b3mWriter.WriteVariable<char>(0);
	//VertexCount
	b3mWriter.WriteUINT(vertexCount, vertexIndexByteSize);
	if (!geometryMeshNodes.size()) {
		throw ButiException(L"this file doesn't contain Geometry", L"", L"");
	}

	for (auto itr = modelNodes.begin(); itr != modelNodes.end(); itr++) {

		auto geometry = (*itr)->geometryNode.lock();

		auto vertexNode = geometry->GetChildNode("Vertices");

		auto UVLayerNode = geometry->GetChildNode("LayerElementUV");

		auto normalLayerNode = geometry->GetChildNode("LayerElementNormal");

		auto materialLayerNode = geometry->GetChildNode("LayerElementMaterial");

		auto polygonIndexNode = geometry->GetPolygonVertexIndexNode();

		auto polygonIndex = polygonIndexNode->GetPolygonIndex();
		
		auto transformMatrix = geometry->modelNode.lock()->GetTransform(globalSettings);

		if (deformerNode.size()) {

			auto vertices = vertexNode->GetProperty<FBXAnalyze::FBXNode_DoubleArrayProperty>()->CreateVertex<Vertex::ModelVertex::Vertex_Model_QuadBone>(globalSettings, &transformMatrix);
			for (auto defItr = deformerNode.begin(); defItr != deformerNode.end(); defItr++) {

				auto index = (*defItr)->GetBoneIndex();
				auto weight= (*defItr)->GetWeight();
				auto weightItr = weight.begin();
				for (auto indexItr = index.begin(); indexItr != index.end() && weightItr != weight.end(); indexItr++, weightItr++) {
					if (vertices.at(*indexItr).boneIndex_1 == -1) {
						vertices.at(*indexItr).boneIndex_1 = (*defItr)->boneIndex;
						vertices.at(*indexItr).weight_01 = *weightItr;
					}
					else if (vertices.at(*indexItr).boneIndex_2 == -1) {
						vertices.at(*indexItr).boneIndex_2 = (*defItr)->boneIndex;
						vertices.at(*indexItr).weight_02 = *weightItr;
					}
					else if (vertices.at(*indexItr).boneIndex_3 == -1) {
						vertices.at(*indexItr).boneIndex_3 = (*defItr)->boneIndex;
						vertices.at(*indexItr).weight_03 = *weightItr;
					}
					else if (vertices.at(*indexItr).boneIndex_4 == -1) {
						vertices.at(*indexItr).boneIndex_4 = (*defItr)->boneIndex;
						vertices.at(*indexItr).weight_04 = *weightItr;
					}
				}

			}
			WriteVertex(globalSettings, vertices, transformMatrix, polygonIndex, b3mWriter, UVLayerNode, normalLayerNode);

		}
		else {
			auto vertices = vertexNode->GetProperty<FBXAnalyze::FBXNode_DoubleArrayProperty>()->CreateVertex<Vertex::Vertex_UV_Normal>(globalSettings, &transformMatrix);
			WriteVertex(globalSettings, vertices, transformMatrix, polygonIndex, b3mWriter, UVLayerNode, normalLayerNode);
		}
		auto materialIndex = materialLayerNode->GetChildNode("Materials")->GetProperty<FBXAnalyze::FBXNode_IntArrayProperty>()->nodeProperty;

		std::vector<int> materialIndexSizes;
		std::string materialMapType = materialLayerNode->GetChildNode("MappingInformationType")->GetProperty < FBXAnalyze::FBXNode_StringProperty >()->nodeProperty;

		if (materialMapType == "AllSame") {

			materialIndexSizes.resize((*itr)->vec_materialNodes.size());;
			materialIndexSizes.at(materialIndex.at(0)) =
			polygonIndexNode->CreateTriPolygonIndexByMonoMaterial().size();
		}
		else {

			polygonIndexNode->CreateTriPolygonIndexByPolyMaterial(materialIndex, (*itr)->vec_materialNodes.size(), materialIndexSizes);

		}
		auto nowIndex = polygonIndexNode->GetTriPolygonIndex();

		for (int matNodeI = 0; matNodeI < (*itr)->vec_materialNodes.size(); matNodeI++) {
			(*itr)->vec_materialNodes.at(matNodeI).second = materialIndexSizes.at(matNodeI);
		}

		index.reserve(index.size() + nowIndex.size());

		std::copy(nowIndex.begin(), nowIndex.end(), std::back_inserter(index));

		index.shrink_to_fit();



	}

	{
		auto nowPoint = b3mWriter.GetNowPos();
		b3mWriter.Jump(typePoint); 
		if (deformerNode.size())
		b3mWriter.WriteVariable((char)Vertex::VertexType::Vertex_Model_QuadBone);
		b3mWriter.Jump(nowPoint);
	}
	//インデックス
	b3mWriter.WriteVariable<int>(index.size() / 3);

	for (int i = 0; i < index.size(); i++) {
		b3mWriter.WriteUINT(index.at(i), vertexIndexByteSize);
	}

	//マテリアルのディレクトリ作成
	auto materialDirPath = GlobalSettings::GetResourceDirectory() + arg_fileDirectory + "material/";
	auto dirRes = _mkdir(materialDirPath.c_str());

	//マテリアルファイルのパス
	std::vector<std::string> vec_materialPaths;
	vec_materialPaths.resize(materialNodes.size());

	//マテリアルファイル生成
	for (int i = 0; i < materialNodes.size(); i++) {
		BinaryWriter bmaWriter;
		std::string materialName;
		auto material = materialNodes.at(i);
		//ファイル生成、ファイルマジック、マテリアル名書き込み
		{

			materialName = material->GetProperty<FBXAnalyze::FBXNode_StringProperty>()->nodeProperty;
			int materialNameCount = materialName.size();


			bmaWriter.WriteStart(GlobalSettings::GetResourceDirectory() + arg_fileDirectory + "material/" + materialName + ".bma");
			vec_materialPaths.at(i) = "material/" + materialName + ".bma";



			bmaWriter.WriteCharactor("ButiMaterialData");
			bmaWriter.WriteVariable<float>(1.0f);
			bmaWriter.WriteVariable<char>(1);
			bmaWriter.WriteVariable<int>(materialName.size());

			bmaWriter.WriteCharactor(materialName);

		}
		//マテリアル名英
		{
			bmaWriter.WriteVariable<int>(materialName.size());
			bmaWriter.WriteCharactor(materialName);

		}
		//各パラメータ

		auto materialProperttyNode = material->GetChildNode("Properties70");

		auto diffuseNode = materialProperttyNode->GetChildPropertyNode("P", "DiffuseColor");
		auto ambientNode = materialProperttyNode->GetChildPropertyNode("P", "AmbientColor");
		auto ambientFactorNode = materialProperttyNode->GetChildPropertyNode("P", "AmbientFactor");
		auto transparentNode = materialProperttyNode->GetChildPropertyNode("P", "TransparentColor");
		auto transparencyFactorNode = materialProperttyNode->GetChildPropertyNode("P", "TransparencyFactor");
		auto opacityNode = materialProperttyNode->GetChildPropertyNode("P", "Opacity");
		auto specularNode = materialProperttyNode->GetChildPropertyNode("P", "SpecularColor");
		auto specularFactorNode = materialProperttyNode->GetChildPropertyNode("P", "SpecularFactor");
		auto shinnesNode = materialProperttyNode->GetChildPropertyNode("P", "Shininess");

		auto shinnesFactorNode = materialProperttyNode->GetChildPropertyNode("P", "ShininessExponent");
		auto reflectionNode = materialProperttyNode->GetChildPropertyNode("P", "ReflectionColor");

		auto reflectionFactorNode = materialProperttyNode->GetChildPropertyNode("P", "ReflectionFactor");

		float alpha = 1.0f;
		if (opacityNode) {
			alpha = opacityNode->GetFloat();
		}
		Vector4 diffuse = Vector4(diffuseNode->GetVector3(globalSettings), alpha);
		Vector4 specular = Vector4(specularNode->GetVector3(globalSettings), specularFactorNode->GetFloat());
		Vector4 ambient = Vector4(ambientNode->GetVector3(globalSettings), ambientFactorNode->GetFloat());
		Vector4 emissive;
		if (reflectionNode) {
			emissive = Vector4(reflectionNode->GetVector3(globalSettings), reflectionFactorNode->GetFloat());
		}
		if (specular.z > 1.0f) {
			specular.z = 1.0f;
		}


		//テクスチャパラメータ
		auto diffiseTextureNode = material->diffuseTextureNode;


		//テクスチャパラメータ書き込み
		{
			bmaWriter.WriteVariable<char>(1);


			{
				std::string path;
				if (diffiseTextureNode.lock()) {
					path = diffiseTextureNode.lock()->GetChildNode("RelativeFilename")->GetProperty<FBXAnalyze::FBXNode_StringProperty>()->nodeProperty;
					bmaWriter.WriteVariable<int>(path.size());
					bmaWriter.WriteCharactor(path);

				}
				else
				{
					bmaWriter.WriteVariable<int>(-1);
				}
			}





		}
		//パラメータ書き込み
		{
			bmaWriter.WriteVariable<Vector4>(diffuse);
			bmaWriter.WriteVariable<Vector4>(specular);
			bmaWriter.WriteVariable<Vector4>(ambient);
			bmaWriter.WriteVariable<Vector4>(Vector4());

			bmaWriter.WriteVariable<char>(0);
			bmaWriter.WriteVariable<char>(0);
		}
		//コメント
		std::string comment = "this file made by fbxFile";
		int commentByteSize = comment.size();

		bmaWriter.WriteVariable<int>(commentByteSize);
		bmaWriter.WriteCharactor(comment);


		bmaWriter.WriteEnd();
	}

	//マテリアルの仕様情報テーブル
	std::vector<std::pair< std::string, int>> vec_materialTables;
	{
		vec_materialTables.reserve(materialCount);
		for (auto modelItr = modelNodes.begin(); modelItr != modelNodes.end(); modelItr++) {
			for (auto materialItr = (*modelItr)->vec_materialNodes.begin();
				materialItr != (*modelItr)->vec_materialNodes.end();
				materialItr++
				) {
				std::pair<std::string, int> table;

				table.first = vec_materialPaths.at((materialItr->first));
				table.second = (materialItr)->second;

				vec_materialTables.push_back(table);
			}
		}
	}


	//マテリアルの仕様情報の記述
	b3mWriter.WriteUINT(vec_materialTables.size(), materialIndexByteSize);


	for (int i = 0; i < vec_materialTables.size(); i++) {
		std::string materialPath = vec_materialTables.at(i).first;
		b3mWriter.WriteVariable<int>(materialPath.size());
		b3mWriter.WriteCharactor(materialPath);
		b3mWriter.WriteVariable<int>(vec_materialTables.at(i).second);

	}

	//ボーン
	{
		UINT boneCount = boneNodes.size();

		b3mWriter.WriteUINT(boneCount, boneIndexByteSize);

		for (int i = 0; i < boneCount; i++) {
			//ボーン名
			std::string boneName = boneNodes.at(i)->GetName();

			b3mWriter.WriteVariable<int>(boneName.size());
			b3mWriter.WriteCharactor(boneName);
			
			//ボーン名英
			b3mWriter.WriteVariable<int>(0);


			auto parent = boneNodes.at(i)->parentBoneIndex;

			Vector3 pos = boneNodes.at(i)->GetPosition(boneNodes, globalSettings);

			b3mWriter.WriteInt(parent, boneIndexByteSize);

			//auto transformFloor = pmxReader.ReadVariable<int>();

			b3mWriter.WriteVariable<int>(0);

			b3mWriter.WriteVariable(pos);


			//boneFlag = BinaryHelper::SwapBytes(boneFlag);
			short boneFlag = 31;

			b3mWriter.WriteVariable<short>(boneFlag);

			b3mWriter.WriteInt(0, boneIndexByteSize);
		}

	}


	//モーフ
	b3mWriter.WriteUINT(0, morphIndexByteSize);

}

void ButiEngine::ModelFileConverter::PMXtoB3M(const std::string& arg_pmxFileName, const std::string& arg_b3mFileName, const std::string& arg_fileDirectory)
{
	BinaryReader pmxReader;
	BinaryWriter b3mWriter;

	pmxReader.ReadStart(GlobalSettings::GetResourceDirectory() + arg_fileDirectory + arg_pmxFileName);
	b3mWriter.WriteStart(GlobalSettings::GetResourceDirectory() + arg_fileDirectory + arg_b3mFileName);
	std::string b3mMagic = "Buti3DModelData";
	b3mWriter.WriteCharactor(b3mMagic);
	b3mWriter.WriteVariable<float>(1.0f);

	auto magic = pmxReader.ReadCharactor(4);
	if (magic != "PMX ") {
		throw ButiException(L"this file is not pmx Format.", L"", L"");
	}

	auto ver = pmxReader.ReadVariable<float>();

	char nextByte = pmxReader.ReadVariable<char>();

	char encodeType = pmxReader.ReadVariable<char>();

	char exUVCount = pmxReader.ReadVariable<char>();
	char vertexIndexByteSize = pmxReader.ReadVariable<char>();

	char textureIndexByteSize = pmxReader.ReadVariable<char>();

	char materialIndexByteSize = pmxReader.ReadVariable<char>();

	char boneIndexByteSize = pmxReader.ReadVariable<char>();


	char morphIndexByteSize = pmxReader.ReadVariable<char>();


	char rigidIndexByteSize = pmxReader.ReadVariable<char>();

	PMXReadStruct pmxStruct;

	if (nextByte > 8) {
		pmxReader.ReadVariable<char>();
	}
	int textCount = pmxReader.ReadVariable<int>();

	//モデル名
	{
		std::wstring nameWStr;
		if (encodeType) {
			Util::MultiBytetoWString(pmxReader.ReadCharactor(textCount), nameWStr);

		}
		else {

			nameWStr = pmxReader.ReadWCharactor(textCount / 2);
		}
		pmxStruct.modelName = (nameWStr);
	}
	//モデル名英
	{
		textCount = pmxReader.ReadVariable<int>();
		std::wstring nameWStr;
		if (encodeType) {
			Util::MultiBytetoWString(pmxReader.ReadCharactor(textCount), nameWStr);

		}
		else {

			nameWStr = pmxReader.ReadWCharactor(textCount / 2);
		}
		pmxStruct.modelEngName = (nameWStr);
	}
	//コメント
	{
		textCount = pmxReader.ReadVariable<int>(); std::wstring nameWStr;
		if (encodeType) {
			Util::MultiBytetoWString(pmxReader.ReadCharactor(textCount), nameWStr);

		}
		else {

			nameWStr = pmxReader.ReadWCharactor(textCount / 2);
		}
		pmxStruct.comment = (nameWStr);
	}
	//コメント英
	{
		textCount = pmxReader.ReadVariable<int>(); std::wstring nameWStr;
		if (encodeType) {
			Util::MultiBytetoWString(pmxReader.ReadCharactor(textCount), nameWStr);
		}
		else {

			nameWStr = pmxReader.ReadWCharactor(textCount / 2);
		}
		pmxStruct.engComment = (nameWStr);
	}

	b3mWriter.WriteVariable<char>(encodeType);

	if (encodeType) {
		std::string formatName = "made from pmx";

		b3mWriter.WriteVariable<int>(formatName.size());
		b3mWriter.WriteCharactor(formatName);
		b3mWriter.WriteVariable<int>(formatName.size());
		b3mWriter.WriteCharactor(formatName);

		b3mWriter.WriteVariable<int>(pmxStruct.modelName.size());
		b3mWriter.WriteCharactor(Util::WStringToString(pmxStruct.modelName));
		b3mWriter.WriteVariable<int>(pmxStruct.modelEngName.size());
		b3mWriter.WriteCharactor(Util::WStringToString(pmxStruct.modelEngName));
		b3mWriter.WriteVariable<int>(pmxStruct.comment.size());
		b3mWriter.WriteCharactor(Util::WStringToString(pmxStruct.comment));
		b3mWriter.WriteVariable<int>(pmxStruct.engComment.size());
		b3mWriter.WriteCharactor(Util::WStringToString(pmxStruct.engComment));
	}
	else {



		std::wstring formatName = L"made from pmx";

		b3mWriter.WriteVariable<int>(formatName.size());
		b3mWriter.WriteWCharactor(formatName);
		b3mWriter.WriteVariable<int>(formatName.size());
		b3mWriter.WriteWCharactor(formatName);

		b3mWriter.WriteVariable<int>(pmxStruct.modelName.size());
		b3mWriter.WriteWCharactor(pmxStruct.modelName);
		b3mWriter.WriteVariable<int>(pmxStruct.modelEngName.size());
		b3mWriter.WriteWCharactor(pmxStruct.modelEngName);
		b3mWriter.WriteVariable<int>(pmxStruct.comment.size());
		b3mWriter.WriteWCharactor(pmxStruct.comment);
		b3mWriter.WriteVariable<int>(pmxStruct.engComment.size());
		b3mWriter.WriteWCharactor(pmxStruct.engComment);
	}
	//インデックスバイトサイズの書き込み
	b3mWriter.WriteVariable<char>(vertexIndexByteSize);
	b3mWriter.WriteVariable<char>(materialIndexByteSize);
	b3mWriter.WriteVariable<char>(boneIndexByteSize);
	b3mWriter.WriteVariable<char>(morphIndexByteSize);

	int vertexCount = pmxReader.ReadVariable<int>();
	auto vertexTypePoint = b3mWriter.GetNowPos();
	b3mWriter.WriteVariable<char>((char)Vertex::VertexType::Vertex_Model_Mix);
	b3mWriter.WriteVariable<char>(exUVCount);

	b3mWriter.WriteUINT(vertexCount, vertexIndexByteSize);

	//ここから頂点

	UINT singleBoneCount = 0;
	UINT doubleBoneCount = 0;
	UINT quadBoneCount = 0;
	UINT sdefBoneCount = 0;
	for (UINT i = 0; i < vertexCount; i++) {
		auto pos = pmxReader.ReadVariable<Vector3>();
		auto normal = pmxReader.ReadVariable<Vector3>();
		auto uv = pmxReader.ReadVariable<Vector2>();
		std::vector< Vector4> vec_UV;
		vec_UV.resize(exUVCount);

		for (int j = 0; j < exUVCount; j++) {
			vec_UV.at(i) = pmxReader.ReadVariable<Vector4>();
		}

		Vertex::VertexType type = (Vertex::VertexType)(pmxReader.ReadVariable<char>() + 4);

		b3mWriter.WriteVariable<char>((char)(type));


		switch (type) {
		case Vertex::VertexType::Vertex_Model_SingleBone: {

			switch (exUVCount)
			{
			case 0: {
				Vertex::ModelVertex::Vertex_Model_SingleBone vertex;
				vertex.position = pos;
				vertex.uv = uv;
				vertex.normal = normal;

				vertex.boneIndex = pmxReader.ReadInt(boneIndexByteSize);
				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_SingleBone>(vertex);
			}
				  break;
			case 1: {
				Vertex::ModelVertex::Vertex_Model_SingleBone_UVex01 vertex;
				vertex.position = pos;
				vertex.SetUV(uv);
				vertex.SetUV_ex1(vec_UV.at(0));
				vertex.normal = normal;

				vertex.boneIndex = pmxReader.ReadInt(boneIndexByteSize);


				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_SingleBone_UVex01>(vertex);
			}
				  break;
			case 2: {
				Vertex::ModelVertex::Vertex_Model_SingleBone_UVex02 vertex;
				vertex.position = pos;
				vertex.SetUV(uv);
				vertex.SetUV_ex1(vec_UV.at(0));
				vertex.SetUV_ex2(vec_UV.at(1));
				vertex.normal = normal;

				vertex.boneIndex = pmxReader.ReadInt(boneIndexByteSize);


				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_SingleBone_UVex02>(vertex);
			}
				  break;
			case 3: {
				Vertex::ModelVertex::Vertex_Model_SingleBone_UVex03 vertex;
				vertex.position = pos;
				vertex.SetUV(uv);
				vertex.SetUV_ex1(vec_UV.at(0));
				vertex.SetUV_ex2(vec_UV.at(1));
				vertex.SetUV_ex3(vec_UV.at(2));
				vertex.normal = normal;

				vertex.boneIndex = pmxReader.ReadInt(boneIndexByteSize);


				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_SingleBone_UVex03>(vertex);
			}
				  break;
			case 4: {
				Vertex::ModelVertex::Vertex_Model_SingleBone_UVex04 vertex;
				vertex.position = pos;
				vertex.SetUV(uv);
				vertex.SetUV_ex1(vec_UV.at(0));
				vertex.SetUV_ex2(vec_UV.at(1));
				vertex.SetUV_ex3(vec_UV.at(2));
				vertex.SetUV_ex4(vec_UV.at(3));
				vertex.normal = normal;

				vertex.boneIndex = pmxReader.ReadInt(boneIndexByteSize);


				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_SingleBone_UVex04>(vertex);
			}
				  break;
			}

			singleBoneCount++;

		}break;
		case Vertex::VertexType::Vertex_Model_DoubleBone: {

			switch (exUVCount)
			{
			case 0: {
				Vertex::ModelVertex::Vertex_Model_DoubleBone vertex;
				vertex.position = pos;
				vertex.uv = uv;
				vertex.normal = normal;

				vertex.boneIndex_1 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_2 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.weight = pmxReader.ReadVariable<float>();

				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_DoubleBone>(vertex);
			}
				  break;
			case 1: {
				Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex01 vertex;
				vertex.position = pos;
				vertex.SetUV(uv);
				vertex.SetUV_ex1(vec_UV.at(0));
				vertex.normal = normal;

				vertex.boneIndex_1 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_2 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.weight = pmxReader.ReadVariable<float>();

				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex01>(vertex);
			}
				  break;
			case 2: {
				Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex02 vertex;
				vertex.position = pos;
				vertex.SetUV(uv);
				vertex.SetUV_ex1(vec_UV.at(0));
				vertex.SetUV_ex2(vec_UV.at(1));
				vertex.normal = normal;

				vertex.boneIndex_1 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_2 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.weight = pmxReader.ReadVariable<float>();

				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex02>(vertex);
			}
				  break;
			case 3: {
				Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex03 vertex;
				vertex.position = pos;
				vertex.SetUV(uv);
				vertex.SetUV_ex1(vec_UV.at(0));
				vertex.SetUV_ex2(vec_UV.at(1));
				vertex.SetUV_ex3(vec_UV.at(2));
				vertex.normal = normal;

				vertex.boneIndex_1 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_2 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.weight = pmxReader.ReadVariable<float>();

				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex03>(vertex);
			}
				  break;
			case 4: {
				Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex04 vertex;
				vertex.position = pos;
				vertex.SetUV(uv);
				vertex.SetUV_ex1(vec_UV.at(0));
				vertex.SetUV_ex2(vec_UV.at(1));
				vertex.SetUV_ex3(vec_UV.at(2));
				vertex.SetUV_ex4(vec_UV.at(3));
				vertex.normal = normal;

				vertex.boneIndex_1 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_2 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.weight = pmxReader.ReadVariable<float>();

				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_DoubleBone_UVex04>(vertex);
			}
				  break;
			}

			doubleBoneCount++;
		}break;
		case Vertex::VertexType::Vertex_Model_QuadBone: {

			switch (exUVCount)
			{
			case 0: {
				Vertex::ModelVertex::Vertex_Model_QuadBone vertex;
				vertex.position = pos;
				vertex.uv = uv;
				vertex.normal = normal;

				vertex.boneIndex_1 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_2 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_3 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_4 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.weight_01 = pmxReader.ReadVariable<float>();

				vertex.weight_02 = pmxReader.ReadVariable<float>();

				vertex.weight_03 = pmxReader.ReadVariable<float>();

				vertex.weight_04 = pmxReader.ReadVariable<float>();

				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_QuadBone>(vertex);
			}
				  break;
			case 1: {
				Vertex::ModelVertex::Vertex_Model_QuadBone_UVex01 vertex;
				vertex.position = pos;
				vertex.SetUV(uv);
				vertex.SetUV_ex1(vec_UV.at(0));
				vertex.normal = normal;

				vertex.boneIndex_1 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_2 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_3 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_4 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.weight_01 = pmxReader.ReadVariable<float>();

				vertex.weight_02 = pmxReader.ReadVariable<float>();

				vertex.weight_03 = pmxReader.ReadVariable<float>();

				vertex.weight_04 = pmxReader.ReadVariable<float>();

				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_QuadBone_UVex01>(vertex);
			}
				  break;
			case 2: {
				Vertex::ModelVertex::Vertex_Model_QuadBone_UVex02 vertex;
				vertex.position = pos;
				vertex.SetUV(uv);
				vertex.SetUV_ex1(vec_UV.at(0));
				vertex.SetUV_ex2(vec_UV.at(1));
				vertex.normal = normal;

				vertex.boneIndex_1 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_2 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_3 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_4 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.weight_01 = pmxReader.ReadVariable<float>();

				vertex.weight_02 = pmxReader.ReadVariable<float>();

				vertex.weight_03 = pmxReader.ReadVariable<float>();

				vertex.weight_04 = pmxReader.ReadVariable<float>();

				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_QuadBone_UVex02>(vertex);
			}
				  break;
			case 3: {
				Vertex::ModelVertex::Vertex_Model_QuadBone_UVex03 vertex;
				vertex.position = pos;
				vertex.SetUV(uv);
				vertex.SetUV_ex1(vec_UV.at(0));
				vertex.SetUV_ex2(vec_UV.at(1));
				vertex.SetUV_ex3(vec_UV.at(2));
				vertex.normal = normal;

				vertex.boneIndex_1 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_2 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_3 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_4 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.weight_01 = pmxReader.ReadVariable<float>();

				vertex.weight_02 = pmxReader.ReadVariable<float>();

				vertex.weight_03 = pmxReader.ReadVariable<float>();

				vertex.weight_04 = pmxReader.ReadVariable<float>();

				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_QuadBone_UVex03>(vertex);
			}
				  break;
			case 4: {
				Vertex::ModelVertex::Vertex_Model_QuadBone_UVex04 vertex;
				vertex.position = pos;
				vertex.SetUV(uv);
				vertex.SetUV_ex1(vec_UV.at(0));
				vertex.SetUV_ex2(vec_UV.at(1));
				vertex.SetUV_ex3(vec_UV.at(2));
				vertex.SetUV_ex4(vec_UV.at(3));
				vertex.normal = normal;

				vertex.boneIndex_1 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_2 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_3 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_4 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.weight_01 = pmxReader.ReadVariable<float>();

				vertex.weight_02 = pmxReader.ReadVariable<float>();

				vertex.weight_03 = pmxReader.ReadVariable<float>();

				vertex.weight_04 = pmxReader.ReadVariable<float>();

				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_QuadBone_UVex04>(vertex);
			}
				  break;
			}
			quadBoneCount++;

		}break;
		case Vertex::VertexType::Vertex_Model_SDEFBone: {

			switch (exUVCount)
			{
			case 0: {
				Vertex::ModelVertex::Vertex_Model_SdefBone vertex;
				vertex.position = pos;
				vertex.uv = uv;
				vertex.normal = normal;

				vertex.boneIndex_1 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_2 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.weight = pmxReader.ReadVariable<float>();

				vertex.SDEF_C = pmxReader.ReadVariable<Vector3>();

				vertex.SDEF_R0 = pmxReader.ReadVariable<Vector3>();

				vertex.SDEF_R1 = pmxReader.ReadVariable<Vector3>();

				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_SdefBone>(vertex);
			}
				  break;
			case 1: {
				Vertex::ModelVertex::Vertex_Model_SdefBone_UVex01 vertex;
				vertex.position = pos;
				vertex.SetUV(uv);
				vertex.SetUV_ex1(vec_UV.at(0));
				vertex.normal = normal;

				vertex.boneIndex_1 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_2 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.weight = pmxReader.ReadVariable<float>();

				vertex.SDEF_C = pmxReader.ReadVariable<Vector3>();

				vertex.SDEF_R0 = pmxReader.ReadVariable<Vector3>();

				vertex.SDEF_R1 = pmxReader.ReadVariable<Vector3>();

				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_SdefBone_UVex01>(vertex);
			}
				  break;
			case 2: {
				Vertex::ModelVertex::Vertex_Model_SdefBone_UVex02 vertex;
				vertex.position = pos;
				vertex.SetUV(uv);
				vertex.SetUV_ex1(vec_UV.at(0));
				vertex.SetUV_ex2(vec_UV.at(1));
				vertex.normal = normal;

				vertex.boneIndex_1 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_2 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.weight = pmxReader.ReadVariable<float>();

				vertex.SDEF_C = pmxReader.ReadVariable<Vector3>();

				vertex.SDEF_R0 = pmxReader.ReadVariable<Vector3>();

				vertex.SDEF_R1 = pmxReader.ReadVariable<Vector3>();

				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_SdefBone_UVex02>(vertex);
			}
				  break;
			case 3: {
				Vertex::ModelVertex::Vertex_Model_SdefBone_UVex03 vertex;
				vertex.position = pos;
				vertex.SetUV(uv);
				vertex.SetUV_ex1(vec_UV.at(0));
				vertex.SetUV_ex2(vec_UV.at(1));
				vertex.SetUV_ex3(vec_UV.at(2));
				vertex.normal = normal;

				vertex.boneIndex_1 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_2 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.weight = pmxReader.ReadVariable<float>();

				vertex.SDEF_C = pmxReader.ReadVariable<Vector3>();

				vertex.SDEF_R0 = pmxReader.ReadVariable<Vector3>();

				vertex.SDEF_R1 = pmxReader.ReadVariable<Vector3>();

				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_SdefBone_UVex03>(vertex);
			}
				  break;
			case 4: {
				Vertex::ModelVertex::Vertex_Model_SdefBone_UVex04 vertex;
				vertex.position = pos;
				vertex.SetUV(uv);
				vertex.SetUV_ex1(vec_UV.at(0));
				vertex.SetUV_ex2(vec_UV.at(1));
				vertex.SetUV_ex3(vec_UV.at(2));
				vertex.SetUV_ex4(vec_UV.at(3));
				vertex.normal = normal;

				vertex.boneIndex_1 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.boneIndex_2 = pmxReader.ReadInt(boneIndexByteSize);

				vertex.weight = pmxReader.ReadVariable<float>();

				vertex.SDEF_C = pmxReader.ReadVariable<Vector3>();

				vertex.SDEF_R0 = pmxReader.ReadVariable<Vector3>();

				vertex.SDEF_R1 = pmxReader.ReadVariable<Vector3>();

				b3mWriter.WriteVariable<Vertex::ModelVertex::Vertex_Model_SdefBone_UVex04>(vertex);
			}
				  break;
			}

			sdefBoneCount++;
		}break;
		}


		pmxReader.ReadVariable<float>();
	}
	auto vertexIndexPoint = b3mWriter.GetNowPos();
	if (vertexCount == singleBoneCount) {
		b3mWriter.Jump(vertexTypePoint);
		b3mWriter.WriteVariable<char>((char)Vertex::VertexType::Vertex_Model_SingleBone);
		b3mWriter.Jump(vertexIndexPoint);
	}
	else
		if (vertexCount == doubleBoneCount) {
			b3mWriter.Jump(vertexTypePoint);
			b3mWriter.WriteVariable<char>((char)Vertex::VertexType::Vertex_Model_SingleBone);
			b3mWriter.Jump(vertexIndexPoint);
		}
		else
			if (vertexCount == quadBoneCount) {
				b3mWriter.Jump(vertexTypePoint);
				b3mWriter.WriteVariable<char>((char)Vertex::VertexType::Vertex_Model_SingleBone);
				b3mWriter.Jump(vertexIndexPoint);
			}
			else
				if (vertexCount == sdefBoneCount) {
					b3mWriter.Jump(vertexTypePoint);
					b3mWriter.WriteVariable<char>((char)Vertex::VertexType::Vertex_Model_SingleBone);
					b3mWriter.Jump(vertexIndexPoint);
				}

	//頂点インデックス
	int vertexSurfaceIndex = pmxReader.ReadVariable<int>();

	b3mWriter.WriteVariable<int>(vertexSurfaceIndex / 3);

	for (int i = 0; i < vertexSurfaceIndex; i++) {
		b3mWriter.WriteUINT(pmxReader.ReadUINT(vertexIndexByteSize), vertexIndexByteSize);
	}

	//テクスチャ
	auto textureCount = pmxReader.ReadVariable<int>();
	std::vector<std::wstring> vec_texturePaths;
	vec_texturePaths.resize(textureCount);
	for (int i = 0; i < textureCount; i++) {
		int texturePathSize = pmxReader.ReadVariable<int>();
		std::wstring wPath;
		if (encodeType) {
			//string
			Util::MultiBytetoWString(pmxReader.ReadCharactor(texturePathSize), wPath);
			//vec_texturePaths
		}
		else {
			//wstring
			wPath = pmxReader.ReadWCharactor(texturePathSize / 2);
		}
		vec_texturePaths.at(i) = (wPath);
	}

	int materialCount = pmxReader.ReadVariable<int>();
	std::vector<std::pair< std::string, int>> vec_materialPaths;
	vec_materialPaths.resize(materialCount);

	auto materialDirPath = GlobalSettings::GetResourceDirectory() + arg_fileDirectory + "material/";
	auto dirRes = _mkdir(materialDirPath.c_str());

	//bmaの作成
	{
		for (int i = 0; i < materialCount; i++) {
			BinaryWriter bmaWriter;
			//ファイル生成、ファイルマジック、マテリアル名書き込み
			{
				std::wstring materialNameWstr;
				std::string materialNameStr;
				int materialNameCount = pmxReader.ReadVariable<int>();

				if (encodeType) {
					materialNameStr = pmxReader.ReadCharactor(materialNameCount);
					Util::MultiBytetoWString(materialNameStr, materialNameWstr);
				}
				else {
					materialNameWstr = pmxReader.ReadWCharactor(materialNameCount / 2);
					materialNameStr = Util::WStringToString(materialNameWstr);
				}
				bmaWriter.WriteStart(GlobalSettings::GetResourceDirectory() + arg_fileDirectory + "material/" + materialNameStr + ".bma");
				vec_materialPaths.at(i) = { "material/" + materialNameStr + ".bma" ,0 };

				bmaWriter.WriteCharactor("ButiMaterialData");
				bmaWriter.WriteVariable<float>(1.0f);
				bmaWriter.WriteVariable<char>(encodeType);
				bmaWriter.WriteVariable<int>(materialNameWstr.size());
				if (encodeType) {
					bmaWriter.WriteCharactor(materialNameStr);
				}
				else {
					bmaWriter.WriteWCharactor(materialNameWstr);
				}
			}
			//マテリアル名英
			{
				int materialEngNameByteSize = pmxReader.ReadVariable<int>();

				if (encodeType) {
					bmaWriter.WriteVariable<int>(materialEngNameByteSize);
					bmaWriter.WriteCharactor(pmxReader.ReadCharactor(materialEngNameByteSize));
				}
				else {
					bmaWriter.WriteVariable<int>(materialEngNameByteSize / 2);
					bmaWriter.WriteWCharactor(pmxReader.ReadWCharactor(materialEngNameByteSize / 2));
				}
			}
			//各パラメータ
			Vector4 diffuse = pmxReader.ReadVariable<Vector4>();
			Vector4 specular = Vector4(pmxReader.ReadVariable<Vector3>(), pmxReader.ReadVariable<float>());
			Vector4 ambient = Vector4(pmxReader.ReadVariable<Vector3>(), 0);
			char flag = pmxReader.ReadVariable<char>();

			if (specular.z > 1.0f) {
				specular.z = 1.0f;
			}

			//未使用パラメータ
			Vector4 edgeColor = pmxReader.ReadVariable<Vector4>();
			float edgeSize = pmxReader.ReadVariable<float>();

			//テクスチャパラメータ
			int textureIndex = pmxReader.ReadInt(textureIndexByteSize);
			int sphereTextureIndex = pmxReader.ReadInt(textureIndexByteSize);

			char sphereModeFlag = pmxReader.ReadVariable<char>();

			char toonFlag = pmxReader.ReadVariable<char>();

			if (!toonFlag) {
				int toonTexture = pmxReader.ReadInt(textureIndexByteSize);
			}
			else
			{
				char shareToon = pmxReader.ReadVariable<char>();
			}

			//テクスチャパラメータ書き込み
			{
				if (sphereTextureIndex < 0) {
					bmaWriter.WriteVariable<char>(1);
				}
				else
				{
					bmaWriter.WriteVariable<char>(2);
				}
				{
					std::wstring path;
					if (textureIndex != -1) {
						path = vec_texturePaths.at(textureIndex);
						bmaWriter.WriteVariable<int>(path.size());
						if (encodeType) {
							std::string pathStr;
							Util::WStringtoMultiByte(path, pathStr);
							bmaWriter.WriteCharactor(pathStr);
						}
						else {
							bmaWriter.WriteWCharactor(path);
						}
					}
					else
					{
						bmaWriter.WriteVariable<int>(-1);
					}
				}
				if (sphereTextureIndex >= 0)
				{
					std::wstring path = vec_texturePaths.at(sphereTextureIndex);
					bmaWriter.WriteVariable<int>(path.size());
					if (encodeType) {
						std::string pathStr;
						Util::WStringtoMultiByte(path, pathStr);
						bmaWriter.WriteCharactor(pathStr);
					}
					else {
						bmaWriter.WriteWCharactor(path);
					}
				}




			}
			//パラメータ書き込み
			{
				bmaWriter.WriteVariable<Vector4>(diffuse);
				bmaWriter.WriteVariable<Vector4>(specular);
				bmaWriter.WriteVariable<Vector4>(ambient);
				bmaWriter.WriteVariable<Vector4>(Vector4());

				bmaWriter.WriteVariable<char>(flag);
				bmaWriter.WriteVariable<char>(sphereModeFlag);
			}
			//コメント
			int commentByteSize = pmxReader.ReadVariable<int>();
			if (encodeType) {

				bmaWriter.WriteVariable<int>(commentByteSize);
				bmaWriter.WriteCharactor(pmxReader.ReadCharactor(commentByteSize));
			}
			else
			{

				bmaWriter.WriteVariable<int>(commentByteSize / 2);
				auto wComment = pmxReader.ReadWCharactor(commentByteSize / 2);
				bmaWriter.WriteWCharactor(wComment);
			}
			vec_materialPaths.at(i).second = pmxReader.ReadVariable<int>();
			bmaWriter.WriteEnd();
		}

		//マテリアル

		b3mWriter.WriteUINT(vec_materialPaths.size(), materialIndexByteSize);
		for (auto itr = vec_materialPaths.begin(); itr != vec_materialPaths.end(); itr++) {
			if (encodeType) {
				std::string filePath = itr->first;
				b3mWriter.WriteVariable<int>(filePath.size());
				b3mWriter.WriteCharactor(filePath);
				b3mWriter.WriteVariable<int>(itr->second);
			}
			else
			{
				std::wstring filePath;
				filePath = Util::StringToWString(itr->first);
				b3mWriter.WriteVariable<int>(filePath.size());
				b3mWriter.WriteWCharactor(filePath);
				b3mWriter.WriteVariable<int>(itr->second);
			}
		}


	}

	//ボーン

	int boneCount = pmxReader.ReadVariable<int>();
	b3mWriter.WriteInt(boneCount, boneIndexByteSize);
	for (int i = 0; i < boneCount; i++) {
		//ボーン名
		{
			int boneNameCount = pmxReader.ReadVariable<int>();

			if (encodeType) {
				std::string boneName = pmxReader.ReadCharactor(boneNameCount);
				b3mWriter.WriteVariable<int>(boneNameCount);
				b3mWriter.WriteCharactor(boneName);
			}
			else {
				std::wstring boneNameW = pmxReader.ReadWCharactor(boneNameCount / 2);
				b3mWriter.WriteVariable<int>(boneNameCount / 2);
				b3mWriter.WriteWCharactor(boneNameW);
				if (boneNameW == L"") {
					int t = 0;
				}
			}
		}
		//ボーン名英
		{
			int boneNameCount = pmxReader.ReadVariable<int>();

			if (encodeType) {
				std::string boneName = pmxReader.ReadCharactor(boneNameCount);
				b3mWriter.WriteVariable<int>(boneNameCount);
				b3mWriter.WriteCharactor(boneName);
			}
			else {
				std::wstring boneNameW = pmxReader.ReadWCharactor(boneNameCount / 2);
				b3mWriter.WriteVariable<int>(boneNameCount / 2);
				b3mWriter.WriteWCharactor(boneNameW);
				if (boneNameW == L"") {
					int t = 0;
				}
			}
		}
		auto pos = pmxReader.ReadVariable<Vector3>();

		auto parent = pmxReader.ReadInt(boneIndexByteSize);

		b3mWriter.WriteInt(parent, boneIndexByteSize);

		auto transformFloor = pmxReader.ReadVariable<int>();

		b3mWriter.WriteVariable<int>(transformFloor);

		b3mWriter.WriteVariable(pos);

		short boneFlag = (short)pmxReader.ReadInt(2);

		//boneFlag = BinaryHelper::SwapBytes(boneFlag);
		std::cout << std::hex << boneFlag << std::endl;

		b3mWriter.WriteVariable<short>(boneFlag);

		if (Util::GetBitFlag(boneFlag, 0)) {
			b3mWriter.WriteInt(pmxReader.ReadInt(boneIndexByteSize), boneIndexByteSize);
		}
		else {
			b3mWriter.WriteVariable(pmxReader.ReadVariable<Vector3>());
		}

		if (Util::GetBitFlag(boneFlag, 1)) {
		}

		if (Util::GetBitFlag(boneFlag, 2)) {
		}

		if (Util::GetBitFlag(boneFlag, 3)) {

		}


		if (Util::GetBitFlag(boneFlag, 4)) {
		}


		if (Util::GetBitFlag(boneFlag, 6)) {
		}

		if (Util::GetBitFlag(boneFlag, 7)) {
		}


		if (Util::GetBitFlag(boneFlag, 8) || Util::GetBitFlag(boneFlag, 9)) {

			b3mWriter.WriteInt(pmxReader.ReadInt(boneIndexByteSize), boneIndexByteSize);

			b3mWriter.WriteVariable<float>(pmxReader.ReadVariable<float>());
		}

		if (Util::GetBitFlag(boneFlag, 10)) {
			b3mWriter.WriteVariable<Vector3>(pmxReader.ReadVariable<Vector3>());
		}

		if (Util::GetBitFlag(boneFlag, 11)) {
			//localAxis
			b3mWriter.WriteVariable<Vector3>(pmxReader.ReadVariable<Vector3>());
			b3mWriter.WriteVariable<Vector3>(pmxReader.ReadVariable<Vector3>());
		}

		if (Util::GetBitFlag(boneFlag, 12)) {

		}

		if (Util::GetBitFlag(boneFlag, 13)) {
			b3mWriter.WriteVariable<int>(pmxReader.ReadVariable<int>());
		}

		if (Util::GetBitFlag(boneFlag, 5)) {

			b3mWriter.WriteInt(pmxReader.ReadInt(boneIndexByteSize), boneIndexByteSize);
			b3mWriter.WriteVariable<int>(pmxReader.ReadVariable<int>());
			b3mWriter.WriteVariable<float>(pmxReader.ReadVariable<float>());

			int ikLinkCount = pmxReader.ReadVariable<int>();
			b3mWriter.WriteVariable<int>(ikLinkCount);

			for (int j = 0; j < ikLinkCount; j++) {
				b3mWriter.WriteInt(pmxReader.ReadInt(boneIndexByteSize), boneIndexByteSize);
				char isLimmit = pmxReader.ReadVariable<char>();
				b3mWriter.WriteVariable<char>(isLimmit);
				if (isLimmit) {
					b3mWriter.WriteVariable<Vector3>(pmxReader.ReadVariable<Vector3>());
					b3mWriter.WriteVariable<Vector3>(pmxReader.ReadVariable<Vector3>());
				}
			}
		}
	}


	//モーフ

	int morphCount = pmxReader.ReadVariable<int>();

	b3mWriter.WriteInt(morphCount, morphIndexByteSize);

	for (int i = 0; i < morphCount; i++) {
		//モーフ名
		{
			int morphNameCount = pmxReader.ReadVariable<int>();

			if (encodeType) {
				std::string morphName = pmxReader.ReadCharactor(morphNameCount);
				b3mWriter.WriteVariable<int>(morphNameCount);
				b3mWriter.WriteCharactor(morphName);
			}
			else {
				std::wstring morphNameW = pmxReader.ReadWCharactor(morphNameCount / 2);
				b3mWriter.WriteVariable<int>(morphNameCount / 2);
				b3mWriter.WriteWCharactor(morphNameW);
			}
		}
		//モーフ名英
		{
			int morphNameCount = pmxReader.ReadVariable<int>();

			if (encodeType) {
				std::string morphName = pmxReader.ReadCharactor(morphNameCount);
				b3mWriter.WriteVariable<int>(morphNameCount);
				b3mWriter.WriteCharactor(morphName);
			}
			else {
				std::wstring morphNameW = pmxReader.ReadWCharactor(morphNameCount / 2);
				b3mWriter.WriteVariable<int>(morphNameCount / 2);
				b3mWriter.WriteWCharactor(morphNameW);
			}
		}
		//操作パネル
		char controllPanel = pmxReader.ReadVariable<char>();
		b3mWriter.WriteVariable(controllPanel);

		//モーフの種類
		Morph::MorphType morphType = (Morph::MorphType) pmxReader.ReadVariable<char>();

		b3mWriter.WriteVariable<char>((char)morphType);

		//モーフのオフセット
		int morphOffset = pmxReader.ReadVariable<int>();

		b3mWriter.WriteVariable<int>(morphOffset);

		switch (morphType)
		{
		case Morph::MorphType::Group: {

			for (int j = 0; j < morphOffset; j++) {
				Morph::Group_Morph::MorphVar var;
				var.morphIndex = pmxReader.ReadInt(morphIndexByteSize);
				var.morphPer = pmxReader.ReadVariable<float>();
				b3mWriter.WriteVariable(var);
			}
		}break;
		case Morph::MorphType::Vertex: {
			for (int j = 0; j < morphOffset; j++) {
				Morph::Vertex_Morph::MorphVar var;
				var.vartexIndex = pmxReader.ReadUINT(vertexIndexByteSize);
				var.offset = pmxReader.ReadVariable<Vector3>();
				b3mWriter.WriteVariable(var);
			}
		}break;
		case Morph::MorphType::Bone: {
			for (int j = 0; j < morphOffset; j++) {
				Morph::Bone_Morph::MorphVar var;
				var.boneIndex = pmxReader.ReadInt(boneIndexByteSize);
				b3mWriter.WriteVariable(var);
			}
		}break;
		case Morph::MorphType::UV: {
			for (int j = 0; j < morphOffset; j++) {
				Morph::UV_Morph::MorphVar var;
				var.vartexIndex = pmxReader.ReadUINT(vertexIndexByteSize);
				var.offset = pmxReader.ReadVariable<Vector4>();
				b3mWriter.WriteVariable(var);
			}
		}break;
		case Morph::MorphType::UVex01: {
			for (int j = 0; j < morphOffset; j++) {
				Morph::UV_Morph::MorphVar var;
				var.vartexIndex = pmxReader.ReadUINT(vertexIndexByteSize);
				var.offset = pmxReader.ReadVariable<Vector4>();
				b3mWriter.WriteVariable(var);
			}
		}break;
		case Morph::MorphType::UVex02: {
			for (int j = 0; j < morphOffset; j++) {
				Morph::UV_Morph::MorphVar var;
				var.vartexIndex = pmxReader.ReadUINT(vertexIndexByteSize);
				var.offset = pmxReader.ReadVariable<Vector4>();
				b3mWriter.WriteVariable(var);
			}
		}break;
		case Morph::MorphType::UVex03: {
			for (int j = 0; j < morphOffset; j++) {
				Morph::UV_Morph::MorphVar var;
				var.vartexIndex = pmxReader.ReadUINT(vertexIndexByteSize);
				var.offset = pmxReader.ReadVariable<Vector4>();
				b3mWriter.WriteVariable(var);
			}
		}break;
		case Morph::MorphType::UVex04: {
			for (int j = 0; j < morphOffset; j++) {
				Morph::UV_Morph::MorphVar var;
				var.vartexIndex = pmxReader.ReadUINT(vertexIndexByteSize);
				var.offset = pmxReader.ReadVariable<Vector4>();
				b3mWriter.WriteVariable(var);
			}
		}break;
		case Morph::MorphType::Material: {
			for (int j = 0; j < morphOffset; j++) {
				Morph::Material_Morph::MorphVar var;
				var.materialIndex = pmxReader.ReadInt(materialIndexByteSize);
				var.offsetCalculateType = pmxReader.ReadVariable<char>();
				var.Diffuse = pmxReader.ReadVariable<Vector4>();
				var.Specular = Vector4(pmxReader.ReadVariable<Vector3>(), pmxReader.ReadVariable<float>());
				var.Ambient = Vector4(pmxReader.ReadVariable<Vector3>(), 1);
				var.edgeColor = pmxReader.ReadVariable<Vector4>();
				var.edgeSize = pmxReader.ReadVariable<float>();
				var.textureCoefficient = pmxReader.ReadVariable<Vector4>();
				var.sphereTextureCoefficient = pmxReader.ReadVariable<Vector4>();
				var.toonTextureCoefficient = pmxReader.ReadVariable<Vector4>();
				b3mWriter.WriteVariable(var);
			}
		}break;
		case Morph::MorphType::Flip: {
			for (int j = 0; j < morphOffset; j++) {
				Morph::Flip_Morph::MorphVar var;
				var.morphIndex = pmxReader.ReadInt(morphIndexByteSize);
				var.morphPer = pmxReader.ReadVariable<float>();
				b3mWriter.WriteVariable(var);
			}
		}break;
		}


	}


	pmxReader.ReadEnd();
	b3mWriter.WriteEnd();
}

void ButiEngine::ModelFileConverter::VMDtoBMD(const std::string& arg_vmdFileName, const std::string& arg_bmdFileName, const std::string& arg_fileDirectory)
{
	BinaryReader vmdReader;

	vmdReader.ReadStart(GlobalSettings::GetResourceDirectory() + arg_fileDirectory + arg_vmdFileName);

	std::string vmdHeader = vmdReader.ReadCharactor(30);
	std::wstring vmdHeader_modelName = vmdReader.ReadWCharactor(10);

	auto motionCount = vmdReader.ReadVariable<UINT>();
	std::map<std::wstring, std::vector< MotionKeyFrameData>> map_motionDatas;
	for (int i = 0; i < motionCount; i++) {
		std::wstring boneName = vmdReader.ReadShift_jis(15);

		MotionKeyFrameData motionData;

		motionData.endFrame = vmdReader.ReadVariable<UINT>();

		auto px = vmdReader.ReadVariable<float>();

		motionData.pose.position.x = px;

		auto py = vmdReader.ReadVariable<float>();

		motionData.pose.position.y = py;

		auto pz = vmdReader.ReadVariable<float>();

		motionData.pose.position.z = pz;

		auto rx = vmdReader.ReadVariable<float>();

		motionData.pose.rotation.x = rx;
		auto ry = vmdReader.ReadVariable<float>();

		motionData.pose.rotation.y = ry;
		auto rz = vmdReader.ReadVariable<float>();

		motionData.pose.rotation.z = rz;
		auto rw = vmdReader.ReadVariable<float>();

		motionData.pose.rotation.w = rw;

		auto data = vmdReader.ReadVariable< Byte64>();
		motionData.larp = LarpData(data.data);


		if (map_motionDatas.count(boneName)) {
			map_motionDatas.at(boneName).push_back(motionData);
		}
		else {
			map_motionDatas.emplace(boneName, std::vector< MotionKeyFrameData> { motionData });
		}
	}


	auto skinCount = vmdReader.ReadVariable<UINT>();
	for (int i = 0; i < skinCount; i++) {
		vmdReader.Skip(23);
	}

	auto cameraCount = vmdReader.ReadVariable<UINT>();
	for (int i = 0; i < cameraCount; i++) {
		vmdReader.Skip(61);
	}

	auto lightCount = vmdReader.ReadVariable<UINT>();
	for (int i = 0; i < lightCount; i++) {
		vmdReader.Skip(28);
	}

	auto selfShadowDataCount = vmdReader.ReadVariable<UINT>();
	for (int i = 0; i < selfShadowDataCount; i++) {
		vmdReader.Skip(9);
	}

	/*auto IKDataCount = vmdReader.ReadVariable<UINT>();
	std::vector<IKActive> vec_ikData;
	vec_ikData.reserve(IKDataCount);
	for (int i = 0; i < IKDataCount; i++) {
		int frameNum = vmdReader.ReadVariable<int>();
		bool isShow = vmdReader.ReadVariable<bool>();
		int dataCount = vmdReader.ReadVariable < UINT>();

		for (int j = 0; j < dataCount; j++) {
			IKActive add;
			add.boneName = vmdReader.ReadShift_jis(20);

			add.isActive = vmdReader.ReadVariable<bool>();

			vec_ikData.push_back(add);
		}
	}*/

	vmdReader.ReadEnd();
	CreateBMD(map_motionDatas, arg_bmdFileName, arg_fileDirectory);
}

void ButiEngine::ModelFileConverter::CreateBMD(std::map<std::wstring, std::vector<MotionKeyFrameData>>& arg_map_motionDatas, const std::string& arg_bmdFileName, const std::string& arg_fileDirectory)
{
	BinaryWriter bmdWriter;
	bmdWriter.WriteStart(GlobalSettings::GetResourceDirectory() + arg_fileDirectory + arg_bmdFileName);
	std::string b3mMagic = "ButiMotionData";
	bmdWriter.WriteCharactor(b3mMagic);
	bmdWriter.WriteVariable<float>(1.0f);

	//動くボーンの数を仮で設定
	auto boneCountPos = bmdWriter.GetNowPos();
	bmdWriter.WriteVariable<UINT>(0);

	UINT boneCount = 0;



	for (auto itr = arg_map_motionDatas.begin(); itr != arg_map_motionDatas.end(); itr++) {
		if (itr->second.size() <= 1) {
			continue;
		}
		{
			std::sort(itr->second.begin(), itr->second.end(), [](const MotionKeyFrameData keyFrameData, const MotionKeyFrameData keyFrameData_other) {return keyFrameData.endFrame < keyFrameData_other.endFrame; });
		}


		boneCount++;

		//ボーン名
		bmdWriter.WriteVariable<UINT>(itr->first.size());
		bmdWriter.WriteWCharactor(itr->first);

		//モーション数
		bmdWriter.WriteVariable<UINT>(itr->second.size());
		for (auto motionItr = itr->second.begin(); motionItr != itr->second.end(); motionItr++) {
			bmdWriter.WriteVariable<MotionKeyFrameData>(*motionItr);
		}

	}

	bmdWriter.Jump(boneCountPos);
	bmdWriter.WriteVariable<UINT>(boneCount);

	bmdWriter.WriteEnd();
}

