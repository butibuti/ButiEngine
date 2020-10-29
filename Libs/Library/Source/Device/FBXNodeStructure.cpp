#include "stdafx.h"
#include"Header/Device/FBXNodeStructure.h"
#include "..\..\Header\Device\FBXNodeStructure.h"

bool ButiEngine::FBXAnalyze::FBXNodeStructure::NullRecordCheck()
{
	if ((!endOffset) && (!propertyCount) && (!propertyListLen) && (!recordName.size())) {
		return true;
	}
	return false;
}

ButiEngine::Vector3 ButiEngine::FBXAnalyze::FBXNodeStructure::GetVector3(const FBXGlobalSettings& settings)
{
	return Vector3(GetProperty<FBXAnalyze::FBXNode_DoubleProperty>(settings.coordAxis)->nodeProperty, GetProperty<FBXAnalyze::FBXNode_DoubleProperty>(settings.upAxis)->nodeProperty, GetProperty<FBXAnalyze::FBXNode_DoubleProperty>(settings.frontAxis)->nodeProperty);
}

ButiEngine::Vector3 ButiEngine::FBXAnalyze::FBXNodeStructure::GetRawVector3()
{
	return Vector3(GetProperty<FBXAnalyze::FBXNode_DoubleProperty>(0)->nodeProperty, GetProperty<FBXAnalyze::FBXNode_DoubleProperty>(1)->nodeProperty, GetProperty<FBXAnalyze::FBXNode_DoubleProperty>(2)->nodeProperty);
}

ButiEngine::Vector2 ButiEngine::FBXAnalyze::FBXNodeStructure::GetVector2()
{
	return Vector2(GetProperty<FBXAnalyze::FBXNode_DoubleProperty>(0)->nodeProperty, GetProperty<FBXAnalyze::FBXNode_DoubleProperty>(1)->nodeProperty);
}

float ButiEngine::FBXAnalyze::FBXNodeStructure::GetFloat()
{
	return GetProperty<FBXAnalyze::FBXNode_DoubleProperty>(0)->nodeProperty;
}

std::vector<std::shared_ptr<ButiEngine::FBXAnalyze::FBXNodeStructure>> ButiEngine::FBXAnalyze::FBXNodeStructure::SerchChildNode(const std::string & arg_nodeName)
{
	std::vector<std::shared_ptr<ButiEngine::FBXAnalyze::FBXNodeStructure>> out;

	out.resize(multimap_childNodes .count(arg_nodeName));

	auto sercheRange = multimap_childNodes.equal_range(arg_nodeName);
	int i = 0;
	for (auto itr = sercheRange.first; itr != sercheRange.second; itr++, i++) {
		out.at(i) = itr->second;
	}

	return out;
}

std::vector<std::shared_ptr<ButiEngine::FBXAnalyze::FBXNodeStructure>> ButiEngine::FBXAnalyze::FBXNodeStructure::SerchChildNodeWithStringProperty(const std::string & arg_nodeName, const std::string & arg_stringProp, const UINT arg_propertyIndex)
{
	auto child = SerchChildNode(arg_nodeName);
	std::vector< std::shared_ptr<FBXNodeStructure>> out;
	for (auto itr = child.begin(); itr != child.end(); itr++) {
		auto strProp = (*itr)->GetProperty<FBXNode_StringProperty>(arg_propertyIndex);
		if (!strProp) {
			continue;
		}
		if (arg_stringProp == strProp->nodeProperty) {
			
			out.push_back(*itr);
		}

	}

	return out;
}

std::shared_ptr<ButiEngine::FBXAnalyze::FBXNodeStructure> ButiEngine::FBXAnalyze::FBXNodeStructure::GetChildNode(const std::string & arg_nodeName, const UINT arg_index)
{
	std::shared_ptr<FBXNodeStructure> out = nullptr;

	if (multimap_childNodes.count(arg_nodeName)<(arg_index+1)) {
		return out;
	}

	auto serch = multimap_childNodes.equal_range(arg_nodeName).first;


	for (int i = 0; i < arg_index; i++) {
		serch++;
	}
	out = serch->second;

	return out;
}

std::shared_ptr<ButiEngine::FBXAnalyze::FBXNodeStructure> ButiEngine::FBXAnalyze::FBXNodeStructure::GetChildNodeWithStringProperty(const std::string & arg_nodeName, const std::string & arg_stringProp, const UINT arg_propertyIndex, const UINT arg_index)
{
	auto child = SerchChildNodeWithStringProperty(arg_nodeName,arg_stringProp,arg_propertyIndex);

	std::shared_ptr<FBXNodeStructure> out = nullptr;
	
	if (child.size() > arg_index) {
		out = child.at(arg_index);
	}

	return out;
}

std::shared_ptr<ButiEngine::FBXAnalyze::FBXNodeStructure> ButiEngine::FBXAnalyze::FBXNodeStructure::GetChildPropertyNode(const std::string & arg_nodeName, const std::string & arg_propertyName)
{
	std::shared_ptr<FBXNodeStructure> out = nullptr;

	if (!multimap_childNodes.count(arg_nodeName)) {
		return out;
	}

	auto serch = multimap_childNodes.equal_range(arg_nodeName);

	for (auto itr = serch.first; itr != serch.second; itr++) {
		auto strProp = itr->second->GetProperty<FBXNode_StringProperty>();
		if (strProp&&strProp->nodeProperty==arg_propertyName) {

			out = itr->second;

		}
	}
	return out;
}

std::vector<std::shared_ptr<ButiEngine::FBXAnalyze::FBXNodeStructure>> ButiEngine::FBXAnalyze::FBXScene::SerchNode(const std::string & arg_serchNodeName)
{
	std::vector<std::shared_ptr<ButiEngine::FBXAnalyze::FBXNodeStructure>> out;

	out.resize( multimap_nodeRecords.count(arg_serchNodeName));

	auto sercheRange= multimap_nodeRecords.equal_range(arg_serchNodeName);
	int i = 0;
	for (auto itr = sercheRange.first; itr != sercheRange.second; itr++,i++) {
		out.at(i) = itr->second;
	}

	return out;
}

std::vector<std::shared_ptr<ButiEngine::FBXAnalyze::FBXNodeStructure>> ButiEngine::FBXAnalyze::FBXScene::SerchNode(const std::string & arg_serchNodeName, const UINT searchPropertyCount)
{
	std::vector<std::shared_ptr<ButiEngine::FBXAnalyze::FBXNodeStructure>> out;


	auto sercheRange = multimap_nodeRecords.equal_range(arg_serchNodeName);
	int i = 0;
	for (auto itr = sercheRange.first; itr != sercheRange.second; itr++, i++) {
		if(itr->second->multimap_childNodes.size()==searchPropertyCount)
		out.push_back( itr->second);
	}

	return out;
}

std::vector<std::shared_ptr<ButiEngine::FBXAnalyze::FBXNodeStructure>> ButiEngine::FBXAnalyze::FBXScene::SerchNodeWithStringProperty(const std::string & arg_nodeName, const std::string & arg_stringProp, const UINT arg_propertyIndex)
{
	auto child = SerchNode(arg_nodeName);
	std::vector< std::shared_ptr<FBXNodeStructure>> out;
	for (auto itr = child.begin(); itr != child.end(); itr++) {
		auto strProp = (*itr)->GetProperty<FBXNode_StringProperty>(arg_propertyIndex);
		if (!strProp) {
			continue;
		}
		if (arg_stringProp == strProp->nodeProperty) {

			out.push_back(*itr);
		}

	}

	return out;
}

std::vector<ButiEngine::Vector4> ButiEngine::FBXAnalyze::FBXNode_FloatArrayProperty::CreateVector4()
{
	std::vector<Vector4> out;

	out.resize(nodeProperty.size() / 4);

	for (int i = 0; i < out.size(); i++) {
		out.at(i) = Vector4(nodeProperty.at(i * 4 + 0), nodeProperty.at(i * 4 + 1), nodeProperty.at(i * 4 + 2), nodeProperty.at(i * 4 + 3));
	}


	return out;
}

std::vector<ButiEngine::Vector3> ButiEngine::FBXAnalyze::FBXNode_FloatArrayProperty::CreateVector3()
{
	std::vector<Vector3> out;

	out.resize(nodeProperty.size() / 3);

	for (int i = 0; i < out.size(); i++) {
		out.at(i) = Vector3(nodeProperty.at(i * 3 + 1), nodeProperty.at(i * 3 + 2), nodeProperty.at(i * 3 + 0));
	}

	return out;
}

std::vector<ButiEngine::Vector2> ButiEngine::FBXAnalyze::FBXNode_FloatArrayProperty::CreateVector2()
{
	std::vector<Vector2> out;

	out.resize(nodeProperty.size() / 2);

	for (int i = 0; i < out.size(); i++) {
		out.at(i) = Vector2(nodeProperty.at(i * 2 + 0), nodeProperty.at(i * 2 + 1));
	}

	return out;
}

std::vector<ButiEngine::Vector4> ButiEngine::FBXAnalyze::FBXNode_DoubleArrayProperty::CreateVector4()
{
	std::vector<Vector4> out;

	out.resize(nodeProperty.size() / 4);

	for (int i = 0; i < out.size(); i++) {
		out.at(i) = Vector4((float)nodeProperty.at(i * 4 + 0), (float)nodeProperty.at(i * 4 + 1), (float)nodeProperty.at(i * 4 + 2), (float)nodeProperty.at(i * 4 + 3));
	}


	return out;
}

std::vector<ButiEngine::Vector3> ButiEngine::FBXAnalyze::FBXNode_DoubleArrayProperty::CreateVector3(const FBXGlobalSettings& settings, const Matrix4x4* arg_transform)
{
	std::vector<Vector3> out;

	out.resize(nodeProperty.size() / 3);

	for (int i = 0; i<out.size(); i++) {
		out.at(i) = Vector3((float)-1*settings.coordAxisSign* nodeProperty.at(i*3+settings.coordAxis), (float)settings.upAxisSign* nodeProperty.at(i * 3 + settings.upAxis),settings.frontAxisSign* (float)nodeProperty.at(i * 3 + settings.frontAxis));
		if (arg_transform) {
			out.at(i) *= *arg_transform;
		}
	}

	return out;
}

std::vector<ButiEngine::Vector3> ButiEngine::FBXAnalyze::FBXNode_DoubleArrayProperty::CreateVector3Half_Latter()
{
	std::vector<Vector3> out;

	out.resize(nodeProperty.size()/2 / 3 );

	int halfOffset = ((int)nodeProperty.size()) / 2;

	for (int i = 0; i < out.size(); i++) {
		out.at(i) = Vector3((float)nodeProperty.at(i * 3+halfOffset + 1), (float)nodeProperty.at(i * 3+halfOffset + 2), (float)nodeProperty.at(i * 3+halfOffset + 0));
	}

	return out;
}

std::vector<ButiEngine::Vector2> ButiEngine::FBXAnalyze::FBXNode_DoubleArrayProperty::CreateVector2()
{
	std::vector<Vector2> out;

	out.resize(nodeProperty.size() /2);

	for (int i = 0; i < out.size(); i++) {
		out.at(i) =Vector2((float)nodeProperty.at(i * 2 + 0),1.0f- (float)nodeProperty.at(i * 2 + 1));
	}

	return out;
}

std::vector<ButiEngine::Vector2> ButiEngine::FBXAnalyze::FBXNode_DoubleArrayProperty::CreateVector2Half_Latter()
{
	std::vector<Vector2> out;

	out.resize(nodeProperty.size() / 2 / 2);

	int halfOffset = nodeProperty.size() / 2;

	for (int i = 0; i < out.size(); i++) {
		out.at(i) = Vector2((float)nodeProperty.at(i * 2 + halfOffset + 0), (float)nodeProperty.at(i * 2 + halfOffset + 1));
	}

	return out;
}

void ButiEngine::FBXAnalyze::FBXAttributeNode::SetAttributeType()
{
	auto type = SerchChildNode("TypeFlags");

	if (!type.size()) {
		return;
	}

	auto typeStr =(type.at(0)->multimap_properties.find(FBXPropertyDataType::String)->second)->GetThis<FBXNode_StringProperty>()->nodeProperty;

	if (typeStr == "Null") {
		attributeType = NodeAttributeType::eNull;
	}
	else if (typeStr == "Unknown") {
		attributeType = NodeAttributeType::eUnknown;
	}
	else if (typeStr == "Marker") {
		attributeType = NodeAttributeType::eMarker;
	}
	else if (typeStr == "Skeleton") {
		attributeType = NodeAttributeType::eSkeleton;
	}
	else if (typeStr == "Mesh") {
		attributeType = NodeAttributeType::eMesh;
	}
	else if (typeStr == "Nurbs") {
		attributeType = NodeAttributeType::eNurbs;
	}
	else if (typeStr == "Patch") {
		attributeType = NodeAttributeType::ePatch;
	}
	else if (typeStr == "Camera") {
		attributeType = NodeAttributeType::eCamera;
	}
	else if (typeStr == "CameraStereo") {
		attributeType = NodeAttributeType::eCameraStereo;
	}
	else if (typeStr == "CameraSwitcher") {
		attributeType = NodeAttributeType::eCameraSwitcher;
	}
	else if (typeStr == "Light") {
		attributeType = NodeAttributeType::eLight;
	}
	else if (typeStr == "OpticalReference") {
		attributeType = NodeAttributeType::eOpticalReference;
	}
	else if (typeStr == "OpticalMarker") {
		attributeType = NodeAttributeType::eOpticalMarker;
	}
	else if (typeStr == "NurbsCurve") {
		attributeType = NodeAttributeType::eNurbsCurve;
	}
	else if (typeStr == "TrimNurbsSurface") {
		attributeType = NodeAttributeType::eTrimNurbsSurface;
	}
	else if (typeStr == "Boundary") {
		attributeType = NodeAttributeType::eBoundary;
	}
	else if (typeStr == "NurbsSurface") {
		attributeType = NodeAttributeType::eNurbsSurface;
	}
	else if (typeStr == "Shape") {
		attributeType = NodeAttributeType::eShape;
	}
	else if (typeStr == "LODGroup") {
		attributeType = NodeAttributeType::eLODGroup;
	}
	else if (typeStr == "SubDiv") {
		attributeType = NodeAttributeType::eSubDiv;
	}
	else if (typeStr == "CachedEffect") {
		attributeType = NodeAttributeType::eCachedEffect;
	}
	else if (typeStr == "Line") {
		attributeType = NodeAttributeType::eLine;
	}
	else
	{
		attributeType = NodeAttributeType::eUnknown;
	}
}

std::vector<int> ButiEngine::FBXAnalyze::FBXNode_IntArrayProperty::GetPolygonVertexIndex()
{
	std::vector<int> out;
	out.reserve(nodeProperty.size());
	if (nodeProperty.at(0) == nodeProperty.at(1)) {
		auto itr = nodeProperty.begin();
		itr += nodeProperty.size()/2;
		std::copy(itr, nodeProperty.end(), std::back_inserter(out));
	}
	else {
		std::copy(nodeProperty.begin(), nodeProperty.end(),std::back_inserter( out));
	}
	out.shrink_to_fit();
	return out;
}

std::vector<int> ButiEngine::FBXAnalyze::FBXNode_IntArrayProperty::GetUVIndex()
{
	std::vector<int> out;
	out.reserve(nodeProperty.size());
	if (nodeProperty.at(0) == nodeProperty.at(1)) {
		auto itr = nodeProperty.begin();
		itr += nodeProperty.size() / 2;
		std::copy(itr, nodeProperty.end(), std::back_inserter(out));
	}
	else {
		std::copy(nodeProperty.begin(), nodeProperty.end(), std::back_inserter(out));
	}
	out.shrink_to_fit();
	return out;
}



ButiEngine::Matrix4x4 ButiEngine::FBXAnalyze::FBXModelNode::GetTransform(const FBXGlobalSettings & settings)
{
	auto propertiies70Node = GetChildNode("Properties70");

	Vector3 position;
	Vector3 rotation;
	Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);

	auto translationPropNode = propertiies70Node->GetChildPropertyNode("P", "Lcl Translation");
	auto rotationPropNode = propertiies70Node->GetChildPropertyNode("P", "Lcl Rotation");
	auto scalePropNode = propertiies70Node->GetChildPropertyNode("P", "Lcl Scaling");

	if (translationPropNode) {
		position = translationPropNode->GetVector3(settings);
	}
	if (rotationPropNode) {
		rotation = rotationPropNode->GetVector3(settings);
	}
	if (scalePropNode) {
		scale = scalePropNode->GetVector3(settings);
	}

	return ObjectFactory::Create<Transform>(position, rotation, scale)->GetMatrix();
}

std::shared_ptr<  ButiEngine::FBXAnalyze::FBXPolygonVertexIndexNode> ButiEngine::FBXAnalyze::FBXGeometryNode::GetPolygonVertexIndexNode()
{
	return GetChildNode("PolygonVertexIndex")->GetThis<FBXPolygonVertexIndexNode>();
}

ButiEngine::FBXAnalyze::FBXGlobalSettings ButiEngine::FBXAnalyze::FBXGlobalSettingsNode::GetSettings()
{
	FBXGlobalSettings out;

	auto propertyNode = GetChildNode("Properties70");

	out.upAxis = propertyNode->GetChildNodeWithStringProperty("P", "UpAxis", 0)->GetProperty<FBXNode_IntProperty>()->nodeProperty;

	out.frontAxis = propertyNode->GetChildNodeWithStringProperty("P", "FrontAxis", 0)->GetProperty<FBXNode_IntProperty>()->nodeProperty;

	out.coordAxis = propertyNode->GetChildNodeWithStringProperty("P", "CoordAxis", 0)->GetProperty<FBXNode_IntProperty>()->nodeProperty;
	
	out.upAxisSign = propertyNode->GetChildNodeWithStringProperty("P", "UpAxisSign", 0)->GetProperty<FBXNode_IntProperty>()->nodeProperty;

	out.frontAxisSign = propertyNode->GetChildNodeWithStringProperty("P", "FrontAxisSign", 0)->GetProperty<FBXNode_IntProperty>()->nodeProperty;

	out.coordAxisSign = propertyNode->GetChildNodeWithStringProperty("P", "CoordAxisSign", 0)->GetProperty<FBXNode_IntProperty>()->nodeProperty;

	out.frameRate =(float) propertyNode->GetChildNodeWithStringProperty("P", "CustomFrameRate", 0)->GetProperty<FBXNode_DoubleProperty>()->nodeProperty;


	return out;
}


void ButiEngine::FBXAnalyze::FBXPolygonVertexIndexNode::NodeInitialize()
{
	polygonIndex = GetProperty<FBXAnalyze::FBXNode_IntArrayProperty>()->GetPolygonVertexIndex();
	multimap_properties.clear();
}

std::vector<int> ButiEngine::FBXAnalyze::FBXPolygonVertexIndexNode::GetTriPolygonIndex(const int offset)
{
	std::vector<int> out = triPolygonIndex;

	for (auto itr = out.begin(); itr != out.end(); itr++) {
		(*itr) += offset;
	}
	return out;
}

std::vector<int>& ButiEngine::FBXAnalyze::FBXPolygonVertexIndexNode::CreateTriPolygonIndexByPolyMaterial( const std::vector<int>& arg_vec_materialIndex,const UINT arg_materialCount, std::vector<int>& arg_materialIndexSize)
{
	triPolygonIndex= VertexIndexUtill::ConvertTriPolygon(polygonIndex,arg_vec_materialIndex, arg_materialCount,arg_materialIndexSize);

	return triPolygonIndex;
}

std::vector<int>& ButiEngine::FBXAnalyze::FBXPolygonVertexIndexNode::CreateTriPolygonIndexByMonoMaterial()
{
	triPolygonIndex = VertexIndexUtill::ConvertTriPolygon(polygonIndex);

	return triPolygonIndex;
}

std::string ButiEngine::FBXAnalyze::FBXBoneNode::GetName()
{

	return	GetProperty<FBXNode_StringProperty>()->nodeProperty;
}

ButiEngine::Vector3 ButiEngine::FBXAnalyze::FBXBoneNode::GetPosition(const std::vector<std::shared_ptr<FBXBoneNode>>& arg_bones, const FBXGlobalSettings& settings)
{
	auto p = GetChildNode("Properties70") ->GetChildNodeWithStringProperty("P", "Lcl Translation", 0);
	auto output = Vector3();
	if(p)
		output= Vector3(p->GetProperty<FBXNode_DoubleProperty>(settings.coordAxis)->nodeProperty, p->GetProperty<FBXNode_DoubleProperty>(settings.upAxis)->nodeProperty, settings.frontAxisSign * p->GetProperty<FBXNode_DoubleProperty>(settings.frontAxis)->nodeProperty);

	output *= GetRotation(arg_bones, settings);
	if (parentBoneIndex >= 0) {
		output += arg_bones.at(parentBoneIndex)->GetPosition(arg_bones,settings);
	}

	return output;
}

ButiEngine::Matrix4x4 ButiEngine::FBXAnalyze::FBXBoneNode::GetRotation(const std::vector<std::shared_ptr<FBXBoneNode>>& arg_bones, const FBXGlobalSettings& settings)
{
	auto p = GetChildNode("Properties70")->GetChildNodeWithStringProperty("P", "Lcl Rotation", 0);
	Matrix4x4 out;
	
	if(p)
	out=DirectX::XMMatrixRotationX(
		DirectX::XMConvertToRadians(p->GetProperty<FBXNode_DoubleProperty>(settings.coordAxis)->nodeProperty)
	) *
		DirectX::XMMatrixRotationY(
			DirectX::XMConvertToRadians(settings.upAxisSign * p->GetProperty<FBXNode_DoubleProperty>(settings.upAxis)->nodeProperty)
		) *
		DirectX::XMMatrixRotationZ(
			DirectX::XMConvertToRadians(settings.frontAxisSign * p->GetProperty<FBXNode_DoubleProperty>(settings.frontAxis)->nodeProperty)
		);
	if (parentBoneIndex >= 0) {
		out= (XMMATRIX)out*(XMMATRIX) arg_bones.at(parentBoneIndex)->GetRotation(arg_bones, settings);
	}
	return out;

}

std::vector<double> ButiEngine::FBXAnalyze::FBXDeformerNode::GetWeight()
{
	return GetChildNode("Weights")->GetProperty<FBXNode_DoubleArrayProperty>()->nodeProperty;
}

std::vector<int> ButiEngine::FBXAnalyze::FBXDeformerNode::GetBoneIndex()
{
	return GetChildNode("Indexes")->GetProperty<FBXNode_IntArrayProperty>()->nodeProperty;
}
