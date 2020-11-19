#include "Header/GameComponentHeader.h"

namespace ButiEngine {
	class SceneChanger :public GameComponent {
	public:
		void Start()override;

		void OnUpdate()override;

		std::string GetGameComponentName()override {
			return "SceneChanger";
		}

		std::shared_ptr<GameComponent> Clone()override {
			return ObjectFactory::Create<SceneChanger>();
		}

		void OnShowUI()override;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(nextScene);
			archive(removeScene);
			archive(loadScene);
			archive(changeInterval);
		}
	private:
		std::string nextScene = "";
		std::string removeScene = "";
		std::string loadScene = "";
		int changeInterval = 0;
		bool isLoading = false;
	};
}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::SceneChanger)