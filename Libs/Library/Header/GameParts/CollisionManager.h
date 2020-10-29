#pragma once
#include"stdafx.h"
#include"CollisionLayer.h"
namespace ButiEngine {
	namespace Collision {
		class CollisionManager:public IObject
		{
		public:
			CollisionManager(const int arg_initLayerCount);
			void Initialize()override {}
			void PreInitialize()override {}
			void Update();
			UINT* RegistCollisionObject(const int layerNum, std::shared_ptr< CollisionObject> arg_registObj);
			void UnRegistCollisionObject(const int layerNum,UINT* registNum);
		private:
			std::vector<std::shared_ptr< CollisionLayer>> vec_collisionLayer;
		};

	}
}