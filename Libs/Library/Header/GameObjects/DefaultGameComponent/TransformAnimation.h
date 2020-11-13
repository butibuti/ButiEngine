#include"Header/GameObjects/GameComponent.h"
namespace ButiEngine {



	class TransformAnimation :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "TransformAnimation";
		}
		void OnUpdate()override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(speed);
			archive(t);
			archive(shp_targetTransform);
			archive(shp_initTransform);
			archive(easeType);
			archive(isReverse);
		}
		void OnShowUI()override;
		void PositionSet();
		void OnSet()override;
	private:
		int direction = 1;
		float t = 0;
		std::shared_ptr<Transform> shp_targetTransform;
		std::shared_ptr<Transform> shp_initTransform;
		float speed = 0.002f;;
		Easing::EasingType easeType = Easing::EasingType::EaseIn;
		bool isReverse=false;
	};

}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::TransformAnimation);