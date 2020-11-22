#include "stdafx.h"
#include "ScoreUI.h"
#include"Header/GameObjects/DefaultGameComponent/MeshDrawComponent.h"
#include"Header/GameParts/ResourceContainer.h"

void ButiEngine::ScoreUI::Start()
{
	int width = 50;
	auto container = gameObject.lock()->GetResourceContainer();
	auto drawInfo =ObjectFactory::Create< DrawInformation>();
	drawInfo->drawSettings.blendMode = BlendMode::Addition;
	auto meshTag = container->GetMeshTag("Plane_UV");
	auto shaderTag = container->GetShaderTag("UVMesh");
	auto materialTag = container->GetMaterialTag("num_0");

	{

		auto transform = ObjectFactory::Create<Transform>(Vector3(2, 0, 0));
		transform->SetBaseTransform(gameObject.lock()->transform, true);

		ones = gameObject.lock()->AddGameComponent <MeshDrawComponent>(
			meshTag, shaderTag, materialTag, drawInfo, 1, transform
			);
	}
	{
		auto transform = ObjectFactory::Create<Transform>(Vector3(1, 0, 0));
		transform->SetBaseTransform(gameObject.lock()->transform, true);

		tens = gameObject.lock()->AddGameComponent<MeshDrawComponent>(
			meshTag, shaderTag, materialTag, drawInfo, 1, transform
			);
	}
	{
		auto transform = ObjectFactory::Create<Transform>(Vector3( 0, 0, 0));
		transform->SetBaseTransform(gameObject.lock()->transform, true);

		handreds = gameObject.lock()->AddGameComponent<MeshDrawComponent>(
			meshTag, shaderTag, materialTag, drawInfo, 1, transform
			);
	}
	{
		auto transform = ObjectFactory::Create<Transform>(Vector3( -1, 0, 0));
		transform->SetBaseTransform(gameObject.lock()->transform, true);

		thousands = gameObject.lock()->AddGameComponent<MeshDrawComponent>(
			meshTag, shaderTag, materialTag, drawInfo, 1, transform
			);
	}
	{
		auto transform = ObjectFactory::Create<Transform>(Vector3( -2, 0, 0));
		transform->SetBaseTransform(gameObject.lock()->transform, true);

		tenThousands = gameObject.lock()->AddGameComponent<MeshDrawComponent>(
			meshTag, shaderTag, materialTag, drawInfo, 1, transform
			);
	}

}

void ButiEngine::ScoreUI::OnUpdate()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ScoreUI::Clone()
{
	auto ret = ObjectFactory::Create<ScoreUI>();
	return ret;
}

void ButiEngine::ScoreUI::SetScore(const UINT arg_score)
{
	auto container = gameObject.lock()->GetResourceContainer();
	auto scorestr = std::to_string(arg_score);
	auto size = scorestr.size();
	std::string name = "num_" + scorestr.substr(size - 1,1);
	auto materialTag = container->GetMaterialTag(name);

	ones->SetMaterialTag(materialTag);
	ones->ReRegist();
	if (size < 2)
		return;
	name = "num_" + scorestr.substr(size - 2, 1);
	materialTag = container->GetMaterialTag(name);

	tens->SetMaterialTag(materialTag);
	tens->ReRegist();

	if (size < 3)
		return;
	name = "num_" + scorestr.substr(size - 3, 1);
	materialTag = container->GetMaterialTag(name);

	handreds->SetMaterialTag(materialTag);
	handreds->ReRegist();

	if (size < 4)
		return;
	name = "num_" + scorestr.substr(size - 4, 1);
	materialTag = container->GetMaterialTag(name);

	thousands->SetMaterialTag(materialTag);
	thousands->ReRegist();

	if (size < 5)
		return;
	name = "num_" + scorestr.substr(size - 5, 1);
	materialTag = container->GetMaterialTag(name);

	tenThousands->SetMaterialTag(materialTag);
	tenThousands->ReRegist();

}

void ButiEngine::ScoreUI::OnShowUI()
{
}
