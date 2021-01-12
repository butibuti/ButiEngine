#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\UIComponent.h"


std::shared_ptr<ButiEngine::GameComponent> ButiEngine::UIComponent::Clone()
{
	return  ObjectFactory::Create<UIComponent>();
}

void ButiEngine::UIComponent::OnShowUI()
{
	if (gameObject.lock()->transform->ShowUI()) {
		SetRelativeTransform();
	}
}

void ButiEngine::UIComponent::OnSet()
{

	auto winSize = gameObject.lock()->GetApplication().lock()->GetWindow()->GetSize();


	gameObject.lock()->transform->SetLocalPosition().x = relativePos.x * winSize.x;
	gameObject.lock()->transform->SetLocalPosition().y = relativePos.y * winSize.y;

	gameObject.lock()->transform->SetLocalScale().x = relativeScale.x * winSize.x;
	gameObject.lock()->transform->SetLocalScale().y = relativeScale.y * winSize.y;

}

void ButiEngine::UIComponent::Start()
{
	SetRelativeTransform();
	
}

void ButiEngine::UIComponent::SetPosision(const Vector2& arg_relativePos)
{
	auto winSize = gameObject.lock()->GetApplication().lock()->GetWindow()->GetSize();
	gameObject.lock()->transform->SetLocalPosition().x = arg_relativePos.x * winSize.x;
	gameObject.lock()->transform->SetLocalPosition().y = arg_relativePos.y * winSize.y;



	SetRelativeTransform();
}

void ButiEngine::UIComponent::SetScalse(const Vector2& arg_relativeScale)
{
	auto winSize = gameObject.lock()->GetApplication().lock()->GetWindow()->GetSize();
	gameObject.lock()->transform->SetLocalScale().x = arg_relativeScale.x * winSize.x;
	gameObject.lock()->transform->SetLocalScale().y = arg_relativeScale.y * winSize.y;
	SetRelativeTransform();
}

void ButiEngine::UIComponent::SetRelativeTransform()
{

	auto winSize = gameObject.lock()->GetApplication().lock()->GetWindow()->GetSize();
	relativePos = gameObject.lock()->transform->GetLocalPosition().ToVector2();
	relativePos.x /= winSize.x;
	relativePos.y /= winSize.y;
	relativeScale = gameObject.lock()->transform->GetLocalScale().ToVector2();
	relativeScale.x /= winSize.x;
	relativeScale.y /= winSize.y;
}
