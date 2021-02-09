#pragma once
#include"../Device/ObjectFactory.h"
#include"Geometry.h"
namespace ButiEngine {





	class  Camera:public ICamera
	{
	public:
		std::string GetName() const override;
		void SetName(const std::string& arg_name) override;
		void Switch()override;
		void SetActive(const bool arg_active) override;
		bool GetActive()const override;
		void Draw()override;
		void BefDraw()override;
		CameraProjProperty& GetCameraProperty()override;
		int IsContaineVisibility(std::shared_ptr<Geometry::Box_AABB>arg_AABB)override;
	protected:
		Matrix4x4 projectionMatrix;
		Matrix4x4 viewMatrix;
		Vector3 cameraPos;
		CameraProjProperty cameraViewProp; 
		std::shared_ptr<IRenderer> shp_renderer;
		bool isActive = true;
		std::string cameraName;
	private:

	};
	class CameraCreater {
	public:
		static std::shared_ptr<ICamera> CreateCamera(const CameraProjProperty& arg_cameraViewProp, const std::string& cameraName,const bool initActive,std::shared_ptr<IRenderer> arg_shp_renderer,std::weak_ptr<GraphicDevice> arg_wkp_graphicDevice);
		static std::shared_ptr<ICamera> CreateCamera(const std::string& cameraName, const bool initActive, std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice> arg_wkp_graphicDevice);
	};
}