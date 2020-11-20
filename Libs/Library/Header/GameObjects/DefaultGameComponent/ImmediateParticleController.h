#pragma once
#include"Header/GameComponentHeader.h"
#include"Header/Resources/Vertex.h"
namespace ButiEngine {

	struct Particle2D {
		Vector3 position;
		Vector4 color=Vector4(1,1,1,1);
		float size=1.0f;
		Vector3 velocity;
		Vector3 force;
		float accelation;
		float life=60;
		Vector4 colorPase;
		float sizePase=0;
	};


	class Resource_RealTimeMesh;


	class ImmediateParticleController :public GameComponent
	{
	public:

		void OnUpdate()override;
		void OnSet()override;
		std::string GetGameComponentName()override {
			return "ImmediateParticleController";
		}
		void AddParticle(const Particle2D& arg_particle);
		std::shared_ptr<GameComponent> Clone()override;
		void OnRemove()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(meshName);
		}
	private:
		std::vector<Particle2D> vec_particleInfo;
		std::string meshName= "Particle";
		std::shared_ptr<BackupData<Vertex::Vertex_UV_Color>> shp_backUp;
		std::shared_ptr<Resource_RealTimeMesh> shp_mesh;
		int addParticleCount = 0;
	};

}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::ImmediateParticleController);