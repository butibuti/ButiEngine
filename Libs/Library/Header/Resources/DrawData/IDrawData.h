#pragma once
#include"stdafx.h"
#include"../Vertex.h"
#include"IDrawObject.h"
#include"../../Common/CArrayBuffer.h"
namespace ButiEngine {

	class IMatrixUpdater {
	public:
		IMatrixUpdater(std::shared_ptr < CBuffer<ShaderVariable>> arg_cbuffer, std::shared_ptr<Transform> arg_transform, std::weak_ptr<GraphicDevice> arg_wkp_graphicDevice, Matrix4x4& arg_mat) {
			cbuffer = arg_cbuffer;
			transform = arg_transform;
			wkp_graphicDevice = arg_wkp_graphicDevice;
			p_worldMatrix = &arg_mat;
		}
		void Release() {
			cbuffer = nullptr;
			transform = nullptr;
		}
		virtual void WorldMatrixUpdate() = 0;
	protected:
		std::shared_ptr < CBuffer<ShaderVariable>> cbuffer;
		std::shared_ptr<Transform> transform;
		Matrix4x4* p_worldMatrix;
		std::weak_ptr<GraphicDevice> wkp_graphicDevice;
	};
	class MatrixUpdater_default :public IMatrixUpdater {
	public:
		MatrixUpdater_default(std::shared_ptr < CBuffer<ShaderVariable>> arg_cbuffer, std::shared_ptr<Transform> arg_transform, std::weak_ptr<GraphicDevice> arg_wkp_graphicDevice,Matrix4x4& arg_mat) :IMatrixUpdater(arg_cbuffer, arg_transform, arg_wkp_graphicDevice,arg_mat) {

		}

		void WorldMatrixUpdate();
	};
	class MatrixUpdater_billBoard :public IMatrixUpdater {
	public:
		MatrixUpdater_billBoard(std::shared_ptr < CBuffer<ShaderVariable>> arg_cbuffer, std::shared_ptr<Transform> arg_transform, std::weak_ptr<GraphicDevice> arg_wkp_graphicDevice, Matrix4x4& arg_mat) :IMatrixUpdater(arg_cbuffer, arg_transform, arg_wkp_graphicDevice, arg_mat) {

		}

		void WorldMatrixUpdate();
	};
	class MatrixUpdater_billBoardX :public IMatrixUpdater {
	public:
		MatrixUpdater_billBoardX(std::shared_ptr < CBuffer<ShaderVariable>> arg_cbuffer, std::shared_ptr<Transform> arg_transform, std::weak_ptr<GraphicDevice> arg_wkp_graphicDevice, Matrix4x4& arg_mat) :IMatrixUpdater(arg_cbuffer, arg_transform, arg_wkp_graphicDevice, arg_mat) {

		}

		void WorldMatrixUpdate();
	};
	class MatrixUpdater_billBoardY :public IMatrixUpdater {
	public:
		MatrixUpdater_billBoardY(std::shared_ptr < CBuffer<ShaderVariable>> arg_cbuffer, std::shared_ptr<Transform> arg_transform, std::weak_ptr<GraphicDevice> arg_wkp_graphicDevice, Matrix4x4& arg_mat) :IMatrixUpdater(arg_cbuffer, arg_transform, arg_wkp_graphicDevice, arg_mat) {

		}

		void WorldMatrixUpdate();
	};
	class MatrixUpdater_billBoardZ :public IMatrixUpdater {
	public:
		MatrixUpdater_billBoardZ(std::shared_ptr < CBuffer<ShaderVariable>> arg_cbuffer, std::shared_ptr<Transform> arg_transform, std::weak_ptr<GraphicDevice> arg_wkp_graphicDevice, Matrix4x4& arg_mat) :IMatrixUpdater(arg_cbuffer, arg_transform, arg_wkp_graphicDevice, arg_mat) {

		}

		void WorldMatrixUpdate();
	};

	struct DrawInformation :public IObject {
		DrawInformation() {}
		DrawInformation(const DrawSettings arg_DrawSettings) :drawSettings(arg_DrawSettings) {}
		DrawSettings drawSettings;
		bool isAlpha = false;
		bool isDepth = true;
		void Initialize()override {}
		void PreInitialize()override {}
		std::vector<std::shared_ptr<ICBuffer>> vec_exCBuffer;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isAlpha);
			archive(isDepth);
			archive(drawSettings);
			archive(vec_exCBuffer);
		}
		bool IsContainExCBuffer(const std::string& arg_cbufferName) {
			for (auto itr = vec_exCBuffer.begin(); itr != vec_exCBuffer.end(); itr++) {
				if ((*itr)->GetExName() == arg_cbufferName) {
					return true;
				}
			}
			return false;
		}
		void RemoveExCBuffer(const std::string& arg_cbufferName) {
			for (auto itr = vec_exCBuffer.begin(); itr != vec_exCBuffer.end(); ) {
				if ((*itr)->GetExName() == arg_cbufferName) {
					vec_exCBuffer.erase(itr);
					return;
				}
				else
					itr++;
			}
		}
		std::shared_ptr<ICBuffer> GetExCBuffer(const std::string& arg_cbufferName) {
			for (auto itr = vec_exCBuffer.begin(); itr != vec_exCBuffer.end(); ) {
				if ((*itr)->GetExName() == arg_cbufferName) {
					return *itr;
				}
				else {
					itr++;
				}
			}
			return nullptr;
		}

		std::shared_ptr< DrawInformation > Clone() {
			auto output = ObjectFactory::Create<DrawInformation>();

			output->drawSettings = drawSettings;
			output->isAlpha = isAlpha;
			output->isDepth = isDepth;
			for (auto itr = vec_exCBuffer.begin(); itr != vec_exCBuffer.end(); itr++) {
				output->vec_exCBuffer.push_back((*itr)->Clone());
			}

			return output;
		}
	};
	struct DrawData {
		

		void SetBlendMode(const BlendMode& arg_BlendMode);
		void MatrixUpdate();
		Matrix4x4 transform;
		std::shared_ptr<Transform> shp_transform;

		inline float GetMaxZ(const Matrix4x4& arg_viewMatrix){
			
			auto viewPos =transform.GetPosition()  * arg_viewMatrix;

			return viewPos.z;
		}
		
		ModelTag modelTag;
		
		MeshTag meshTag;
		
		ShaderTag shaderTag;

		std::vector < MaterialTag> vec_materialTags;
		std::vector<UINT> subset;

		std::shared_ptr< DrawInformation >shp_drawInfo;

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
			if (out && out->IsThis<CBuffer<T>>()) {
				return out->GetThis<CBuffer<T>>();
			}

			return nullptr;
		}
		template <class T>
			inline std::shared_ptr<CArrayBuffer<T>> GetCArrayBuffer(const std::string& arg_bufferName) {

			auto out = GetICBuffer(arg_bufferName);
			if (out && out->IsThis<CArrayBuffer<T>>()) {
				return out->GetThis<CArrayBuffer<T>>();
			}

			return nullptr;
		}
	protected:
		std::shared_ptr< IMatrixUpdater >shp_worldMatrixUpdater;
		UINT cBufferCount = 0;
		std::shared_ptr < CBuffer<ShaderVariable>> cbuffer;
		std::shared_ptr<Collision::CollisionPrimitive>shp_primitive;
		unsigned int* collisionRegistPtr;
	};

	struct MeshDrawData:public DrawData,public IDrawObject,public IObject {
		void PreInitialize() override{}

		virtual void ChangeCullMode(const CullMode& arg_cull) = 0;
		virtual void ChangeFillMode(const bool isFill) = 0;
		virtual void ChangeSwitchFillMode() = 0;

		virtual void SetTransform(std::shared_ptr<Transform>& arg_transform) = 0;
		virtual void SetBlendMode(const BlendMode& arg_blendMode) = 0;

		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> GetMeshAABB()override;
		std::shared_ptr<Collision::CollisionPrimitive_Box_OBB> GetMeshOBB()override;

		void SetPrimitive(std::shared_ptr<Collision::CollisionPrimitive>arg_prim) override;
		void SetOctRegistPtr(unsigned int* arg_ptr) override;
		std::shared_ptr<Collision::CollisionPrimitive> GetPrimitive() override;
		unsigned int* GetOctRegistPtr() override;
	};
	struct ModelDrawData :public IBoneDrawObject
	{

		std::shared_ptr< CArrayBuffer<Matrix4x4>> cbuffer_bone;
	};
}

