#include"Header/GameComponentHeader.h"
namespace ButiEngine {



	class UIComponent :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "UIComponent";
		}
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(relativePos);
			archive(relativeScale);
		}
		void OnShowUI()override;
		void OnUpdate()override{}
		void OnSet()override;
		void Start()override;
		void SetPosision(const Vector2& arg_relativePos);
		void SetScalse(const Vector2& arg_relativeScale);
	protected:
		void SetRelativeTransform();
		Vector2 relativePos;
		Vector2 relativeScale;
	};

}

BUTI_REGIST_GAMECOMPONENT(ButiEngine::UIComponent)