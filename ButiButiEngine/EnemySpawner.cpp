#include "stdafx.h"
#include "EnemySpawner.h"

void ButiEngine::EnemySpawner::OnSet()
{
    if(!shp_timer)
    shp_timer = ObjectFactory::Create<RelativeTimer>(60);
}

void ButiEngine::EnemySpawner::OnUpdate()
{
    if (shp_timer->Update()) {

        auto i = ButiRandom::GetRandom<int>(0, vec_enemyPatterns.size()-1,1);

        for (auto itr = vec_enemyPatterns[i].names.begin(); itr != vec_enemyPatterns[i].names.end(); itr++) {
            gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal(*itr);
        }
    }
}

void ButiEngine::EnemySpawner::OnRemove()
{
}

void ButiEngine::EnemySpawner::Start()
{

}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::EnemySpawner::Clone()
{
    auto output = ObjectFactory::Create<EnemySpawner>();

    if (shp_timer)
        output->shp_timer = shp_timer;

    output->vec_enemyPatterns = vec_enemyPatterns;

    return output;
}

void ButiEngine::EnemySpawner::OnShowUI()
{


    if (ImGui::Button("AddEnemyPattern")) {
        vec_enemyPatterns.push_back(EnemyPattern());
    }

    for (int i = 0; i < vec_enemyPatterns.size();i++) {
        if (ImGui::TreeNode(("EnemyPattern"+std::to_string(i)).c_str())) {

            vec_enemyPatterns.at(i).ShowGUI();

            ImGui::TreePop();
        }
        else {

            ImGui::SameLine();

            if (ImGui::Button(("Remove##" + std::to_string(i)).c_str())) {
                auto itr = vec_enemyPatterns.begin() + i;

                vec_enemyPatterns.erase(itr);
            }
        }
    }

    if(shp_timer)
    shp_timer->ShowGUI();
}
