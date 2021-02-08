#include"stdafx.h"
#include"Header/Common/Camera_Dx12.h"

#include"Header/Common/CollisionPrimitive.h"
#include "..\..\Header\Common\Camera.h"

std::string ButiEngine::Camera::GetName() const
{
	return cameraName;
}

void ButiEngine::Camera::SetName(const std::string& arg_name)
{
	cameraName = arg_name;
	cameraViewProp.cameraName = arg_name;
}

void ButiEngine::Camera::Switch()
{
	isActive = !isActive;
}

void ButiEngine::Camera::SetActive(const bool arg_active)
{
	isActive = arg_active;
}

bool ButiEngine::Camera::GetActive() const
{
	return isActive;
}

void ButiEngine::Camera::Draw()
{
	shp_renderer->Rendering(cameraViewProp.layer);
}

void ButiEngine::Camera::BefDraw()
{
	cameraPos= shp_transform->GetWorldPosition();
	viewMatrix = shp_transform->GetMatrix().GetInverse();
}

ButiEngine::CameraProjProperty& ButiEngine::Camera::GetCameraProperty()
{
	return cameraViewProp;
}

int ButiEngine::Camera::IsContaineVisibility(std::shared_ptr<Geometry::Box_AABB> arg_AABB)
{
	if (Geometry::SurfaceHit::IsHitAABBCameraFrustum(*arg_AABB, shp_transform, projectionMatrix, cameraViewProp.nearClip, cameraViewProp.farClip)) {


		auto ray = ObjectFactory::Create<Collision::CollisionPrimitive_Segment>(shp_transform, arg_AABB->position);
		//ray->Update();
		auto objects = shp_renderer->GetHitDrawObjects(ray, cameraViewProp.layer);

		return objects.size();
	}
	return -1;
}

std::shared_ptr< ButiEngine::ICamera> ButiEngine::CameraCreater::CreateCamera(const CameraProjProperty& arg_cameraViewProp, const std::string& cameraName, const bool initActive, std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice> arg_wkp_graphicDevice)
{
	std::shared_ptr<ICamera> out;
	out = ObjectFactory::Create<Camera_Dx12>(arg_cameraViewProp, cameraName,arg_shp_renderer, arg_wkp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>());
	
	out->SetActive(initActive);
	return out;
}

std::shared_ptr< ButiEngine::ICamera> ButiEngine::CameraCreater::CreateCamera(const std::string& cameraName, const bool initActive, std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice> arg_wkp_graphicDevice)
{
	CameraProjProperty cameraViewProp = CameraProjProperty(0, 0, 0, 0);
	std::shared_ptr<ICamera> out;
	out = ObjectFactory::Create<Camera_Dx12>(cameraViewProp, cameraName, arg_shp_renderer, arg_wkp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>());
	
	out->SetActive(initActive);
	return out;
}

ButiEngine::CameraProjProperty::CameraProjProperty(const UINT widthScale, const UINT heightScale, const UINT x, const UINT y, const bool arg_isPararell, UINT arg_layer)
	:top(y), left(x),isPararell(arg_isPararell),layer(arg_layer)
{
	width = widthScale;
	height = heightScale;
}
