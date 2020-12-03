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
            auto newEnemy= gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal(*itr);
            newEnemy.lock()->transform->TranslateZ(70);
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


    if (GUI::Button("AddEnemyPattern")) {
        vec_enemyPatterns.push_back(EnemyPattern());
    }

    for (int i = 0; i < vec_enemyPatterns.size();i++) {
        if (GUI::TreeNode(("EnemyPattern"+std::to_string(i)).c_str())) {

            vec_enemyPatterns.at(i).ShowGUI();

            GUI::TreePop();
        }
        else {

            GUI::SameLine();

            if (GUI::Button(("Remove##" + std::to_string(i)).c_str())) {
                auto itr = vec_enemyPatterns.begin() + i;

                vec_enemyPatterns.erase(itr);
            }
        }
    }

    if(shp_timer)
    shp_timer->ShowGUI();
}

void ButiEngine::EnemySpawner::SpawnBoss()
{
    std::string bossStr ="Boss_"+ gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneInformation()->GetSceneName();
    auto newEnemy = gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal(bossStr);
    newEnemy.lock()->transform->TranslateZ(70);
}
