#include "stdafx.h"
#include "LevelSelecter.h"

void ButiEngine::LevelSelecter::Start()
{
}

void ButiEngine::LevelSelecter::OnUpdate()
{
}

std::shared_ptr< ButiEngine::GameComponent> ButiEngine::LevelSelecter::Clone()
{
    auto ret = ObjectFactory::Create<LevelSelecter>();;
    ret->nowSelectNum = nowSelectNum;
    ret->max = max;
    return ret;
}

void ButiEngine::LevelSelecter::OnShowUI()
{
}
