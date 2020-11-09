#include "Header/GameComponentHeader.h"

namespace ButiEngine {
	class GameController :public GameComponent {
	public:

		void OnUpdate()override{}

		std::string GetGameComponentName()override {
			return "GameController";
		}

		std::shared_ptr<GameComponent> Clone()override {
			return ObjectFactory::Create<GameController>();
		}

		template<class Archive>
		void serialize(Archive& archive)
		{
		}
	private:

	};
}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::GameController)