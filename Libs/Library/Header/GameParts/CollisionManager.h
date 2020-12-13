#pragma once

#include"stdafx.h"

#include"..\..\Header\GameParts\CollisionLayer.h"
namespace ButiEngine {
	namespace Collision {
		class CollisionManager :public IObject
		{
		public:
			CollisionManager(const int arg_initLayerCount);
			void Initialize()override {}
			void PreInitialize()override {}
			void Update();
			UINT* RegistCollisionObject(const int layerNum, std::shared_ptr< Collision::CollisionPrimitive>arg_prim,std::shared_ptr<GameObject> arg_registObj);
			void UnRegistCollisionObject(const int layerNum, UINT* registNum);
			UINT GetLayerCount();
		private:
			std::vector<std::shared_ptr< CollisionLayer<GameObject>>> vec_collisionLayer;
		};

	}
}