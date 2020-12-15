#include "stdafx.h"
#include "..\..\Header\GameObjects\Behavior.h"
void ButiEngine::Behavior::Update()
{
	if(isActive)
	OnUpdate();
}

void ButiEngine::Behavior::Set(std::weak_ptr<GameObject> arg_wkp_gameObj)
{
	gameObject = arg_wkp_gameObj;
	OnSet();
}

void ButiEngine::Behavior::OnSet()
{
}

void ButiEngine::Behavior::OnRemove()
{
}

void ButiEngine::Behavior::Start()
{
}

void ButiEngine::Behavior::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

void ButiEngine::Behavior::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{
}

void ButiEngine::Behavior::OnCollisionEnd(std::weak_ptr<GameObject> arg_other)
{
}

void ButiEngine::Behavior::ShowUI()
{

	GUI::Checkbox("IsActive", &isActive);

	OnShowUI();
}

std::weak_ptr< ButiEngine::ICamera>& ButiEngine::Behavior::GetCamera()
{
	auto out = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetCamera();
	return out;
}

std::weak_ptr< ButiEngine::ICamera>& ButiEngine::Behavior::GetCamera(const UINT arg_camNum)
{
	auto out = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetCamera(arg_camNum);
	return out;
}

std::weak_ptr< ButiEngine::ICamera>& ButiEngine::Behavior::GetCamera(const std::string& arg_camName)
{
	auto out = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetCamera(arg_camName);
	return out;
}

std::weak_ptr< ButiEngine::GameObjectManager> ButiEngine::Behavior::GetManager()
{
	return gameObject.lock()->GetGameObjectManager();
}

std::shared_ptr< ButiEngine::GameObjectTagManager> ButiEngine::Behavior::GetTagManager()
{
	auto app = gameObject.lock()->GetApplication().lock();
	auto manager = app->GetGameObjectTagManager();
	return manager;
}

std::weak_ptr<ButiEngine::ICollisionManager> ButiEngine::Behavior::GetCollisionManager()
{
	return gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetCollisionManager();
}
