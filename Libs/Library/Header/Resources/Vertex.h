#pragma once
#include "stdafx.h"
namespace ButiEngine {

	struct BoxSurface {
		float up, down, left, right, front, back;
	};
	struct BoxEightCorner {
		BoxEightCorner(){}
		Vector3 up_left_front;
		Vector3 up_right_front;
		Vector3 up_left_back;
		Vector3 up_right_back;
		Vector3 down_left_front;
		Vector3 down_right_front;
		Vector3 down_left_back;
		Vector3 down_right_back;
	};
	class Resource_Mesh_Dx12;
	class Resource_RealTimeMesh_Dx12;
	struct BackupDataBase :public IObject {
		BoxEightCorner eightCorner=BoxEightCorner();
		BackupDataBase() {}
		virtual ~BackupDataBase() {
		}
		virtual void ResourceUpdate(std::shared_ptr<Resource_Mesh_Dx12> arg_resource, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) = 0;
		virtual void ResourceUpdate(std::shared_ptr<Resource_RealTimeMesh_Dx12> arg_resource, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) = 0;
		void Initialize()override {}
		void PreInitialize()override {}
		inline void SetBoxSurfaces(const BoxSurface& arg_boxSurface) {
			eightCorner.up_left_front = Vector3(arg_boxSurface.left, arg_boxSurface.up, arg_boxSurface.front);
			eightCorner.up_right_front = Vector3(arg_boxSurface.right, arg_boxSurface.up, arg_boxSurface.front);
			eightCorner.up_left_back = Vector3(arg_boxSurface.left, arg_boxSurface.up, arg_boxSurface.back);
			eightCorner.up_right_back = Vector3(arg_boxSurface.right, arg_boxSurface.up, arg_boxSurface.back);
			eightCorner.down_left_front = Vector3(arg_boxSurface.left, arg_boxSurface.down, arg_boxSurface.front);
			eightCorner.down_right_front = Vector3(arg_boxSurface.right, arg_boxSurface.down, arg_boxSurface.front);
			eightCorner.down_left_back = Vector3(arg_boxSurface.left, arg_boxSurface.down, arg_boxSurface.back);
			eightCorner.down_right_back = Vector3(arg_boxSurface.right, arg_boxSurface.down, arg_boxSurface.back);
		}
		inline const BoxEightCorner& GetBoxEightCorner()const {
			return eightCorner;
		}
	};
	template<typename T>
	struct BackupData : public BackupDataBase {
		void Clear() {
			vertices.clear();
			indices.clear();
		}
		std::vector<T> vertices;
		std::vector<UINT> indices;
		void ResourceUpdate(std::shared_ptr<Resource_Mesh_Dx12> arg_resource, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice)override;
		void ResourceUpdate(std::shared_ptr<Resource_RealTimeMesh_Dx12> arg_resource, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice)override;
	};
	namespace Vertex {

		enum class VertexType {
			Vertex_UV = 0,
			Vertex_Normal = 1,
			Vertex_UV_Normal = 2,
			Vertex_UV_Normal_Color = 3,
			Vertex_Model_SingleBone = 4,
			Vertex_Model_DoubleBone = 5,
			Vertex_Model_QuadBone = 6,
			Vertex_Model_SDEFBone = 7,
			Vertex_Model_Mix = 8,
		};
		namespace VertexInformation {

			struct Vertex {
				Vector3 position;
				Vertex(const Vector3& arg_position) :position(arg_position) {}
				Vertex() {}
			};
			struct UV {
				Vector2 uv;
				UV(const Vector2& arg_uv) :uv(arg_uv) {};
				UV() {}
				inline void SetUV(const Vector2& arg_uv) { uv = arg_uv; }
				inline Vector2 GetUV()const { return uv; }
			};
			struct UV_ex1 {
				Vector4 uv;
				UV_ex1(const Vector4& arg_uv) :uv(arg_uv) {};
				UV_ex1() {}
				inline void SetUV_ex1(const Vector4& arg_uv) { uv = arg_uv; }
				inline Vector4 GetUV_ex1()const { return uv; }
			};
			struct UV_ex2 {
				Vector4 uv;
				UV_ex2(const Vector4& arg_uv) :uv(arg_uv) {};
				UV_ex2() {}
				inline void SetUV_ex2(const Vector4& arg_uv) { uv = arg_uv; }
				inline Vector4 GetUV_ex2()const { return uv; }
			};
			struct UV_ex3 {
				Vector4 uv;
				UV_ex3(const Vector4& arg_uv) :uv(arg_uv) {};
				UV_ex3() {}
				inline void SetUV_ex3(const Vector4& arg_uv) { uv = arg_uv; }
				inline Vector4 GetUV_ex3()const { return uv; }
			};
			struct UV_ex4 {
				Vector4 uv;
				UV_ex4(const Vector4& arg_uv) :uv(arg_uv) {};
				UV_ex4() {}
				inline void SetUV_ex4(const Vector4& arg_uv) { uv = arg_uv; }
				inline Vector4 GetUV_ex4()const { return uv; }
			};
			struct Normal {
				Vector3 normal;
				Normal(const Vector3& arg_normal) :normal(arg_normal) {}
				Normal() {}
				inline void SetNormal(const Vector3& arg_normal) { normal = arg_normal; }
				inline Vector3 GetNormal()const { return normal; }
			};
			struct Color {
				Vector4 color = Vector4(0.5f, 0, 0, 1.0f);
				Color(const Vector4& arg_color) :color(arg_color) {}
				Color() {}
				inline void SetColor(const Vector4& arg_color) { color = arg_color; }
				inline Vector4 GetColor()const { return color; }
			};
			//////以下、テンプレート用のダミー
			struct Dummy_UV {
			protected:
				void SetUV(const Vector2& arg_uv) {  }
				Vector2 GetUV()const { return Vector2(); }
			};
			struct Dummy_Normal {
			protected:
				void SetNormal(const Vector3& arg_uv) {  }
				Vector3 GetNormal()const { return Vector3(); }
			};
			struct Dummy_Color {
			protected:
				void SetColor(const Vector4& arg_uv) {  }
				Vector4 GetColor()const { return Vector4(); }
			};
			struct SingleBone {
				short boneIndex=-1;
			};
			struct DoubleBone {
				short boneIndex_1=-1;
				short boneIndex_2=-1;
				float weight=0.0f;
			};
			struct QuadBone {
				short boneIndex_1=-1;
				short boneIndex_2=-1;
				short boneIndex_3=-1;
				short boneIndex_4=-1;
				float weight_01 = 0.0f;
				float weight_02 = 0.0f;
				float weight_03 = 0.0f;
				float weight_04 = 0.0f;
			};
			struct SDEFBone {
				short boneIndex_1=-1;
				short boneIndex_2=-1;
				float weight = 0.0f;
				Vector3 SDEF_C;
				Vector3 SDEF_R0;
				Vector3 SDEF_R1;
			};
			struct PMX {
				short boneIndex_1=-1;
				short boneIndex_2=-1;
				short boneIndex_3=-1;
				short boneIndex_4=-1;
				float weight_01 = 0.0f;
				float weight_02 = 0.0f;
				float weight_03 = 0.0f;
				float weight_04 = 0.0f;
				Vector3 SDEF_C;
				Vector3 SDEF_R0;
				Vector3 SDEF_R1;
			};
		}
		struct Vertex_UV :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Dummy_Normal, public VertexInformation::Dummy_Color
		{
			friend class VertexHelper;
			Vertex_UV(const Vector3& arg_position, const Vector2& arg_uv) :Vertex(arg_position), UV(arg_uv) {}
			Vertex_UV() {}
		};
		struct Vertex_Normal :public VertexInformation::Vertex, public VertexInformation::Normal, public VertexInformation::Dummy_UV, public VertexInformation::Dummy_Color
		{
			friend class VertexHelper;
			Vertex_Normal(const Vector3& arg_position, const Vector3& arg_normal) :Vertex(arg_position), Normal(arg_normal)
			{
			}
			Vertex_Normal() {

			}
		};
		struct Vertex_Color :public VertexInformation::Vertex, public VertexInformation::Color, public VertexInformation::Dummy_UV, public VertexInformation::Dummy_Normal
		{
			friend class VertexHelper;

			Vertex_Color(const Vector3& arg_position, const Vector4& arg_color) :Vertex(arg_position), Color(arg_color)
			{}
			Vertex_Color() {}
		};
		struct Vertex_UV_Color :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Color, public VertexInformation::Dummy_Normal
		{
			friend class VertexHelper;
		};
		struct Vertex_UV_Normal :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::Dummy_Color
		{
			friend class VertexHelper;
			static VertexType GetType() {
				return VertexType::Vertex_UV_Normal;
			}
		};
		struct Vertex_UV_Normal_UVex01 :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::Dummy_Color, public VertexInformation::UV_ex1
		{
			friend class VertexHelper;

		};
		struct Vertex_UV_Normal_UVex02 :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::Dummy_Color, public VertexInformation::UV_ex1, public VertexInformation::UV_ex2
		{
			friend class VertexHelper;

		};
		struct Vertex_UV_Normal_UVex03 :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::Dummy_Color, public VertexInformation::UV_ex1, public VertexInformation::UV_ex2, public VertexInformation::UV_ex3
		{
			friend class VertexHelper;

		};
		struct Vertex_UV_Normal_UVex04 :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::Dummy_Color, public VertexInformation::UV_ex1, public VertexInformation::UV_ex2, public VertexInformation::UV_ex3, public VertexInformation::UV_ex4
		{
			friend class VertexHelper;

		};

		namespace ModelVertex {

			struct Vertex_Model_SingleBone :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::Dummy_Color, public VertexInformation::SingleBone
			{
				friend class VertexHelper;
			};
			struct Vertex_Model_SingleBone_UVex01 :public Vertex_Model_SingleBone, public VertexInformation::UV_ex1
			{
				friend class VertexHelper;

				static VertexType GetType() {
					return VertexType::Vertex_Model_SingleBone;
				}
			};
			struct Vertex_Model_SingleBone_UVex02 :public Vertex_Model_SingleBone_UVex01, public VertexInformation::UV_ex2
			{
				friend class VertexHelper;
			};
			struct Vertex_Model_SingleBone_UVex03 :public Vertex_Model_SingleBone_UVex02, public VertexInformation::UV_ex3
			{
				friend class VertexHelper;
			};
			struct Vertex_Model_SingleBone_UVex04 :public Vertex_Model_SingleBone_UVex03, public VertexInformation::UV_ex4
			{
				friend class VertexHelper;
			};


			struct Vertex_Model_DoubleBone :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::Dummy_Color, public VertexInformation::DoubleBone
			{
				friend class VertexHelper;
				static VertexType GetType() {
					return VertexType::Vertex_Model_DoubleBone;
				}
			};

			struct Vertex_Model_DoubleBone_UVex01 :public Vertex_Model_DoubleBone, public VertexInformation::UV_ex1
			{
				friend class VertexHelper;
			};
			struct Vertex_Model_DoubleBone_UVex02 :public Vertex_Model_DoubleBone_UVex01, public VertexInformation::UV_ex2
			{
				friend class VertexHelper;
			};
			struct Vertex_Model_DoubleBone_UVex03 :public Vertex_Model_DoubleBone_UVex02, public VertexInformation::UV_ex3
			{
				friend class VertexHelper;
			};
			struct Vertex_Model_DoubleBone_UVex04 :public Vertex_Model_DoubleBone_UVex03, public VertexInformation::UV_ex4
			{
				friend class VertexHelper;
			};

			struct Vertex_Model_QuadBone :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::Dummy_Color, public VertexInformation::QuadBone
			{
				friend class VertexHelper;

				static VertexType GetType() {
					return VertexType::Vertex_Model_QuadBone;
				}
			};

			struct Vertex_Model_QuadBone_UVex01 :public Vertex_Model_QuadBone, public VertexInformation::UV_ex1
			{
				friend class VertexHelper;
			};
			struct Vertex_Model_QuadBone_UVex02 :public Vertex_Model_QuadBone_UVex01, public VertexInformation::UV_ex2
			{
				friend class VertexHelper;
			};
			struct Vertex_Model_QuadBone_UVex03 :public Vertex_Model_QuadBone_UVex02, public VertexInformation::UV_ex3
			{
				friend class VertexHelper;
			};
			struct Vertex_Model_QuadBone_UVex04 :public Vertex_Model_QuadBone_UVex03, public VertexInformation::UV_ex4
			{
				friend class VertexHelper;
			};

			struct Vertex_Model_SdefBone :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::Dummy_Color, public VertexInformation::SDEFBone
			{
				friend class VertexHelper;
			};


			struct Vertex_Model_SdefBone_UVex01 :public Vertex_Model_SdefBone, public VertexInformation::UV_ex1
			{
				friend class VertexHelper;
			};
			struct Vertex_Model_SdefBone_UVex02 :public Vertex_Model_SdefBone_UVex01, public VertexInformation::UV_ex2
			{
				friend class VertexHelper;
			};
			struct Vertex_Model_SdefBone_UVex03 :public Vertex_Model_SdefBone_UVex02, public VertexInformation::UV_ex3
			{
				friend class VertexHelper;
			};
			struct Vertex_Model_SdefBone_UVex04 :public Vertex_Model_SdefBone_UVex03, public VertexInformation::UV_ex4
			{
				friend class VertexHelper;
			};

			struct Vertex_Model_PMX :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::Dummy_Color, public VertexInformation::PMX
			{
				friend class VertexHelper;
			};

			struct Vertex_Model_PMX_UVex01 :public Vertex_Model_PMX, public VertexInformation::UV_ex1
			{
				friend class VertexHelper;
			};
			struct Vertex_Model_PMX_UVex02 :public Vertex_Model_PMX_UVex01, public VertexInformation::UV_ex2
			{
				friend class VertexHelper;
			};
			struct Vertex_Model_PMX_UVex03 :public Vertex_Model_PMX_UVex02, public VertexInformation::UV_ex3
			{
				friend class VertexHelper;
			};
			struct Vertex_Model_PMX_UVex04 :public Vertex_Model_PMX_UVex03, public VertexInformation::UV_ex4
			{
				friend class VertexHelper;
			};
		}
		struct Vertex_Normal_Color :public VertexInformation::Vertex, public VertexInformation::Normal, public VertexInformation::Color, public VertexInformation::Dummy_UV
		{
			friend class VertexHelper;
		};

		struct Vertex_UV_Normal_Color :public VertexInformation::Vertex, public VertexInformation::UV, public VertexInformation::Normal, public VertexInformation::Color
		{
			friend class VertexHelper;
			Vertex_UV_Normal_Color() {}
			Vertex_UV_Normal_Color(const Vector3& arg_position, const Vector2& arg_uv) :Vertex(arg_position), UV(arg_uv) {}
			Vertex_UV_Normal_Color(const Vector3& arg_position, const Vector2& arg_uv, const Vector3& arg_normal) :Vertex(arg_position), UV(arg_uv), Normal(arg_normal) {}

		};
		class VertexHelper {
		public:

			template<typename T, typename U>
			static void VertexConvert(const BackupData<T>& inputMeshData, BackupData<U>& outputMeshData) {
				outputMeshData.vertices.clear();
				outputMeshData.indices = inputMeshData.indices;
				bool uvCopy = false; bool normalCopy = false; bool colorCopy = false;

				if (StringHelper::Contains(Util::GetStringTypeName<T>(), "UV") && StringHelper::Contains(Util::GetStringTypeName<U>(), "UV")) {
					uvCopy = true;
				}
				if (StringHelper::Contains(Util::GetStringTypeName<T>(), "Normal") && StringHelper::Contains(Util::GetStringTypeName<U>(), "Normal")) {
					normalCopy = true;
				}
				if (StringHelper::Contains(Util::GetStringTypeName<T>(), "Color") && StringHelper::Contains(Util::GetStringTypeName<U>(), "Color")) {
					colorCopy = true;
				}


				for (int i = 0; i < inputMeshData.vertices.size(); i++) {
					U outVertex;
					outVertex.position = inputMeshData.vertices.at(i).position;
					if (uvCopy)
					{
						outVertex.SetUV(inputMeshData.vertices.at(i).GetUV());
					}
					if (normalCopy)
					{
						outVertex.SetNormal(inputMeshData.vertices.at(i).GetNormal());
					}
					if (colorCopy)
					{
						outVertex.SetColor(inputMeshData.vertices.at(i).GetColor());
					}
					outputMeshData.vertices.push_back(outVertex);
				}
				outputMeshData.eightCorner. up_left_back = inputMeshData.eightCorner.up_left_back;
				outputMeshData.eightCorner.up_left_front = inputMeshData.eightCorner.up_left_front;
				outputMeshData.eightCorner.up_right_back = inputMeshData.eightCorner.up_right_back;
				outputMeshData.eightCorner.up_right_front = inputMeshData.eightCorner.up_right_front;
				outputMeshData.eightCorner.down_left_back = inputMeshData.eightCorner.down_left_back;
				outputMeshData.eightCorner.down_left_front = inputMeshData.eightCorner.down_left_front;
				outputMeshData.eightCorner.down_right_back = inputMeshData.eightCorner.down_right_back;
				outputMeshData.eightCorner.down_right_front = inputMeshData.eightCorner.down_right_front;
			}


		private:
			VertexHelper() {}
		};


	}

}