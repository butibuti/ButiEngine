#include"stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\Particle_3D.h"
#include "Header/GameObjects/DefaultGameComponent/MeshDrawComponent.h"

#include "..\..\Header\Common\CerealUtill.h"
BUTI_REGIST_GAMECOMPONENT(ButiEngine::Particle_3D)
void ButiEngine::Particle_3D::OnSet()
{
	if (IsCereal()) {
		auto drawcomp = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
		if (drawcomp) {

			shp_particleBuffer = drawcomp->GetDrawInformation()->GetExCBuffer("ParticleParam")->GetThis<CBuffer_Dx12<ParticleParameter>>();
			return;
		}
	}
	auto drawcomp = gameObject.lock()->GetGameComponent("MeshDraw");
	if (drawcomp) {
		shp_drawComponent = drawcomp->GetThis<MeshDrawComponent>();
		
		shp_drawComponent->GetDrawInformation()->vec_exCBuffer.push_back(ObjectFactory::Create<CBuffer_Dx12<ParticleParameter>>(4));
	}
	if (!shp_drawComponent) {
		auto drawInfo = ObjectFactory::Create<DrawInformation>();
		drawInfo->drawSettings.topologyType = TopologyType::point;
		if (!shp_particleBuffer) {
			shp_particleBuffer = ObjectFactory::Create<CBuffer_Dx12<ParticleParameter>>(4); 
			shp_particleBuffer->SetExName("ParticleParam");
			shp_particleBuffer->Get().noise = 1;
			shp_particleBuffer->Get().power = 0.1f;

			drawInfo->vec_exCBuffer.push_back(shp_particleBuffer);
		}
		shp_drawComponent = ObjectFactory::Create<MeshDrawComponent>(gameObject.lock()->GetResourceContainer()->GetMeshTag("SphereForParticle_UV_Normal"), gameObject.lock()->GetResourceContainer()->GetShaderTag("GSParticle_Cube"), gameObject.lock()->GetResourceContainer()->GetMaterialTag("particleMaterial.bma", "Material/"), drawInfo);
		
		gameObject.lock()->AddGameComponent_Insert(shp_drawComponent);
	}
}

void ButiEngine::Particle_3D::OnUpdate()
{

	time +=1.0f;

	shp_particleBuffer->Get().time = time;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Particle_3D::Clone()
{
	auto ret = ObjectFactory::Create<Particle_3D>();
	ret->SetIsCereal(IsCereal());
	return ret;
}
