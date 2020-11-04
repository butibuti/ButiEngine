#include "stdafx.h"
#include"Header/GameObjects/DefaultGameComponent/ColliderComponent.h"
#include"Header/GameParts/CollisionLayer.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\ColliderComponent.h"

ButiEngine::Collision::ColliderComponent::ColliderComponent(std::shared_ptr<CollisionPrimitive> arg_shp_collisionPrim, const UINT arg_layerNum)
{
	shp_collisionPrim = arg_shp_collisionPrim;
	layerNum = arg_layerNum;
}

void ButiEngine::Collision::ColliderComponent::OnSet()
{
	CollisionStart();

}

void ButiEngine::Collision::ColliderComponent::OnUpdate()
{
	shp_collisionPrim->Update();
}

void ButiEngine::Collision::ColliderComponent::OnRemove()
{
	CollisionStop();
}

void ButiEngine::Collision::ColliderComponent::CollisionStart()
{
	if (p_index) {
		return;
	}
	auto registObj = ObjectFactory::Create< CollisionObject>(shp_collisionPrim, gameObject.lock());

	p_index = GetCollisionManager().lock()->RegistCollisionObject(layerNum, registObj);

}

void ButiEngine::Collision::ColliderComponent::CollisionStop()
{
	if (!p_index) {
		return;
	}
	GetCollisionManager().lock()->UnRegistCollisionObject(layerNum, p_index);
	p_index = nullptr;
}

std::shared_ptr< ButiEngine::GameComponent> ButiEngine::Collision::ColliderComponent::Clone()
{
	return ObjectFactory::Create<ColliderComponent>(shp_collisionPrim,layerNum);
}
