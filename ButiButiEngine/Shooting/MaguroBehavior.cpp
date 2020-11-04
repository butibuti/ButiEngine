#include "stdafx.h"
#include "MaguroBehavior.h"
#include"Header/GameObjects/DefaultGameComponent/ChaseComponent.h"
#include"Header/GameObjects/DefaultGameComponent/LookAtComponent.h"
#include"Header/GameObjects/DefaultGameComponent/MeshDrawComponent.h"
#include"Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#include"Header/GameParts/ResourceContainer.h"
#include"HavokBehavior.h"

void ButiEngine::MaguroBehavior::Start()
{
	auto cameraMan = GetManager().lock()->AddObject(ObjectFactory::Create<Transform>(gameObject.lock()->transform->GetWorldPosition() + Vector3(0, 30, -10)), "cameraMan");


	auto camera = GetCamera("main");
	camera.lock()->shp_transform = cameraMan.lock()->transform;
	cameraMan.lock()->AddGameComponent<ChaseComponent>(targetTrans, 0.005f);
	cameraMan.lock()->AddGameComponent<LookAtComponent>(lookTargetTrans);
}

void ButiEngine::MaguroBehavior::OnUpdate()
{
	auto headMove= GameDevice::input.GetLeftStick();
	gameObject.lock()->transform->RollLocalRotationY_Degrees(headMove.x *3);

	headSwing +=(headMove.x-headSwing )*0.1f;

	targetTrans->SetLocalRotation(gameObject.lock()->transform->GetLocalRotation());

	int i = 1;
	for (auto headItr = vec_maguroHeadBonesTransform.begin(); headItr != vec_maguroHeadBonesTransform.end(); headItr++,i*=3) {
		(*headItr)->SetLocalRotation(Vector3(0, headSwing*60/i, 0));

	}

	if (speed > 0.08f) {
		speed -= 0.02f;
	}

	if (GameDevice::input.GetPadButton(PadButtons::XBOX_B)&&speed<=0.4f) {
		speed +=0.05f;
	}



	auto vec= gameObject.lock()->transform->GetFront();

	taleSwing += speed/2 * (swingVeloc) * 10;
	if (abs(taleSwing) > 15) {
		taleSwing = 15 * swingVeloc;
		swingVeloc *= -1;
	}
	for (auto taleItr = vec_maguroTaleBonesTransform.begin(); taleItr != vec_maguroTaleBonesTransform.end(); taleItr++) {
		(*taleItr)->SetLocalRotation(Vector3( 0,taleSwing,0));

	}

}

void ButiEngine::MaguroBehavior::OnSet()
{
	targetTrans = ObjectFactory::Create<Transform>(Vector3(0, 3.0f, -2.0f), Vector3(15, 0, 0), Vector3(1, 1, 1));

	targetTrans->SetBaseTransform(gameObject.lock()->transform, true);

	lookTargetTrans = ObjectFactory::Create<Transform>(Vector3(0, 0, 5));

	lookTargetTrans->SetBaseTransform(gameObject.lock()->transform, true);



	auto maguroDraw = gameObject.lock()->GetGameComponent("ModelDraw")->GetThis<ModelDrawComponent>();

	vec_maguroHeadBonesTransform.push_back(maguroDraw->GetModelData()->SerchBoneByName(L"hipBone")->transform);
	vec_maguroHeadBonesTransform.push_back(maguroDraw->GetModelData()->SerchBoneByName(L"spine_front")->transform);
	vec_maguroHeadBonesTransform.push_back(maguroDraw->GetModelData()->SerchBoneByName(L"head")->transform);


	vec_maguroTaleBonesTransform.push_back(maguroDraw->GetModelData()->SerchBoneByName(L"spine_tale")->transform);
	vec_maguroTaleBonesTransform.push_back(maguroDraw->GetModelData()->SerchBoneByName(L"spine_tale2")->transform);
	vec_maguroTaleBonesTransform.push_back(maguroDraw->GetModelData()->SerchBoneByName(L"spine_tale_3")->transform);

	havok = GetManager().lock()->AddObject(ObjectFactory::Create<Transform>(Vector3(2.2f, 0, 1.7f)), "havok");
	havok.lock()->transform->SetBaseTransform(maguroDraw->GetModelData()->SerchBoneByName(L"hire_L")->transform, true);
	havok.lock()->AddGameComponent<MeshDrawComponent>(
		gameObject.lock()->GetResourceContainer()->GetModelTag("gun.b3m", "Model/Maguro/"), gameObject.lock()->GetResourceContainer() ->GetShaderTag("DefaultMesh")
		);
	shp_havokBehavior = havok.lock()->AddBehavior<HavokBehavior>();
}

std::shared_ptr< ButiEngine::Behavior> ButiEngine::MaguroBehavior::Clone()
{
	return ObjectFactory::Create<MaguroBehavior>();
}
