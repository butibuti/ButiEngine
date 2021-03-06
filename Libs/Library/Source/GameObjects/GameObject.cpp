#include"stdafx.h"
#include "..\..\Header\Common\CerealUtill.h"

ButiEngine::GameObject::GameObject()
{
}

ButiEngine::GameObject::GameObject(std::shared_ptr<Transform> arg_transform, const std::string& arg_objectName, const std::string& arg_tagName )
{
	transform = arg_transform;
	objectName = arg_objectName;
}

void ButiEngine::GameObject::Update()
{
	if (!isActive) {
		return;
	}
	BehaviorHit();
	GameComponentUpdate();
	OnUpdate();
}

void ButiEngine::GameObject::Start()
{
	for (auto itr = vec_gameComponents.begin(); itr != vec_gameComponents.end(); itr++) {
		(*itr)->Start();
	}
}

void ButiEngine::GameObject::SetActive(bool arg_isActive)
{
	isActive = arg_isActive;
	for (auto itr = vec_childGameObjects.begin(); itr != vec_childGameObjects.end(); itr++) {
		itr->lock()->SetActive(arg_isActive);
	}
}

void ButiEngine::GameObject::SetGameObjectManager(std::weak_ptr<GameObjectManager> arg_wkp_gameObjectManager)
{
	
	wkp_gameObjManager = arg_wkp_gameObjectManager;
	

}

bool ButiEngine::GameObject::GetActive()
{
	return isActive;
}

void ButiEngine::GameObject::SetIsRemove(bool arg_isRemove)
{
	isRemove = arg_isRemove;
	for (auto itr = vec_childGameObjects.begin(); itr != vec_childGameObjects.end(); itr++) {
		itr->lock()->SetIsRemove(arg_isRemove);
	}
}

bool ButiEngine::GameObject::GetIsRemove()
{
	return isRemove;
}

void ButiEngine::GameObject::OnUpdate()
{
}

void ButiEngine::GameObject::Hit(std::shared_ptr<GameObject> shp_other)
{
	vec_collisionObject.push_back(shp_other);
}


void ButiEngine::GameObject::Release()
{
	auto endItr = vec_gameComponents.end();
	for (auto itr = vec_gameComponents.begin(); itr != endItr; itr++) {
		(*itr)->OnRemove();
	}
	vec_gameComponents.clear();
	vec_befCollisionObject.clear();
	vec_collisionObject.clear();
	vec_childGameObjects.clear();
	wkp_gameObjManager.lock()->UnRegistGameObject(GetThis<GameObject>());
}

void ButiEngine::GameObject::Initialize()
{

}

void ButiEngine::GameObject::PreInitialize()
{
}

void ButiEngine::GameObject::RegistReactionComponent(std::shared_ptr<GameComponent> arg_shp_gameComponent)
{
	vec_collisionReactionComponents.push_back(arg_shp_gameComponent);
}


std::shared_ptr<ButiEngine::GameComponent> ButiEngine::GameObject::AddGameComponent(std::shared_ptr<GameComponent> arg_shp_gameComponent)
{
	vec_newGameComponent.push_back(arg_shp_gameComponent);
	arg_shp_gameComponent->Set(GetThis<GameObject>());
	return arg_shp_gameComponent;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::GameObject::AddGameComponent_Insert(std::shared_ptr<GameComponent> arg_shp_gameComponent)
{
	arg_shp_gameComponent->Set(GetThis<GameObject>());
	RegisterGameComponent(arg_shp_gameComponent);
	return arg_shp_gameComponent;
}


std::shared_ptr<ButiEngine::GameComponent> ButiEngine::GameObject::GetGameComponent(const std::string& arg_gameComponentName,unsigned int index)
{
	std::shared_ptr<ButiEngine::GameComponent> output = nullptr;
	for (auto itr = vec_gameComponents.begin(); itr != vec_gameComponents.end(); itr++) {
		if ((*itr)->GetGameComponentName() == arg_gameComponentName) {
			if(index==0)
			return *itr;
			index--;
			output = *itr;
		}
	}
	for (auto itr = vec_newGameComponent.begin(); itr != vec_newGameComponent.end(); itr++) {
		if ((*itr)->GetGameComponentName() == arg_gameComponentName) {
			if (index == 0)
			return *itr;
			index--;
			output = *itr;
		}
	}

	return output;
}


void ButiEngine::GameObject::RemoveGameComponent(const std::string& arg_key)
{
	for (auto itr = vec_gameComponents.begin(); itr != vec_gameComponents.end(); itr++) {
		if ((*itr)->GetGameComponentName() == arg_key) {
			(*itr)->OnRemove(); 
			(*itr)->SetIsRemove(true);
			break;
		}
	}
}

void ButiEngine::GameObject::ShowUI()
{
	if (GUI::TreeNode("Tag")) {
		static std::string tagName="";
		GUI::Text("Now::" + GetApplication().lock()->GetGameObjectTagManager()->GetTagName(gameObjectTag));
		GUI::InputTextWithHint("##tagname", tagName, GUI::tagName, sizeof(GUI::tagName));
		if (GUI::Button("Set Tag!!")) {
			tagName = GUI::tagName;
			GUI::TagNameReset();
			gameObjectTag = GetApplication().lock()->GetGameObjectTagManager()->GetObjectTag(tagName);
		}
		GUI::TreePop();
	}
	



	if (GUI::TreeNode("Transform")) {
		transform->ShowUI();

		std::string target = "BaseTransform:";
		if (transform->GetBaseTransform()) {
			target += "Existence";
		}
		else {
			target += "nullptr";
		}


		GUI::BeginChild("##BaseTransform", Vector2((GUI::GetFontSize()) * (target.size() + 2), GUI::GetFontSize() * 2), true);
		GUI::BulletText((target).c_str());
		if (transform->GetBaseTransform()) {
			GUI::SameLine();
			if (GUI::Button("Detach")) {
				transform->SetBaseTransform( nullptr);
			}
		}
		GUI::SameLine();
		if (GUI::Button("Attach New")) {
			if (!transform->GetBaseTransform())
				transform->SetBaseTransform(ObjectFactory::Create<Transform>(),true);
			else {
				transform->SetBaseTransform(transform->GetBaseTransform()->Clone(), true);
			}
		}

		if (GUI::IsWindowHovered())
		{
			auto obj = GetApplication().lock()->GetGUIController()->GetDraggingObject();


			if (obj && obj->IsThis<GameObject>()) {

				auto trans = obj->GetThis<GameObject>()->transform;
				transform->SetBaseTransform(trans,true);
			}

		}
		GUI::EndChild();

		GUI::TreePop();
	}

	if (GUI::TreeNode("GameComponent")) {

		auto endItr = vec_gameComponents.end();
		int componentCount = 0;
		for (auto itr = vec_gameComponents.begin(); itr != endItr;componentCount++) {
			bool isComponentRemove = false;
			if (GUI::TreeNode(((*itr)->GetGameComponentName()+"##"+std::to_string(componentCount)).c_str())) {
				if (GUI::Button("Remove")) {
					isComponentRemove = true;
				}

				(*itr)->ShowUI();

				GUI::TreePop();
			}

			if (isComponentRemove) {
				(*itr)->OnRemove();
				itr = vec_gameComponents.erase(itr);
				endItr = vec_gameComponents.end();
			}
			else {
				itr++;
			}

		}

		GUI::TreePop();
	}
}

std::string ButiEngine::GameObject::SetObjectName(const std::string& arg_objectName)
{
	if (wkp_gameObjManager.lock()) {
		objectName = wkp_gameObjManager.lock()->ReNameGameObject(arg_objectName, objectName);

		return objectName;
	}
	else {
		objectName = arg_objectName;
		return arg_objectName;
	}
}
std::weak_ptr< ButiEngine::GameObjectManager> ButiEngine::GameObject::GetGameObjectManager()
{
	return wkp_gameObjManager;
}

std::weak_ptr<ButiEngine::IApplication> ButiEngine::GameObject::GetApplication()
{
	return  wkp_gameObjManager.lock()->GetApplication();
}

std::shared_ptr<ButiEngine::IResourceContainer> ButiEngine::GameObject::GetResourceContainer()
{
	return wkp_gameObjManager.lock()->GetScene().lock()->GetSceneManager().lock()->GetApplication().lock()->GetResourceContainer();
}

std::shared_ptr<ButiEngine::GraphicDevice> ButiEngine::GameObject::GetGraphicDevice()
{
	return wkp_gameObjManager.lock()->GetScene().lock()->GetSceneManager().lock()->GetApplication().lock()->GetGraphicDevice();
}


std::shared_ptr<ButiEngine::GameObject> ButiEngine::GameObject::Clone()
{
	auto output= ObjectFactory::Create<GameObject>(transform->Clone(), GetGameObjectName());
	output->SetGameObjectTag(gameObjectTag);
	output->gameObjectTag = gameObjectTag;
	auto componentsEndItr = vec_gameComponents.end();
	for (auto itr = vec_gameComponents.begin(); itr != componentsEndItr;itr++) {
		auto cloneComponent = (*itr)->Clone();
		if(cloneComponent)
		output->vec_gameComponents.push_back(cloneComponent);
	}

	return output;
}

void ButiEngine::GameObject::Init_RegistGameComponents()
{
	auto endItr = vec_gameComponents.end();
	for (auto itr = vec_gameComponents.begin(); itr != endItr; itr++) {
		(*itr)->Set(GetThis<GameObject>());
	}
}


std::shared_ptr<ButiEngine::GameComponent> ButiEngine::GameObject::RegisterGameComponent(std::shared_ptr<GameComponent> arg_shp_gameComponent)
{
	
	vec_gameComponents.push_back(arg_shp_gameComponent);
	return arg_shp_gameComponent;
}


void ButiEngine::GameObject::Translate(const Vector3& arg_velocity)
{
	transform->SetLocalPosition() += arg_velocity;
}

void ButiEngine::GameObject::ScaleChange(const Vector3& changePase)
{
	transform->SetLocalScale() += changePase;
}

void ButiEngine::GameObject::Rotation(const Vector3& rotationPase)
{
	transform->RollLocalRotation(rotationPase);
}


void ButiEngine::GameObject::GameComponentUpdate()
{
	for (auto itr = vec_newGameComponent.begin(); itr != vec_newGameComponent.end(); itr++) {
		RegisterGameComponent(*itr);
	}
	vec_newGameComponent.clear();
	auto itr = vec_gameComponents.begin();
	while (itr!=vec_gameComponents.end())
	{
		if ((*itr)->IsRemove()) {
			itr= vec_gameComponents.erase(itr);
		}
		else {
			(*itr)->Update();
			itr++;
		}
	}
}


void ButiEngine::GameObject::BehaviorHit()
{
	auto collisionEndItr = vec_collisionObject.end();
	auto reactionEnd = vec_collisionReactionComponents.end();
	for (auto itr = vec_collisionObject.begin(); itr != collisionEndItr; itr++) {


		bool isBef=false;
		for (auto befItr = vec_befCollisionObject.begin(); befItr != vec_befCollisionObject.end(); befItr++) {
			if ((*befItr)== (*itr)) {
				isBef = true;
				vec_befCollisionObject.erase(befItr);
				break;
			}
		}
		for (auto behItr = vec_collisionReactionComponents.begin(); behItr != reactionEnd; behItr++) {
			if (isBef) {
				(*behItr)->OnCollision(*itr);
			}
			else {
				(*behItr)->OnCollisionEnter(*itr);
			}
		}
	}
	auto befEnd = vec_befCollisionObject.end();
	for (auto befItr = vec_befCollisionObject.begin(); befItr != befEnd; befItr++) {
		for (auto behItr = vec_collisionReactionComponents.begin(); behItr != reactionEnd; behItr++) {
			(*behItr)->OnCollisionEnd(*befItr);
		}
	}

	vec_befCollisionObject = vec_collisionObject;
	vec_collisionObject.clear();
	vec_collisionObject.reserve(vec_befCollisionObject.size());
}


void ButiEngine::OutputCereal(const std::shared_ptr<GameObject>& v)
{
	std::stringstream stream;


	cereal::BinaryOutputArchive binOutArchive(stream);
	binOutArchive(v);

	std::ofstream outputFile(GlobalSettings::GetResourceDirectory()+"GameObject/"+ v->GetGameObjectName()+".gameObject", std::ios::binary);

	outputFile << stream.str();

	outputFile.close();
	stream.clear();
}

void ButiEngine::InputCereal(std::shared_ptr<GameObject>& v, const std::string& path)
{
	std::stringstream stream;

	std::ifstream inputFile( path, std::ios::binary);

	stream << inputFile.rdbuf();

	cereal::BinaryInputArchive binInputArchive(stream);


	binInputArchive(v);
}
