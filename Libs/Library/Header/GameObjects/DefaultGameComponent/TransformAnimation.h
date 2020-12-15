#include"Header/GameComponentHeader.h"
namespace ButiEngine {



	class TransformAnimation :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "TransformAnimation";
		}
		void OnUpdate()override;
		std::shared_ptr<Transform> GetInitTransform();
		std::shared_ptr<Transform> GetTargetTransform();
		void SetInitTransform(std::shared_ptr<Transform> arg_shp_InitTransform);
		void SetTargetTransform(std::shared_ptr<Transform> arg_shp_targetTransform);
		void SetEaseType(const Easing::EasingType type);
		void SetSpeed(const float arg_speed);
		void SetTime(const float arg_time);
		void SetReverse(const bool isReverse);
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
		virtual void PositionSet();
		void OnSet()override;
	protected:
		int direction = 1;
		float t = 0;
		std::shared_ptr<Transform> shp_targetTransform;
		Quat initRotate;
		Quat targetRotate;
		std::shared_ptr<Transform> shp_initTransform;
		float speed = 0.002f;;
		Easing::EasingType easeType = Easing::EasingType::EaseIn;
		bool isReverse=false;
	};

}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::TransformAnimation)