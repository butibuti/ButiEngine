#include "stdafx.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\CameraMan.h"

float ButiEngine::CameraMan::horizonQuakeMax = 0.25;
float ButiEngine::CameraMan::vertQuakeMax = 0.25;

void ButiEngine::CameraMan::Start()
{

	auto camera = GetCamera(cameraName);
	child = ObjectFactory::Create<Transform>();
	child->SetBaseTransform(gameObject.lock()->transform,true);
	camera.lock()->shp_transform =child;
}

void ButiEngine::CameraMan::OnUpdate()
{
	auto hq= min(horizonQuake, horizonQuakeMax);;
	auto vq = min(vertQuakeMax, vertQuake);
	child->SetLocalPosition(Vector3(ButiRandom::GetRandom<float>(-hq,hq,10), ButiRandom::GetRandom<float>(-vq, vq, 10),0));

	horizonQuake *= 0.9;
	vertQuake *= 0.9;

}

void ButiEngine::CameraMan::ShakeVartical(const float power)
{
	vertQuake += power;
}

void ButiEngine::CameraMan::ShakeHorizontal(const float power)
{
	horizonQuake += power;
}

void ButiEngine::CameraMan::OnShowUI()
{
	GUI::BulletText("CameraName");
	static char cameraNameBuf[128] = {};
	GUI::InputText("##cameraName",cameraNameBuf,128);
	GUI::SameLine();
	if (GUI::Button("Change")) {
		cameraName = cameraNameBuf;
		memset(cameraNameBuf, 0, 128);
	}
}
