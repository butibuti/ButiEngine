#pragma once
#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultBehavior\SampleBehavior.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\ModelDrawComponent.h"
#include"Header/GameParts/ResourceContainer.h"
#include"Header/Resources/Resource_RealTimeMesh.h"
#include "Header/GameObjects/DefaultGameComponent/TransformAnimation.h"

void ButiEngine::SampleBehavior::OnUpdate()
{
	if (GameDevice::input.CheckKey(Keys::W)) {
		gameObject.lock()->transform->TranslateZ(0.1f);
	}
	if (GameDevice::input.CheckKey(Keys::S)) {
		gameObject.lock()->transform->TranslateZ(-0.1f);
	}
	if (GameDevice::input.CheckKey(Keys::D)) {
		gameObject.lock()->transform->TranslateX(0.1f);
	}
	if (GameDevice::input.CheckKey(Keys::A)) {
		shp_mesh->GetBackupData_Row()->GetThis<BackupData<Vertex::Vertex_UV_Normal>>()->vertices.at(0).position.y += 0.1;
	}
	if (GameDevice::input.CheckKey(Keys::LeftShift)) {
		gameObject.lock()->transform->TranslateY(-0.1f);
	}
	if (GameDevice::input.CheckKey(Keys::Space)) {
		gameObject.lock()->transform->TranslateY(0.1f);
	}

	shp_mesh->Update();

}

void ButiEngine::SampleBehavior::OnSet()
{
}

void ButiEngine::SampleBehavior::Start()
{
	//splineCurve = SplineCurve(Vector3(0,6,-10), { Vector3(-10,5 , -1),Vector3(-5, 2,10),Vector3(5, 3, 5)});
	
	auto meshTag = gameObject.lock()->GetResourceContainer()->GetMeshTag("Sphere_UV_Normal");
	shp_mesh = gameObject.lock()->GetResourceContainer()->GetMesh(meshTag).lock()->GetThis<Resource_RealTimeMesh>();
}

std::shared_ptr<ButiEngine::Behavior> ButiEngine::SampleBehavior::Clone()
{
	return ObjectFactory::Create<SampleBehavior>();
}

void ButiEngine::SampleBehavior::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{
	ImGui::Begin("##Collision");
	ImGui::Text("Enter");
	ImGui::End();
}

void ButiEngine::SampleBehavior::OnCollision(std::weak_ptr<GameObject> arg_other)
{
	ImGui::Begin("##Collision");

	ImGui::Text("OnCollison");
	ImGui::End();
}

void ButiEngine::SampleBehavior::OnCollisionEnd(std::weak_ptr<GameObject> arg_other)
{
	ImGui::Begin("##Collision");

	ImGui::Text("End");
	ImGui::End();
}

void ButiEngine::SampleBehavior::OnShowUI()
{
}

void ButiEngine::SampleBehavior::PositionSet()
{

}

