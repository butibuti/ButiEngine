#include "stdafx.h"
#include "Booster.h"
#include"Header/GameObjects/DefaultGameComponent/ImmediateParticleController.h"

void ButiEngine::Booster::Start()
{

	shp_particleController = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("ParticleController").lock()->GetGameComponent<ImmediateParticleController>();

}

void ButiEngine::Booster::OnUpdate()
{
	for (int i = 0; i < volume; i++) {
		float seed = ButiRandom::GetRandom<float>(0.0, colorRange, 5);
		float sizeseed = ButiRandom::GetRandom<float>(0.0, sizeRange, 5);
		Particle3D particle;
		particle.position = gameObject.lock()->transform->GetWorldPosition() + offset * gameObject.lock()->transform->GetLocalRotation();
		particle.accelation = 0.85;
		particle.force = Vector3(ButiRandom::GetRandom<float>(-0.5, 0.5, 5), 0, ButiRandom::GetRandom<float>(-0.5, 0, 5)) * gameObject.lock()->transform->GetLocalRotation();
		particle.velocity = Vector3(0, 0.03, 0);
		particle.size = sizeBase + sizeBase;
		particle.sizePase = sizePase;
		particle.life = 30.0f;
		particle.anglePase = 0.0025f * seed;


		particle.color = colorBase * seed;
		particle.colorPase = colorPase;
		shp_particleController->AddParticle(particle);
	}

	

}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Booster::Clone()
{
	auto ret = ObjectFactory::Create<Booster>();
	ret->colorBase = colorBase;
	ret->sizeBase = sizeBase;
	ret->volume = volume;
	ret->colorPase = colorPase;
	ret->sizePase = sizePase;
	ret->colorRange = colorRange;
	ret->offset = offset;
	ret->sizeRange = sizeRange;

	return ret;
}

void ButiEngine::Booster::OnShowUI()
{
	ImGui::BulletText("Offset");
	ImGui::DragFloat3("##offset", &offset.x, 0.01, -50, 50);

	ImGui::BulletText("volume");
	ImGui::DragInt("##Volume", &volume, 1.0f, 0, 10);

	ImGui::BulletText("BaseColor");
	ImGui::DragFloat4("##Color", &colorBase.x, 0.01, 0, 1);
	ImGui::BulletText("ColorPase");
	ImGui::DragFloat4("##ColorPase", &colorPase.x, 0.01, 0, 1);

	ImGui::BulletText("ColorRange");
	ImGui::DragFloat("##colorRange", &colorRange,0.1,0,100);

	ImGui::BulletText("BaseSize");
	ImGui::DragFloat("##Size", &sizeBase, 0.01, 0, 1);
	ImGui::BulletText("SizePase");
	ImGui::DragFloat("##SizePase", &sizePase, 0.01, 0, 1);

	ImGui::BulletText("SizeRange");
	ImGui::DragFloat("##sizeRange", &sizeRange, 0.1, 0, 100);
}
