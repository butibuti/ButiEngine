#pragma once
#include"stdafx.h"
#include"Header/Common/CollisionPrimitive.h"
namespace ButiEngine {
	namespace Collision {
		enum class CollisionPrimType {
			cube,sphere,capsule
		};
		class ColliderComponent:public GameComponent
		{
		public:
			ColliderComponent(std::shared_ptr<CollisionPrimitive> arg_shp_collisionPrim,const UINT arg_layerNum=0);
			void OnSet()override;
			void OnUpdate()override;
			void OnRemove()override;
			void CollisionStart();
			void CollisionStop();
			std::string GetGameComponentName() {
				return "ColliderComponent";
			}
		private:
			UINT* p_index=nullptr;
			UINT layerNum;
			std::shared_ptr<CollisionPrimitive> shp_collisionPrim;
		};
	}
}
