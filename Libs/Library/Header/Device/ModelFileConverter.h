#pragma once
#include"stdafx.h"
#include"../Device/FBXNodeStructure.h"
#include"../Resources/ModelAnimation.h"
#include"../Resources/Vertex.h"
namespace ButiEngine {
	class ModelFileConverter
	{
		struct Byte64 {
			BYTE data[64];
		};
		static std::shared_ptr< FBXAnalyze::FBXNodeStructure> ReadNodeRecord(BinaryReader& fbxReader, std::unordered_multimap<std::string, std::shared_ptr< ButiEngine::FBXAnalyze::FBXNodeStructure>>& arg_vetor, std::shared_ptr< FBXAnalyze::FBXNodeStructure> parent=nullptr);

		template <class T>
		static void WriteVertex(const FBXAnalyze::FBXGlobalSettings arg_globalSettings,const std::vector<T>& arg_vertices,const Matrix4x4& arg_transformMatrix ,const std::vector<int>& arg_polygonIndex, BinaryWriter& arg_writer, std::shared_ptr<FBXAnalyze::FBXNodeStructure> arg_UVLayerNode, std::shared_ptr<FBXAnalyze::FBXNodeStructure> arg_normalLayerNode)
		{


			auto vertexUVs = arg_UVLayerNode->GetChildNode("UV")->GetProperty<FBXAnalyze::FBXNode_DoubleArrayProperty>()->CreateVector2();



			bool isUVMappingPolygpn = FBXAnalyze::UVUTill::GetUVMappingMode(arg_UVLayerNode);
			bool isUVReferenceIndexDirect = FBXAnalyze::UVUTill::GetUVReferenceMode(arg_UVLayerNode);



			auto vertexNormals = arg_normalLayerNode->GetChildNode("Normals")->GetProperty<FBXAnalyze::FBXNode_DoubleArrayProperty>()->CreateVector3(arg_globalSettings);

			bool isNormalMappingPolygpn = FBXAnalyze::UVUTill::GetUVMappingMode(arg_normalLayerNode);
			bool isNormalReferenceIndexDirect = FBXAnalyze::UVUTill::GetUVReferenceMode(arg_normalLayerNode);

			std::vector<int> uvIndex;
			if (isUVReferenceIndexDirect) {
				uvIndex = arg_UVLayerNode->GetChildNode("UVIndex")->GetProperty<FBXAnalyze::FBXNode_IntArrayProperty>()->GetUVIndex();
			}


			for (int i = 0; i < arg_polygonIndex.size(); i++) {

				arg_writer.WriteVariable((char)T::GetType());

				auto polygonRef = arg_polygonIndex.at(i);
				if (polygonRef < 0) {
					polygonRef = polygonRef * -1 - 1;
				}
				T vertex = arg_vertices.at(polygonRef);

				int UVRefVertexNum = 0;
				if (isUVMappingPolygpn) {
					UVRefVertexNum = i;
				}
				else
				{
					UVRefVertexNum = polygonRef;
				}
				int UVRefIndexNum = 0;
				if (isUVReferenceIndexDirect) {

					UVRefIndexNum = uvIndex.at(UVRefVertexNum);
				}
				else
				{
					UVRefIndexNum = UVRefVertexNum;
				}
				vertex.uv = vertexUVs.at(UVRefIndexNum);

				vertex.normal = vertexNormals.at(i);
				arg_writer.WriteVariable(vertex);
			}

		}

	public :
		static void FBXtoB3M(const std::string& arg_fbxFileName, const std::string& arg_b3mFileName, const std::string& arg_fileDirectory = "");
		static void PMXtoB3M(const std::string& arg_pmxFileName, const std::string& arg_b3mFileName, const std::string& arg_fileDirectory="");
		static void VMDtoBMD(const std::string& arg_vmdFileName,const std::string& arg_bmdFileName,const std::string& arg_fileDirectory="");
		static void CreateBMD( std::map<std::wstring, std::vector< MotionKeyFrameData>>& arg_map_keyFrames,const std::string& arg_bmdFileName, const std::string& arg_fileDirectory = "");
	};

}