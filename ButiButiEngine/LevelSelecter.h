#include "Header/GameComponentHeader.h"

namespace ButiEngine {
	class LevelSelecter :public GameComponent {
	public:
		void Start() override;
		void OnUpdate()override;
		virtual std::string GetGameComponentName() override {
			return "LevelSelecter";
		}

		std::shared_ptr<GameComponent> Clone()override;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void OnShowUI()override;



	private:
		UINT nowSelectNum = 0;
		UINT max=3;
	};
}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::LevelSelecter);

