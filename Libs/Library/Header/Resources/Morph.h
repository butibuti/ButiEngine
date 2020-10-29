#pragma once
#include"stdafx.h"
namespace ButiEngine {
	namespace Morph {

		enum class MorphType {
			Group = 0, Vertex = 1,Bone=2,UV=3, UVex01 = 4, UVex02 = 5, UVex03 = 6, UVex04 = 7,Material=8,Flip=9
		};

		class Morph :public IObject
		{
		public:
			void SetName(const std::wstring & arg_name) { morphName = arg_name; }
			void SetEngName(const std::wstring & arg_name) { morphNameEng = arg_name; }
			void Initialize(){}
			void PreInitialize(){}
		protected:
			std::wstring morphName;
			std::wstring morphNameEng;

		};

		class Vertex_Morph :public Morph {
		public:

			struct MorphVar {
				UINT vartexIndex;
				Vector3 offset;
			};

			std::vector<MorphVar> vec_vars;
		private:
		};

		class UV_Morph :public Morph {
		public:

			struct MorphVar {
				UINT vartexIndex;
				Vector2 offset;
			};

			std::vector<MorphVar> vec_vars;
		private:
		};
		class Material_Morph :public Morph {
		public:

			struct MorphVar {
				int materialIndex;
				char offsetCalculateType;
				Vector4 Diffuse;
				Vector4 Specular;
				Vector4 Ambient;
				Vector4 textureCoefficient;
				Vector4 sphereTextureCoefficient;
				Vector4 edgeColor;
				float edgeSize;
				Vector4 toonTextureCoefficient;
			};

			std::vector<MorphVar> vec_vars;
		private:
		};

		class Bone_Morph :public Morph {
		public:

			struct MorphVar {
				int boneIndex;
			};

			std::vector<MorphVar> vec_vars;
		private:
		};

		class Group_Morph :public Morph {
		public:

			struct MorphVar {
				int morphIndex;
				float morphPer;
			};

			std::vector<MorphVar> vec_vars;
		private:
		};

		class Flip_Morph :public Morph {
		public:

			struct MorphVar {
				int morphIndex;
				float morphPer;
			};

			std::vector<MorphVar> vec_vars;
		private:
		};
	}
}
