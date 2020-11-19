#include "stdafx.h"
#include"Header/GameParts/CollisionLayer.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\ColliderComponent.h"

ButiEngine::Collision::ColliderComponent::ColliderComponent(std::shared_ptr<CollisionPrimitive> arg_shp_collisionPrim, const UINT arg_layerNum)
{
	shp_collisionPrim = arg_shp_collisionPrim;
	layerNum = arg_layerNum;
}

void ButiEngine::Collision::ColliderComponent::OnSet()
{
	if(shp_collisionPrim)
	if (!shp_collisionPrim->wkp_transform.lock()) {
		shp_collisionPrim->wkp_transform = gameObject.lock()->transform;
	}

}

void ButiEngine::Collision::ColliderComponent::OnUpdate()
{
	shp_collisionPrim->Update();
}

void ButiEngine::Collision::ColliderComponent::OnRemove()
{
	CollisionStop();
}

void ButiEngine::Collision::ColliderComponent::Start()
{
	CollisionStart();
}

void ButiEngine::Collision::ColliderComponent::CollisionStart()
{
	if (!shp_collisionPrim) {
		return;
	}

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
	if (shp_collisionPrim) {
		auto prim = shp_collisionPrim->Clone();
		return ObjectFactory::Create<ColliderComponent>(prim, layerNum);
	}
	else {
		return ObjectFactory::Create<ColliderComponent>();
	}
}

void ButiEngine::Collision::ColliderComponent::OnShowUI()
{
	if (shp_collisionPrim) {
		shp_collisionPrim->ShowUI();
	}
	else {
		if (ImGui::Button("Point")) {
			shp_collisionPrim = ObjectFactory::Create<Collision::CollisionPrimitive_Point>(gameObject.lock()->transform);
		}ImGui::SameLine();
		if (ImGui::Button("Sphere")) {
			shp_collisionPrim = ObjectFactory::Create<Collision::CollisionPrimitive_Sphere>(1,gameObject.lock()->transform);
		}ImGui::SameLine();
		if (ImGui::Button("Box_ABB")) {
			shp_collisionPrim = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(1,1,1),gameObject.lock()->transform);
		}ImGui::SameLine();
		if (ImGui::Button("Box_OBB")) {
			shp_collisionPrim = ObjectFactory::Create<Collision::CollisionPrimitive_Box_OBB>(Vector3(1, 1, 1), gameObject.lock()->transform);
		}ImGui::SameLine();
		if (ImGui::Button("Polygon")) {
			shp_collisionPrim = ObjectFactory::Create<Collision::CollisionPrimitive_Polygon>(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(0, 0, 0), gameObject.lock()->transform);
		}ImGui::SameLine();
		if (ImGui::Button("Surface")) {
			shp_collisionPrim = ObjectFactory::Create<Collision::CollisionPrimitive_Surface>(Vector3(0, 1, 0), gameObject.lock()->transform);
		}

	}

}
