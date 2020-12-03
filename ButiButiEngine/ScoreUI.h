#include "Header/GameComponentHeader.h"

namespace ButiEngine {
	class MeshDrawComponent;
	class ScoreUI :public GameComponent {
	public:
		void Start()override;
		void OnUpdate()override;
		void SetColor(const Vector4& arg_color);
		virtual std::string GetGameComponentName() override {
			return "ScoreUI";
		}
		std::shared_ptr<GameComponent> Clone()override;

		void SetScore(const UINT arg_score);


		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void OnShowUI()override;



	private:
		std::shared_ptr<MeshDrawComponent> ones;
		std::shared_ptr<MeshDrawComponent> tens;
		std::shared_ptr<MeshDrawComponent> handreds;
		std::shared_ptr<MeshDrawComponent> thousands;
		std::shared_ptr<MeshDrawComponent> tenThousands;
		std::shared_ptr<CBuffer_Dx12<LightVariable>> shp_color;
	};




	class RemainUI :public GameComponent {
	public:
		void Start()override;
		void OnUpdate()override;
		void SetColor(const Vector4& arg_color);
		virtual std::string GetGameComponentName() override {
			return "RemainUI";
		}
		std::shared_ptr<GameComponent> Clone()override;

		void SetScore(const UINT arg_score);


		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void OnShowUI()override;



	private:
		std::shared_ptr<MeshDrawComponent> ones;
		std::shared_ptr<MeshDrawComponent> tens;
		std::shared_ptr<CBuffer_Dx12<LightVariable>> shp_color;
	};

}
BUTI_REGIST_GAMECOMPONENT(ButiEngine::ScoreUI);
BUTI_REGIST_GAMECOMPONENT(ButiEngine::RemainUI);