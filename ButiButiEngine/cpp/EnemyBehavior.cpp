#include "stdafx.h"
#include "..\include\EnemyBehavior.h"
#include"Header/GameParts/ResourceContainer.h"
#include "Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include"Header/GameObjects/DefaultGameComponent/ColliderComponent.h"

void ButiEngine::EnemyBehavior::Start()
{
}

void ButiEngine::EnemyBehavior::OnUpdate()
{
	
}

void ButiEngine::EnemyBehavior::OnCollision(std::weak_ptr<GameObject> arg_other)
{
	if (arg_other.lock()->GetGameObjectTag()==GameObjectTagManager::GetObjectTag("Bomb")) {
		ImGui::Begin("Collision!!!!");
		ImGui::End();
	}
}
