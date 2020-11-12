#pragma once
#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultBehavior\SampleBehavior.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\ModelDrawComponent.h"
#include"Header/GameParts/ResourceContainer.h"


BUTI_REGIST_BEHAVIOR(ButiEngine::SampleBehavior);

void ButiEngine::SampleBehavior::OnUpdate()
{
	ImGui::Begin((gameObject.lock()->GetGameObjectName() + ":BaraBaraSlider").c_str());
	
	cb_barabara->ShowUI();
	ImGui::End();
}

void ButiEngine::SampleBehavior::OnSet()
{
}

void ButiEngine::SampleBehavior::Start()
{
	auto drawInfo = ObjectFactory::Create<DrawInformation>();
	cb_barabara = ObjectFactory::Create<CBuffer_Dx12<TestGSVariable>>(5);
	cb_barabara->SetExName("BaraBara");
	drawInfo->vec_exCBuffer.push_back(cb_barabara);
	gameObject.lock()->AddGameComponent<ModelDrawComponent>(
		gameObject.lock()->GetResourceContainer()->GetModelTag("hikari.b3m", "Model/aomoti式_ウルトラマンヒカリ/"), gameObject.lock()->GetResourceContainer()->GetShaderTag("PMXModel_GS"), drawInfo
		);
}

std::shared_ptr<ButiEngine::Behavior> ButiEngine::SampleBehavior::Clone()
{
	return ObjectFactory::Create<SampleBehavior>();
}

void ButiEngine::SampleBehavior::OnShowUI()
{
	ImGui::SliderFloat("Time", &t, 0, 1.0f);
}
