#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\Particle_Standard.h"

#include"Header/Resources/DrawData/IDrawData.h"
void ButiEngine::Particle_Standard::OnSet()
{
	if (!shp_drawComponent) {
		auto drawInfo = ObjectFactory::Create<DrawInformation>();
		drawInfo->drawSettings.topologyType = TopologyType::point;
		drawInfo->isDepth = false;
		drawInfo->drawSettings.blendMode = BlendMode::Addition;
		if (!shp_particleBuffer) {
			shp_particleBuffer = ObjectFactory::Create<CBuffer_Dx12<ParticleParameter>>(4);
			shp_particleBuffer->SetExName("ParticleParam");
			shp_particleBuffer->Get().noise = 1;
			shp_particleBuffer->Get().power = 0.1f;
			drawInfo->vec_exCBuffer.push_back(shp_particleBuffer);
		}
		shp_drawComponent = ObjectFactory::Create<MeshDrawComponent>(gameObject.lock()->GetResourceContainer()->GetMeshTag("SphereForParticle_UV_Normal"), gameObject.lock()->GetResourceContainer()->GetShaderTag("GSParticle_Standard"), gameObject.lock()->GetResourceContainer()->GetMaterialTag("particleMaterial.bma", "Material/"), drawInfo);
		if(!IsCereal())
		gameObject.lock()->AddGameComponent_Insert(shp_drawComponent);
	}
}

void ButiEngine::Particle_Standard::OnUpdate()
{
	GUI::Begin((gameObject.lock()->GetGameObjectName() + ":ParticleParameter").c_str());
	shp_particleBuffer->ShowUI();
	GUI::End();
	time += 1.0f;

	shp_particleBuffer->Get().time = time;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Particle_Standard::Clone()
{
	auto ret = ObjectFactory::Create<Particle_Standard>();
	if(shp_particleBuffer)
	ret->shp_particleBuffer = shp_particleBuffer->Clone()->GetThis<CBuffer_Dx12<ParticleParameter>>();
	ret->SetIsCereal(true);
	return ret;
}
