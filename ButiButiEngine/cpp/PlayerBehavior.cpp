#include "stdafx.h"
#include "..\include\PlayerBehavior.h"
#include"Header/GameObjects/DefaultGameComponent/ColliderComponent.h"
#include"Header/GameObjects/DefaultGameComponent/MeshDrawComponent.h"
#include"Header/GameParts/ResourceContainer.h"
#include "Header/GameObjects/DefaultGameComponent/SucideComponent.h"
#include"include/GameController.h"
#include"Header/GameObjects/DefaultGameComponent/ImmediateParticleController.h"

void ButiEngine::PlayerBehavior::Start()
{
	initPos = gameObject.lock()->transform->GetLocalPosition();
	initRotate = gameObject.lock()->transform->GetLocalRotation();
	gameObject.lock()->SetGameObjectTag(GameObjectTagManager::CreateGameObjectTag("Player"));
	gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->ActiveCollision(true);
	
	auto bomb = gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("bomb");
	bomb.lock()->transform->SetBaseTransform( gameObject.lock()->transform,true);
	bomb.lock()->SetGameObjectTag(GameObjectTagManager::GetObjectTag("Bomb"));
	controller = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("GameController").lock()->GetGameComponent<GameController>();

	shp_particleController= gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("ParticleController").lock()->GetGameComponent<ImmediateParticleController>();

	stagemax = controller->GetStageMax();
	stagemin = controller->GetStageMin();
}

void ButiEngine::PlayerBehavior::OnUpdate()
{
	moveForce *= 0.7f;
	if (GameDevice::input.GetLeftStick().x<0||GameDevice::input.CheckKey(Keys::A)) {
		gameObject.lock()->transform->RollLocalRotationY_Degrees(-controllPase);
	}
	else if (GameDevice::input.GetLeftStick().x > 0 || GameDevice::input.CheckKey(Keys::D)) {

		gameObject.lock()->transform->RollLocalRotationY_Degrees(controllPase);
	}
	Particle2D particle;
	particle.position = gameObject.lock()->transform->GetWorldPosition()+Vector3( ButiRandom::GetRandom<float>(-0.2,0.2,5), ButiRandom::GetRandom<float>(-0.2, 0.2, 5), 0)* gameObject.lock()->transform->GetLocalRotation();
	particle.accelation = 0.85;
	particle.force= Vector3(ButiRandom::GetRandom<float>(-0.5, 0.5, 5), 0, ButiRandom::GetRandom<float>(-0.5, 0, 5)) * gameObject.lock()->transform->GetLocalRotation();
	particle.velocity = Vector3(0, 0.01, 0);
	particle.sizePase = 0.01f;
	particle.life = 30.0f;

	particle.color = Vector4(0.8, 0.002, 0.001, 1);
	shp_particleController->AddParticle(particle);
	

	Vector3 velocity = gameObject.lock()->transform->GetFront();
	
	velocity *= speed;
	inertia *= inertiaMinorPase;
	velocity += inertia;

	gameObject.lock()->transform->Translate(velocity+moveForce);

	inertia += velocity * 0.1f;

	auto pos = gameObject.lock()->transform->GetWorldPosition();
	if (pos.x < stagemin.x) {
		pos.x = stagemin.x;
		gameObject.lock()->transform->SetWorldPosition(pos);
	}else if (pos.x > stagemax.x) {
		pos.x = stagemax.x;
		gameObject.lock()->transform->SetWorldPosition(pos);
	}

	if (pos.z > stagemax.z) {
		pos.z = stagemax.z;
		gameObject.lock()->transform->SetWorldPosition(pos);
	}else if (pos.z <stagemin.z) {
		pos.z = stagemin.z;
		gameObject.lock()->transform->SetWorldPosition(pos);
	}

}

void ButiEngine::PlayerBehavior::OnShowUI()
{
	ImGui::BulletText("Speed");
	if (ImGui::SliderFloat("##speed", &speed, 0.0f, 1.0f, "%.3f", 1.0f)) {
	}
	ImGui::BulletText("Air resistance");
	if (ImGui::SliderFloat("##inertiaMinorPase", &inertiaMinorPase, 0.0f, 1.0f, "%.3f", 1.0f)) {
	}
	ImGui::BulletText("ControllPase");
	if (ImGui::SliderFloat("##controllPase", &controllPase, 0.0f, 10.0f, "%.3f", 1.0f)) {
	}
	ImGui::BulletText("HP");
	ImGui::SliderInt("##playerHP",&hp,0,100);
}

void ButiEngine::PlayerBehavior::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{

	if (arg_other.lock()->GetGameObjectTag() != GameObjectTagManager::GetObjectTag("Enemy")) {
		return;
	}
	moveForce += ((Vector3)(gameObject.lock()->transform->GetWorldPosition() - arg_other.lock()->transform->GetWorldPosition())).GetNormalize() * 2.5f;
	moveForce.y = 0;
	hp--;
	if (hp <= 0) {
		controllPase=0;
		speed = 0;
		gameObject.lock()->AddGameComponent<SucideComponent>(30.0f);
		controller->Failed();
	}
}
