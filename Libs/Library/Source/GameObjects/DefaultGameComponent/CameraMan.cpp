#include "stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\CameraMan.h"

void ButiEngine::CameraMan::Start()
{

	auto camera = GetCamera("main");
	camera.lock()->shp_transform = gameObject.lock()->transform;
}
