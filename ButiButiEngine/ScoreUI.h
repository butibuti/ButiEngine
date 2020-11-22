#include "Header/GameComponentHeader.h"

namespace ButiEngine {
	class MeshDrawComponent;
	class ScoreUI :public GameComponent {
	public:
		void Start()override;
		void OnUpdate()override;
		virtual std::string GetGameComponentName() override {
			return "ScoreUI";
		}
		std::shared_ptr<GameComponent> Clone()override;

		void SetScore(const UINT arg_score);


		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void OnShowUI()override;



	private:
		std::shared_ptr<MeshDrawComponent> ones;
		std::shared_ptr<MeshDrawComponent> tens;
		std::shared_ptr<MeshDrawComponent> handreds;
		std::shared_ptr<MeshDrawComponent> thousands;
		std::shared_ptr<MeshDrawComponent> tenThousands;
	};
}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::ScoreUI);