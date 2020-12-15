#include "stdafx.h"
#include "..\..\Header\GameObjects\GameComponent.h"


void ButiEngine::GameComponent::Update()
{
	if(isActive)
	OnUpdate();
}

void ButiEngine::GameComponent::Set(std::weak_ptr<GameObject> arg_wkp_gameObj)
{
	gameObject = arg_wkp_gameObj;
	OnSet();
}

void ButiEngine::GameComponent::OnSet()
{
}

void ButiEngine::GameComponent::OnRemove()
{
}

std::weak_ptr< ButiEngine::ICamera>& ButiEngine::GameComponent::GetCamera()
{
	auto out = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetCamera();
	return out;
}

std::weak_ptr< ButiEngine::ICamera>& ButiEngine::GameComponent::GetCamera(const UINT arg_camNum)
{
	auto out = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetCamera(arg_camNum);
	return out;
}

std::weak_ptr< ButiEngine::ICamera>& ButiEngine::GameComponent::GetCamera(const std::string& arg_camName)
{
	auto out = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetCamera(arg_camName);
	return out;
}

std::weak_ptr< ButiEngine::GameObjectManager> ButiEngine::GameComponent::GetManager()
{
	return gameObject.lock()->GetGameObjectManager();
}

std::weak_ptr<ButiEngine::ICollisionManager> ButiEngine::GameComponent::GetCollisionManager()
{
	return gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetCollisionManager();
}

std::shared_ptr<ButiEngine::GameObjectTagManager> ButiEngine::GameComponent::GetTagManager()
{
	return gameObject.lock()->GetApplication().lock()->GetGameObjectTagManager();
}
