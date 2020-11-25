#include "Header/GameComponentHeader.h"

namespace ButiEngine {
	class ScoreUI;
	class ClearSceneController :public GameComponent {
	public:
		void Start()override;

		void OnUpdate()override;

		std::string GetGameComponentName()override {
			return "ClearSceneController";
		}

		std::shared_ptr<GameComponent> Clone()override {
			return ObjectFactory::Create<ClearSceneController>();
		}

		void OnShowUI()override;
		void ButtonSelect();
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(changeInterval);
		}
	private:

		int changeInterval = 0;
		int selectButton = 0;
		bool isLoading = false;
		std::vector<std::shared_ptr<ScoreUI>> vec_ranks;
		std::shared_ptr<ScoreUI> shp_currentScore;
	};


	class FailedSceneController :public GameComponent {
	public:
		void Start()override;

		void OnUpdate()override;

		std::string GetGameComponentName()override {
			return "FailedSceneController";
		}

		std::shared_ptr<GameComponent> Clone()override {
			return ObjectFactory::Create<FailedSceneController>();
		}

		void OnShowUI()override;
		void ButtonSelect();
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(changeInterval);
		}
	private:

		int changeInterval = 0;
		int selectButton = 0;
		bool isLoading = false;
		std::shared_ptr<ScoreUI> shp_currentScore;
	};

}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::ClearSceneController)
BUTI_REGIST_GAMECOMPONENT(ButiEngine::FailedSceneController)