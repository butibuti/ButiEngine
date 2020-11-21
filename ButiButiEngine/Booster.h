
#include "Header/GameComponentHeader.h"

namespace ButiEngine {
	class GameController;
	class ImmediateParticleController;
	class Booster :public GameComponent {
	public:
		void Start() override;
		void OnUpdate()override;
		virtual std::string GetGameComponentName() override{
			return "Booster";
		}

		std::shared_ptr<GameComponent> Clone()override;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(colorBase);
			archive(colorPase);
			archive(colorRange);
			archive(offset);
			archive(sizeBase);
			archive(sizeRange);
			archive(sizePase);
			archive(volume);
		}

		void OnShowUI()override;

		

	private:
		Vector4 colorBase;
		Vector4 colorPase;
		float sizeBase;
		float sizePase;
		float colorRange;
		float sizeRange;
		Vector3 offset;
		int volume=1;
		std::shared_ptr<ImmediateParticleController> shp_particleController;
	};
}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::Booster);

