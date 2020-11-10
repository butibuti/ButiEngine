#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\Particle_3D.h"
#include"Header/GameParts/ResourceContainer.h"

void ButiEngine::Particle_3D::OnSet()
{
	if (!shp_drawComponent) {
		auto drawInfo = ObjectFactory::Create<DrawInformation>();
		drawInfo->drawSettings.topologyType = TopologyType::point;
		if (!shp_particleBuffer) {
			shp_particleBuffer = ObjectFactory::Create<CBuffer_Dx12<ParticleParameter>>(3); 
			shp_particleBuffer->SetExName("ParticleParam");
			shp_particleBuffer->Get().noise = 1;
			shp_particleBuffer->Get().power = 0.1f;

			drawInfo->vec_exCBuffer.push_back(shp_particleBuffer);
		}
		shp_drawComponent = ObjectFactory::Create<MeshDrawComponent>(gameObject.lock()->GetResourceContainer()->GetMeshTag("SphereForParticle_UV_Normal"), gameObject.lock()->GetResourceContainer()->GetShaderTag("GSParticle_Cube"), gameObject.lock()->GetResourceContainer()->GetMaterialTag("particleMaterial.bma", "Material/"), drawInfo);
		if (!IsCereal())
		gameObject.lock()->AddGameComponent_Insert(shp_drawComponent);
	}
}

void ButiEngine::Particle_3D::OnUpdate()
{

	ImGui::Begin((gameObject.lock()->GetGameObjectName()+ ":ParticleParameter").c_str());
	shp_particleBuffer->ShowUI();
	ImGui::End();
	time +=1.0f;

	shp_particleBuffer->Get().time = time;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Particle_3D::Clone()
{
	auto ret = ObjectFactory::Create<Particle_3D>();
	if (shp_particleBuffer)
	ret->shp_particleBuffer = shp_particleBuffer->Clone()->GetThis<CBuffer_Dx12<ParticleParameter>>();
	ret->SetIsCereal(true);
	return ret;
}
