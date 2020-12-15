#pragma once

#include"stdafx.h"

#include"..\..\Header\GameParts\CollisionLayer.h"
namespace ButiEngine {
	namespace Collision {
		struct CollisionLayerInfo {

			template<class Archive>
			void serialize(Archive& archive)
			{
				archive(size);
				archive(offset);
				archive(level);
			}
			Vector3 size;
			Vector3 offset;
			int level;
		};
		class CollisionManager :public ICollisionManager
		{
		public:
			CollisionManager(const int arg_initLayerCount);
			CollisionManager(){}
			void Initialize()override {}
			void PreInitialize()override {}
			void Update()override;
			UINT* RegistCollisionObject(const int layerNum, std::shared_ptr< Collision::CollisionPrimitive>arg_prim,std::shared_ptr<GameObject> arg_registObj)override;
			void UnRegistCollisionObject(const int layerNum, UINT* registNum)override;
			UINT GetLayerCount()override;
			void AddLayer(const Vector3& size,const int level)override;
			void RemoveLayer(const int arg_layer)override;
			void ReCreateLayers();
			bool IsWillHit(std::shared_ptr< Collision::CollisionPrimitive>arg_prim, int arg_layer)override;
			void ShowGUI()override;
			template<class Archive>
			void serialize(Archive& archive)
			{
				archive(vec_collisionLayerInfo);
			}
		private:
			std::vector<std::shared_ptr< CollisionLayer<GameObject>>> vec_collisionLayer;
			std::vector<CollisionLayerInfo> vec_collisionLayerInfo;
		};

	}


	void OutputCereal(const std::shared_ptr<Collision:: CollisionManager>& v,  const std::string& path);

	void InputCereal(std::shared_ptr<Collision::CollisionManager>& v, const std::string& path);
}