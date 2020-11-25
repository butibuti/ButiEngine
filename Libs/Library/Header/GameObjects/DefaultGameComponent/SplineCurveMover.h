
#include"Header/GameObjects/GameComponent.h"
namespace ButiEngine {



	class SplineCurveMover :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "SplineCurveMover";
		}
		void OnUpdate()override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(speed);
			archive(t);
			archive(splineCurve);
		}
		void OnShowUI()override;
		void PositionSet();
		void SetSplineCurve(const SplineCurve& arg_curve);
		void SetSpeed(const float spped);
	private:

		SplineCurve splineCurve;
		float t = 0;
		float speed = 0.0025f;;
	};

}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::SplineCurveMover);