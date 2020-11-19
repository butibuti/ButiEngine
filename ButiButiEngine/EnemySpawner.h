#pragma once
#include "Header/GameComponentHeader.h"
#include"Header/Device/RelativeTimer.h"

namespace ButiEngine {
	struct EnemyPattern {
		std::vector<std::string> names;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(names);
		}

		void ShowGUI() {


			if (ImGui::Button("AddEnemy")) {
				names.push_back("Enemy_Standard");
			}

			int i = 0;
			for (auto itr = names.begin(); itr != names.end(); i++) {
				ImGui::BulletText(itr->c_str());
				ImGui::SameLine();

				if (ImGui::Button(("edit" + std::to_string(i)).c_str())) {
					ImGui::OpenPopup(("edit_popup"+std::to_string(i)).c_str());
				}

				if(ImGui::BeginPopup(("edit_popup" + std::to_string(i)).c_str()) ){

					ImGui::InputTextWithHint("fileName", itr->c_str(), CallBacks::objectName, 64, 64, CallBacks::ImguiCallBack);

					if (ImGui::Button("Change")) {
						*itr = CallBacks::objectName;
						CallBacks::ObjectNameReset();
					}

					if (ImGui::Button("Remove")) {
						itr = names.erase(itr);
					}
					else {
						itr++;
					}

					ImGui::EndPopup();
				}
				else {
					itr++;
				}
			}
		}
	};

	class EnemySpawner :public GameComponent
	{
	public:
		void OnSet()override;
		void OnUpdate()override;
		void OnRemove()override;
		void Start()override;
		std::string GetGameComponentName() {
			return "EnemySpawner";
		}
		std::shared_ptr<GameComponent> Clone()override;

		void OnShowUI()override;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(shp_timer);
			archive(vec_enemyPatterns);

		}
	private:
		std::shared_ptr<RelativeTimer> shp_timer;
		std::vector<EnemyPattern> vec_enemyPatterns;
	};

}


BUTI_REGIST_GAMECOMPONENT (ButiEngine::EnemySpawner);