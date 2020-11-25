#include"Header/GameObjects/GameComponent.h"
namespace ButiEngine {



	class CloudMover :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "CloucMover";
		}
		void OnUpdate()override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void OnShowUI()override;
		static float startZ;
		static float endZ;
	private:
		Vector3 velocity = Vector3(0, 0, -0.07);
	};

}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::CloudMover);