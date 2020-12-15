#include"./TransformAnimation.h"
namespace ButiEngine {
	class CubeTransformAnimation :public TransformAnimation
	{
	public:
		std::string GetGameComponentName()override {
			return "CubeTransformAnimation";
		}
		void SetXEaseType(const Easing::EasingType type);
		void SetYEaseType(const Easing::EasingType type);
		void SetZEaseType(const Easing::EasingType type);
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
			archive(xEaseType);
			archive(yEaseType);
			archive(zEaseType);
			archive(isReverse);
		}
		void OnShowUI()override;
		void PositionSet()override;
	protected:
		Easing::EasingType xEaseType = Easing::EasingType::none;
		Easing::EasingType yEaseType = Easing::EasingType::none;
		Easing::EasingType zEaseType = Easing::EasingType::none;
	};

}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::CubeTransformAnimation)