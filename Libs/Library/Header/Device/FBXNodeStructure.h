#pragma once
#include"stdafx.h"
namespace ButiEngine {
	namespace FBXAnalyze {
		enum class FBXPropertyDataType {
			Short = 'Y', Bool = 'C', Int = 'I', Float = 'F', Double = 'D',Long='L',
			FloatArray = 'f', DoubleArray = 'd', LongArray = 'l', IntArray = 'i', BoolArray = 'b',
			String = 'S', RawData='R',
		};
		struct FBXProperty :IObject {
			void Initialize()override {};
			void PreInitialize()override {};
		};
		struct FBXGlobalSettings {
			char upAxis = 1;
			char upAxisSign = 1;
			char frontAxis = 2;
			char frontAxisSign = -1;
			char coordAxis = 0;
			char coordAxisSign = -1;
			float frameRate;
		};
		struct FBXNodeStructure :public IObject
		{
		public:
			std::unordered_multimap<std::string, std::shared_ptr< FBXNodeStructure>> multimap_childNodes;
			std::string recordName;
			UINT endOffset;
			UINT propertyListLen;
			UINT propertyCount;
			bool isEmpty = false;
			bool isParent=false;
			void Initialize()override {}
			void PreInitialize()override{}
			bool NullRecordCheck();
			Vector3 GetVector3(const FBXGlobalSettings& settings);
			Vector3 GetRawVector3();
			Vector2 GetVector2();
			virtual void NodeInitialize() {}
			float GetFloat();
			std::weak_ptr< FBXNodeStructure>parent;
			std::multimap<FBXPropertyDataType,std::shared_ptr<FBXProperty>> multimap_properties;
			std::vector < std::shared_ptr<FBXNodeStructure>> SerchChildNode(const std::string& arg_nodeName);
			std::vector < std::shared_ptr<FBXNodeStructure>> SerchChildNodeWithStringProperty(const std::string& arg_nodeName, const std::string& arg_stringProp, const UINT arg_propertyIndex=0);
			std::shared_ptr<FBXNodeStructure> GetChildNode(const std::string& arg_nodeName, const UINT arg_index = 0);
			std::shared_ptr<FBXNodeStructure> GetChildNodeWithStringProperty(const std::string& arg_nodeName, const std::string& arg_stringProp, const UINT arg_propertyIndex, const UINT arg_index = 0);
			std::shared_ptr< FBXNodeStructure> GetChildPropertyNode(const std::string& arg_nodeName, const std::string& arg_propertyName);
			template <typename T>
			inline std::shared_ptr<T> GetProperty(const UINT arg_index=0) {

				FBXPropertyDataType type = T::GetType();
				if (!multimap_properties.count(type)) {
					return nullptr;
				}
				auto itrs= multimap_properties.find(type);

				
				for (int i = 0; i < arg_index; i++) {
					itrs++;
				}


				return (itrs)->second->GetThis<T>();
			}

		private:

		};

		enum class LinkType {
			OO, OP, PP
		};

		struct FBXNodeLinkInfo :public IObject {
			void Initialize()override {}
			void PreInitialize()override {}
			std::shared_ptr<FBXNodeStructure> serverNode;
			std::shared_ptr<FBXNodeStructure> resieverNode;
			std::string info="none prop";
			LinkType linkType;
		};
		struct FBXConnectionNode :public FBXNodeStructure {
		public:
			template<class T>
			std::shared_ptr< FBXNodeStructure>GetServeNode(std::vector< std::shared_ptr< T>>& arg_toNode);

			template<class T>
			std::shared_ptr< FBXNodeStructure> GetResieveNode(std::vector<std::shared_ptr< T>>& arg_byNodes);
			
			template<class T,class U>
			std::shared_ptr<FBXNodeLinkInfo> GetPairNode(std::vector<std::shared_ptr< T>>& arg_byNodes, std::vector<std::shared_ptr<U>>& arg_toNodes);
			
		};
		
		enum class NodeAttributeType {
			eUnknown,eNull,	eMarker,eSkeleton,eMesh,
			eNurbs,	ePatch,	eCamera,eCameraStereo,eCameraSwitcher,
			eLight,	eOpticalReference,eOpticalMarker,eNurbsCurve,eTrimNurbsSurface,
			eBoundary,eNurbsSurface,eShape,eLODGroup,eSubDiv,eCachedEffect,eLine,
		};

		struct FBXNode_End :public FBXNodeStructure {

		};


		struct FBXAttributeNode :public FBXNodeStructure {
			NodeAttributeType attributeType;
			void SetAttributeType();
		};
		struct FBXGeometryNode;
		struct FBXModelNode;
		struct FBXGlobalSettingsNode :public FBXNodeStructure {
			FBXGlobalSettings GetSettings();
		};

		struct FBXTextureNode :public FBXNodeStructure {

		};
		struct FBXMaterialNode :public FBXNodeStructure {

			int index;
			std::vector< std::weak_ptr<FBXGeometryNode>> geometryNodes; 
			std::weak_ptr<FBXTextureNode> diffuseTextureNode;
			std::vector< std::weak_ptr<FBXModelNode>>  modelNodes;
		};
		struct FBXModelNode :public FBXNodeStructure {
			std::vector< std::pair<int, UINT>> vec_materialNodes;
			std::weak_ptr<FBXGeometryNode> geometryNode;
			Matrix4x4 GetTransform(const FBXGlobalSettings& settings);
		};
		struct FBXBoneNode :public FBXNodeStructure {
			std::string GetName();
			Vector3 GetPosition(const std::vector<std::shared_ptr<FBXBoneNode>>& arg_bones, const FBXGlobalSettings& settings);
			Matrix4x4 GetRotation(const std::vector<std::shared_ptr<FBXBoneNode>>& arg_bones, const FBXGlobalSettings& settings);
			int parentBoneIndex = -1;
			int boneIndex = -1;
		};
		struct FBXDeformerNode :public FBXNodeStructure {
			std::vector<double> GetWeight();
			std::vector<int> GetBoneIndex();
			int boneIndex = -1;
		};
		struct FBXPolygonVertexIndexNode;

		struct FBXGeometryNode :public FBXNodeStructure {

			std::weak_ptr<FBXModelNode> modelNode;
			std::shared_ptr< FBXPolygonVertexIndexNode> GetPolygonVertexIndexNode();
		};

		struct FBXPolygonVertexIndexNode :public FBXNodeStructure {
		private:
			std::vector<int> polygonIndex;
			std::vector<int> triPolygonIndex;
		public:
			void NodeInitialize()override;
			std::vector<int>& GetPolygonIndex() {
				return polygonIndex;
			}
			std::vector<int>& GetTriPolygonIndex() {
				return triPolygonIndex;
			}
			std::vector<int> GetTriPolygonIndex(const int offset);

			std::vector<int>& CreateTriPolygonIndexByPolyMaterial(const std::vector<int>& arg_vec_materialIndex, const UINT arg_materialCount, std::vector<int>& arg_materialIndexSize);
			std::vector<int>& CreateTriPolygonIndexByMonoMaterial();
		};

		struct FBXNode_IntProperty :public FBXProperty {
			int nodeProperty = 0;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::Int;
			}
		};
		struct FBXNode_LongProperty :public FBXProperty {
			long long int nodeProperty = 0;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::Long;
			}
		};
		struct FBXNode_FloatProperty :public FBXProperty {
			float nodeProperty = 0.0f;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::Float;
			}
		};
		struct FBXNode_StringProperty :public FBXProperty {
			FBXNode_StringProperty(const std::string& propertyString) {
				nodeProperty = propertyString;
			}
			FBXNode_StringProperty() {}
			std::string nodeProperty = "";
			static FBXPropertyDataType GetType() {
				return FBXPropertyDataType::String;
			}
		};


		struct FBXNode_DoubleProperty :public FBXProperty {
			double nodeProperty = 0.0;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::Double;
			}
		};
		struct FBXNode_ShortProperty :public FBXProperty {
			short nodeProperty = 0;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::Short;
			}
		};
		struct FBXNode_BoolProperty :public FBXProperty {
			bool nodeProperty = 0;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::Bool;
			}
		};
		struct FBXNode_RawDataProperty :public FBXProperty {
			std::vector<char> nodeProperty;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::RawData;
			}
		};

		struct FBXNode_IntArrayProperty :public FBXProperty {
			std::vector< int> nodeProperty;
			std::vector< int> GetPolygonVertexIndex();
			std::vector< int> GetUVIndex();
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::IntArray;
			}
		};
		struct FBXNode_LongArrayProperty :public FBXProperty {
			std::vector<long long int> nodeProperty;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::LongArray;
			}
		};
		struct FBXNode_FloatArrayProperty :public FBXProperty {
			std::vector<float> nodeProperty;
			std::vector< Vector4> CreateVector4();
			std::vector< Vector3> CreateVector3();
			std::vector< Vector2> CreateVector2();
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::FloatArray;
			}
		};
		struct FBXNode_DoubleArrayProperty :public FBXProperty {
			std::vector< double> nodeProperty;
			std::vector< Vector4> CreateVector4();
			std::vector< Vector3> CreateVector3(const FBXGlobalSettings& settings, const Matrix4x4* arg_transform = nullptr);
			template <class T>
			std::vector<T> CreateVertex(const FBXGlobalSettings& settings, const Matrix4x4* arg_transform = nullptr) {
				std::vector<T> out;
				int size = nodeProperty.size() / 3;
				out.reserve(size);

				for (int i = 0; i < size; i++) {
					T vertex;
					vertex.position = Vector3((float)-1 * settings.coordAxisSign * nodeProperty.at(i * 3 + settings.coordAxis), (float)settings.upAxisSign * nodeProperty.at(i * 3 + settings.upAxis), settings.frontAxisSign * (float)nodeProperty.at(i * 3 + settings.frontAxis));
					if (arg_transform) {
						vertex.position *= *arg_transform;
					}

					out.push_back(vertex);
				}

				return out;

			}
			std::vector< Vector3> CreateVector3Half_Latter();
			std::vector< Vector2> CreateVector2();
			std::vector< Vector2> CreateVector2Half_Latter();
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::DoubleArray;
			}
		};
		struct FBXNode_ShortArrayProperty :public FBXProperty {
			std::vector< short> nodeProperty;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::Short;
			}
		};
		struct FBXNode_BoolArrayProperty :public FBXProperty {
			std::vector< bool> nodeProperty;
			static FBXPropertyDataType GetType()  {
				return FBXPropertyDataType::BoolArray;
			}
		};
		class FBXScene:public IObject
		{
		public:
			void Initialize()override {}
			void PreInitialize()override{}
			std::unordered_multimap<std::string, std::shared_ptr< FBXNodeStructure>>& GetVec_NodeRecords() {
				return multimap_nodeRecords;
			}
			void SetNodeRecords(std::unordered_multimap<std::string, std::shared_ptr< FBXNodeStructure>> arg_nodeRecords) {
				multimap_nodeRecords = arg_nodeRecords;
			}
			std::vector < std::shared_ptr< FBXNodeStructure>> SerchNode(const std::string& arg_serchNodeName);
			std::vector < std::shared_ptr< FBXNodeStructure>> SerchNode(const std::string& arg_serchNodeName,const UINT searchPropertyCount);
			std::vector < std::shared_ptr<FBXNodeStructure>> SerchNodeWithStringProperty(const std::string& arg_nodeName, const std::string& arg_stringProp, const UINT arg_propertyIndex = 0);
			

		private:
			std::unordered_multimap<std::string, std::shared_ptr< FBXNodeStructure>> multimap_nodeRecords;
		};

		namespace VertexIndexUtill {

			static inline std::vector<int> ConvertTriPolygon(std::vector<int>& arg_polygonVertex, int offset = 0) {
				std::vector<int> out;
				out.reserve(arg_polygonVertex.size() * 2);
				std::vector<int> buffer;
				buffer.resize(6);

				int nowPolygonNum = 0;
				int nowOffset = 0;
				for (auto itr = arg_polygonVertex.begin(); itr != arg_polygonVertex.end(); itr++) {
					buffer.at(nowPolygonNum) = nowOffset;
					nowPolygonNum++;
					if (*itr < 0) {
						switch (nowPolygonNum)
						{
						case 3:
							out.push_back(buffer.at(2) + offset);
							out.push_back(buffer.at(1) + offset);
							out.push_back(buffer.at(0) + offset);
							break;
						case 4:
							out.push_back(buffer.at(3) + offset);
							out.push_back(buffer.at(1) + offset);
							out.push_back(buffer.at(0) + offset);


							out.push_back(buffer.at(3) + offset);
							out.push_back(buffer.at(2) + offset);
							out.push_back(buffer.at(1) + offset);
							//std::cout << "Quad polygon is out of support." << std::endl;
							break;
						default:
							break;
						}


						nowPolygonNum = 0;
					}
					nowOffset++;
				}
				out.shrink_to_fit();
				return out;
			}

			static inline std::vector<int> ConvertTriPolygon(std::vector<int>& arg_polygonVertex,  const std::vector<int>& arg_vec_materialIndex, const UINT arg_materialCount ,  std::vector<int>& arg_materialIndexSize,int offset=0) {
				std::vector<int> out;
				std::vector<int> buffer;
				buffer.resize(6);
				int nowPolygonNum = 0;
				int nowOffset=0;

				auto matItr = arg_vec_materialIndex.begin();
				std::vector<std::vector<UINT>> materialIndices;
				materialIndices.resize(arg_materialCount);

				for (int i = 0; i < materialIndices.size(); i++) {
					materialIndices.at(i).reserve(arg_polygonVertex.size() * 3);
				}

				for (auto itr = arg_polygonVertex.begin(); itr != arg_polygonVertex.end(); itr++) {
					buffer.at(nowPolygonNum) = nowOffset;
					nowPolygonNum++;
					if (*itr < 0) {
						switch (nowPolygonNum)
						{
						case 3:
							materialIndices.at(*matItr) .push_back(buffer.at(2)+offset);
							materialIndices.at(*matItr) .push_back(buffer.at(1)+offset);
							materialIndices.at(*matItr) .push_back(buffer.at(0)+offset);
							matItr++;
							break;
						case 4:
							materialIndices.at(*matItr) .push_back(buffer.at(3)+offset);
							materialIndices.at(*matItr) .push_back(buffer.at(1)+offset);
							materialIndices.at(*matItr) .push_back(buffer.at(0)+offset);


							materialIndices.at(*matItr) .push_back(buffer.at(3)+offset);
							materialIndices.at(*matItr) .push_back(buffer.at(2)+offset);
							materialIndices.at(*matItr) .push_back(buffer.at(1)+offset);
							matItr++;
							std::cout << "Quad polygon is out of support." << std::endl;
							break;
						default:
							break;
						}


						nowPolygonNum = 0;
					}
					nowOffset++;
				}

				out.reserve(nowOffset);

				arg_materialIndexSize.reserve(materialIndices.size());

				for (int i = 0; i < materialIndices.size(); i++) {
					arg_materialIndexSize.push_back(materialIndices.at(i).size());
					std::copy(materialIndices.at(i).begin(), materialIndices.at(i).end(), std::back_inserter(out));
				}

				out.shrink_to_fit();
				return out;
			}
		}

		namespace UVUTill {

			//true=Index_to_Direct
			//false=Direct
			static inline bool GetUVReferenceMode(std::shared_ptr<FBXNodeStructure> arg_UVLayernode)
			{
				std::string refType = arg_UVLayernode->GetChildNode("ReferenceInformationType")->GetProperty<FBXNode_StringProperty>()->nodeProperty;
				if (refType == "IndexToDirect")
				{
					return true;
				}
				else
				{
					return false;
				}
			}



			//true=Polygon_Vertex
			//false=Controll_Point
			static inline bool GetUVMappingMode(std::shared_ptr<FBXNodeStructure> arg_UVLayernode) {
				std::string mapType = arg_UVLayernode->GetChildNode("MappingInformationType")->GetProperty<FBXNode_StringProperty>()->nodeProperty;
				if (mapType == "ByPolygonVertex")
				{
					return true;
				}
				else
				{
					return false;
				}
			}


		}

	}
}

template<class T>
inline std::shared_ptr<ButiEngine::FBXAnalyze::FBXNodeStructure> ButiEngine::FBXAnalyze::FBXConnectionNode::GetServeNode(std::vector<std::shared_ptr<T>>& arg_toNode)
{
	long long int key = GetProperty<FBXNode_LongProperty>()->nodeProperty;
	for (auto itr = arg_toNode.begin(); itr != arg_toNode.end(); itr++) {
		if ((*itr)->GetProperty<FBXNode_LongProperty>()->nodeProperty == key) {
			return (*itr);
		}
	}

	return nullptr;
}

template<class T>
std::shared_ptr<ButiEngine::FBXAnalyze::FBXNodeStructure> ButiEngine::FBXAnalyze::FBXConnectionNode::GetResieveNode(std::vector<std::shared_ptr<T>>& arg_byNodes)
{
	long long int key = GetProperty<FBXNode_LongProperty>(1)->nodeProperty;
	for (auto itr = arg_byNodes.begin(); itr != arg_byNodes.end(); itr++) {
		if ((*itr)->GetProperty<FBXNode_LongProperty>()->nodeProperty == key) {
			return (*itr);
		}
	}

	return nullptr;
}

template<class T, class U>
inline std::shared_ptr<ButiEngine::FBXAnalyze::FBXNodeLinkInfo> ButiEngine::FBXAnalyze::FBXConnectionNode::GetPairNode(std::vector<std::shared_ptr<T>>& arg_byNodes, std::vector<std::shared_ptr<U>>& arg_toNodes)
{
	std::shared_ptr<FBXNodeStructure> resiever = GetResieveNode(arg_byNodes);
	if (!resiever)
	{
		return nullptr;
	}
	std::shared_ptr<FBXNodeStructure> server = GetServeNode(arg_toNodes);
	if (!server) {
		return nullptr;
	}
	auto out = ObjectFactory::Create<FBXNodeLinkInfo>();
	out->serverNode = server;
	out->resieverNode = resiever;
	std::string linkType = GetProperty<FBXNode_StringProperty>(0)->nodeProperty;
	if (linkType == "OO")
	{
		out->linkType = LinkType::OO;
	}
	else if (linkType == "OP")
	{
		out->linkType = LinkType::OP;
		out->info = GetProperty<FBXNode_StringProperty>(1)->nodeProperty;
	}
	else {
		out->linkType = LinkType::PP;
	}
	
	return out;
	

}