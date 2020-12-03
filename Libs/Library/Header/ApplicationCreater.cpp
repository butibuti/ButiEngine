#include"stdafx.h"

#include"Header/GameParts/Application.h"
std::shared_ptr<ButiEngine::IApplication> ButiEngine::CreateDefaultApplicationInstance(const std::string windowName, const WindowPopType arg_windowPopType, const UINT windowWidth, const UINT windowHeight, const bool isFullScreen)
{
    auto app = ObjectFactory::Create<Application>();

    app->CreateInstances(windowName, arg_windowPopType, windowWidth, windowHeight, isFullScreen);
    return app;
}
