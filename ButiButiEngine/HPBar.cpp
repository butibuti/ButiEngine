#include "stdafx.h"
#include "HPBar.h"

void ButiEngine::HPBar::OnUpdate()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::HPBar::Clone()
{
    auto ret = ObjectFactory::Create<HPBar>();
    ret->max = max;
    ret->now = now;
    ret->width = width;
    return ret;
}

void ButiEngine::HPBar::SetHP(const UINT arg_hp)
{
    now = arg_hp;


    now= min(max, now);

    gameObject.lock()->transform->SetLocalScale().x=(width/max)*now;

}

void ButiEngine::HPBar::OnShowUI()
{
}
