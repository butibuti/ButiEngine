#include "stdafx.h"
#include "..\include\EnemyBehavior.h"
#include"Header/GameParts/ResourceContainer.h"
#include "Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"

void ButiEngine::EnemyBehavior::Start()
{
	auto drawInfo = ObjectFactory::Create<DrawInformation>();
	cb_barabara = ObjectFactory::Create<CBuffer_Dx12<TestGSVariable>>(4);
	cb_barabara->SetExName("BaraBara");
	drawInfo->vec_exCBuffer.push_back(cb_barabara);
	gameObject.lock()->AddGameComponent<ModelDrawComponent>(
		gameObject.lock()->GetResourceContainer()->GetModelTag("hikari.b3m", "Model/aomoti式_ウルトラマンヒカリ/"), gameObject.lock()->GetResourceContainer()->GetShaderTag("PMXModel_GS"), drawInfo
		);
}

void ButiEngine::EnemyBehavior::OnUpdate()
{

	ImGui::Begin((gameObject.lock()->GetGameObjectName() + ":BaraBaraSlider").c_str());

	cb_barabara->ShowUI();
	ImGui::End();
}
