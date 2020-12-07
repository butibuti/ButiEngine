
#include "MeshDrawComponent.h"
namespace ButiEngine {

	class Particle_3D:public GameComponent {
	public:
		Particle_3D(){}
		std::string GetGameComponentName() override {
			return "Particle_3D";
		}
		void OnSet()override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(shp_drawComponent);
			archive(shp_particleBuffer);
			archive(time);
		}
	protected:

		void OnUpdate() override;
	private:
		std::shared_ptr<MeshDrawComponent> shp_drawComponent;
		std::shared_ptr<CBuffer_Dx12<ParticleParameter>> shp_particleBuffer;
		float time=0;
	};

}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::Particle_3D)