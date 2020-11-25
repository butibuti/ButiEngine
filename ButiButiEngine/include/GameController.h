#include "Header/GameComponentHeader.h"

namespace ButiEngine {
	class ScoreUI;
	class GameController :public GameComponent {
	public:
		void Start()override;
		void OnUpdate()override;

		void OnSet()override;

		std::string GetGameComponentName()override {
			return "GameController";
		}

		std::shared_ptr<GameComponent> Clone()override {
			return ObjectFactory::Create<GameController>();
		}

		Vector3 GetStageMax() {
			return stageMax;
		}

		Vector3 GetStageMin() {
			return stageMin;
		}

		int AddScore(int arg_score);
		void DestroyBoss(int score);
		int GetNowScore(int  arg_score) {
			return nowScore;
		}

		void Failed();

		void OnShowUI()override;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(stageMax);
			archive(stageMin);
			archive(befStage);
			archive(nextStage);
			archive(nowScore);
			archive(targetScore);
			archive(shp_timer);
		}
	private:
		Vector3 stageMax;
		Vector3 stageMin;
		std::string nextStage = "";
		std::string befStage = "";
		int nowScore;
		int enemyCount;
		int targetScore;
		int combo;
		bool isSpawnBoss=false;
		std::shared_ptr<RelativeTimer> shp_timer;
		std::shared_ptr<RelativeTimer> shp_combotimer;
		std::shared_ptr<ScoreUI> shp_scoreUI;
	};
}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::GameController)