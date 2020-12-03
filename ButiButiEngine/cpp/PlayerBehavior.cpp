#pragma once
#include "stdafx.h"
#include "..\include\PlayerBehavior.h"
#include "Header/GameObjects/DefaultGameComponent/SucideComponent.h"
#include"include/GameController.h"
#include"Header/GameObjects/DefaultGameComponent/TransformAnimation.h"
#include"Header/GameObjects/DefaultGameComponent/CameraMan.h"

void ButiEngine::PlayerBehavior::Start()
{
	initPos = gameObject.lock()->transform->GetLocalPosition();
	initRotate = gameObject.lock()->transform->GetLocalRotation();
	gameObject.lock()->SetGameObjectTag(gameObject.lock()->GetApplication().lock()->GetGameObjectTagManager()->CreateGameObjectTag("Player"));
	gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->ActiveCollision(true);
	
	auto bomb = gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("bomb");
	bomb.lock()->transform->SetBaseTransform( gameObject.lock()->transform,true);
	bomb.lock()->SetGameObjectTag(gameObject.lock()-> GetApplication().lock()->GetGameObjectTagManager()->GetObjectTag("Bomb"));
	controller = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("GameController").lock()->GetGameComponent<GameController>();

	stagemax = controller->GetStageMax();
	stagemin = controller->GetStageMin();

	damageInvTimer = ObjectFactory::Create<RelativeTimer>(60);

	shp_cameraman = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("camera").lock()->GetBehavior< CameraMan>();
}

void ButiEngine::PlayerBehavior::OnUpdate()
{
	if (damageInvTimer->Update()) {
		damageInvTimer->Stop();
	}
	moveForce *= 0.7f;
	if (GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_LEFT) || GameDevice::GetInput()->GetLeftStick().x<0||GameDevice::GetInput()->CheckKey(Keys::A)|| GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_BUTTON_LEFT)) {
		gameObject.lock()->transform->RollLocalRotationY_Degrees(-controllPase*GameDevice::WorldSpeed);
	}
	else if (GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_RIGHT) || GameDevice::GetInput()->GetLeftStick().x > 0 || GameDevice::GetInput()->CheckKey(Keys::D) || GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_BUTTON_RIGHT)) {

		gameObject.lock()->transform->RollLocalRotationY_Degrees(controllPase * GameDevice::WorldSpeed);
	}

	

	Vector3 velocity = gameObject.lock()->transform->GetFront();
	
	velocity *= speed;
	inertia *= inertiaMinorPase;
	velocity += inertia;

	gameObject.lock()->transform->Translate(velocity * GameDevice::WorldSpeed +moveForce * GameDevice::WorldSpeed);

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
	GUI::BulletText("Speed");
	if (GUI::SliderFloat("##speed", &speed, 0.0f, 1.0f, "%.3f", 1.0f)) {
	}
	GUI::BulletText("Air resistance");
	if (GUI::SliderFloat("##inertiaMinorPase", &inertiaMinorPase, 0.0f, 1.0f, "%.3f", 1.0f)) {
	}
	GUI::BulletText("ControllPase");
	if (GUI::SliderFloat("##controllPase", &controllPase, 0.0f, 10.0f, "%.3f", 1.0f)) {
	}
	GUI::BulletText("HP");
	GUI::SliderInt("##playerHP",hp,0,100);
}

void ButiEngine::PlayerBehavior::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{

	if (isInvisible|| damageInvTimer->IsOn()||arg_other.lock()->GetGameObjectTag() != gameObject.lock()->GetApplication().lock()->GetGameObjectTagManager()->GetObjectTag("Enemy")) {
		return;
	}
	shp_cameraman->ShakeVartical(0.3f);
	moveForce += ((Vector3)(gameObject.lock()->transform->GetWorldPosition() - arg_other.lock()->transform->GetWorldPosition())).GetNormalize() * 2.5f;
	moveForce.y = 0;
	hp--;
	damageInvTimer->Start();


	auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("se_Damege.wav", "Sound/");

	gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.025);

	auto warningPlane = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("Warning").lock();
	auto warningtarget = warningPlane->transform->Clone();
	auto warnAnim = warningPlane->AddGameComponent<TransformAnimation>();
	warnAnim->SetSpeed(0.09);
	warnAnim->SetEaseType(Easing::EasingType::Parabola);

	warningtarget->SetLocalScale().y = 2000;

	warnAnim->SetTargetTransform(warningtarget);

	auto hpBar = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("HPBar_"+std::to_string(hp+1));;
	if (hpBar.lock()) {

		auto anim = hpBar.lock()->AddGameComponent<TransformAnimation>();
		auto target = hpBar.lock()->transform->Clone();
		target->SetLocalScale().x = 0;
		anim->SetSpeed(0.1);
		anim->SetTargetTransform(target);
	}

	if (hp <= 0) {
		controllPase=0;
		speed = 0;
		controller->Failed();
		isInvisible = true;
		gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("Explosion_Player", ObjectFactory::Create<Transform>(gameObject.lock()->transform->GetWorldPosition()));
		gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("Explosion_Player", ObjectFactory::Create<Transform>(gameObject.lock()->transform->GetWorldPosition()+Vector3(1,1,-1)));
		gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("Explosion_Player", ObjectFactory::Create<Transform>(gameObject.lock()->transform->GetWorldPosition()+Vector3(-2,-0.5,2)));

		auto seTag = gameObject.lock()->GetResourceContainer()->GetSoundTag("se_Bomb.wav", "Sound/");

		gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSoundManager()->Play(seTag, 0.025);
	}
}
