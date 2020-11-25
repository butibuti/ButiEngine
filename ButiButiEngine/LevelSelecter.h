#include "Header/GameComponentHeader.h"

namespace ButiEngine {
	class MeshDrawComponent;
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
			archive(max);
			archive(unit);
		}

		void OnShowUI()override;



	private:
		int nowSelectNum = 0;
		int max=3;
		float unit=200;
		std::shared_ptr<Transform> shp_initCameraTransform;

		std::weak_ptr<GameObject> wkp_cam; 
		std::weak_ptr<MeshDrawComponent> wkp_draw;

	};
}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::LevelSelecter);

