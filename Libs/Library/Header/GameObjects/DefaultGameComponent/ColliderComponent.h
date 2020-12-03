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
			ColliderComponent(){}
			void Initialize()override;
			void OnSet()override;
			void OnUpdate()override;
			void OnRemove()override;
			void Start()override;
			void CollisionStart();
			void CollisionStop();
			std::string GetGameComponentName() {
				return "ColliderComponent";
			}
			std::shared_ptr<GameComponent> Clone()override;

			void OnShowUI()override;

			template<class Archive>
			void serialize(Archive& archive)
			{
				archive(shp_collisionPrim);
				archive(layerNum);
				archive(isActive);
			}
		private:
			UINT* p_index=nullptr;
			UINT layerNum=0;
			std::shared_ptr<CollisionPrimitive> shp_collisionPrim;
		};
	}
}
