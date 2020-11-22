#include "Header/GameComponentHeader.h"

namespace ButiEngine {
	class HPBar :public GameComponent {
	public:

		virtual std::string GetGameComponentName() override {
			return "HPBar";
		}
		void OnUpdate()override;
		std::shared_ptr<GameComponent> Clone()override;

		void SetHP(const UINT arg_hp);


		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(max);
			archive(now);
			archive(width);
		}

		void OnShowUI()override;



	private:
		UINT now = 10;
		float width = 500;
		UINT max = 10;
	};
}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::HPBar);
