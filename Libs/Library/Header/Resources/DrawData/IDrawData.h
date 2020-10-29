#pragma once
#include"stdafx.h"
#include"../Vertex.h"
#include"IDrawObject.h"
#include"Header/Common/CArrayBuffer.h"
namespace ButiEngine {
	
	struct DrawInformation :public IObject {
		DrawInformation() {}
		DrawInformation(const DrawSettings arg_DrawSettings) :drawSettings(arg_DrawSettings) {}
		DrawSettings drawSettings;
		bool isLightUp = true;
		void Initialize()override {}
		void PreInitialize()override {}
		std::vector<std::shared_ptr<ICBuffer>> vec_exCBuffer;
		bool isUseMaterial=true;
	};
	struct DrawData {
		

		void SetBlendMode(const BlendMode& arg_BlendMode);

		std::shared_ptr<Transform> transform;

		inline float GetMaxZ(const Matrix4x4& arg_viewMatrix){
			auto trans = transform->GetMatrix().Transpose();
			auto upFrontZ = max((boxEightCorner.up_right_front * trans * arg_viewMatrix).z, (boxEightCorner.up_left_front * trans * arg_viewMatrix).z);
			auto upBackZ = max((boxEightCorner.up_right_back * trans * arg_viewMatrix).z, (boxEightCorner.up_left_back * trans * arg_viewMatrix).z);
			auto downFrontZ = max((boxEightCorner.down_right_front * trans * arg_viewMatrix).z, (boxEightCorner.down_left_front * trans * arg_viewMatrix).z);
			auto downBackZ = max((boxEightCorner.down_right_back * trans * arg_viewMatrix).z, (boxEightCorner.down_left_back * trans * arg_viewMatrix).z);

			auto upZ = max(upFrontZ, upBackZ);
			auto downZ = max(downFrontZ, downBackZ);

			return max(upZ,downZ);
		}

		
		ModelTag modelTag;
		
		MeshTag meshTag;
		
		ShaderTag shaderTag;

		std::vector < MaterialTag> vec_materialTags;
		std::vector<UINT> subset;

		std::shared_ptr< DrawInformation >shp_drawInfo;

		BoxEightCorner boxEightCorner;
		std::shared_ptr<IRenderer> shp_renderer;
		virtual std::shared_ptr<ICBuffer> AddICBuffer(std::shared_ptr<ICBuffer> arg_cbuffer) {
			shp_drawInfo-> vec_exCBuffer.push_back(arg_cbuffer);
			return arg_cbuffer;
		}

		inline std::shared_ptr<ICBuffer> GetICBuffer(const std::string& arg_bufferName) {

			for (auto itr =shp_drawInfo-> vec_exCBuffer.begin(); itr !=shp_drawInfo-> vec_exCBuffer.end(); itr++) {
				if ((*itr)->GetExName() == arg_bufferName) {
					return *itr;
				}
			}

			return nullptr;
		}

		template <class T>
		inline std::shared_ptr<CBuffer<T>> GetCBuffer(const std::string& arg_bufferName) {

			auto out = GetICBuffer(arg_bufferName);
			if (out&& out->IsThis<CBuffer<T>>()) {
				return out->GetThis<CBuffer<T>>();
			}

			return nullptr;
		}
	protected:
		UINT cBufferCount = 0;
		std::shared_ptr < CBuffer<ShaderVariable>> cbuffer;
	};

	struct MeshDrawData:public DrawData,public IDrawObject,public IObject {
		void PreInitialize() override{}

		virtual void ChangeCullMode(const CullMode& arg_cull) = 0;
		virtual void ChangeFillMode(const bool isFill) = 0;
		virtual void ChangeSwitchFillMode() = 0;

		virtual void SetTransform(std::shared_ptr<Transform>& arg_transform) = 0;
		virtual void SetBlendMode(const BlendMode& arg_blendMode) = 0;

	};
	struct ModelDrawData :public IBoneDrawObject
	{

		std::shared_ptr< CArrayBuffer<Matrix4x4>> cbuffer_bone;
	};
}

