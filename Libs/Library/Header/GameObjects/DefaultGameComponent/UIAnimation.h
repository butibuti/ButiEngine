#include"./TransformAnimation.h"
namespace ButiEngine {
	class UIAnimation :public TransformAnimation
	{
	public:
		std::string GetGameComponentName()override {
			return "UIAnimation";
		}
		void SetInitTransform(std::shared_ptr<Transform> arg_shp_InitTransform)override;
		void SetTargetTransform(std::shared_ptr<Transform> arg_shp_targetTransform)override;
		std::shared_ptr<GameComponent> Clone()override; 
		void PositionSet()override;
		void OnShowUI()override;
		void OnSet()override;
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
	protected:
		Vector2 size;
	};

}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::UIAnimation)