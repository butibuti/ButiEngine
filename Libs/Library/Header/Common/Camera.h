#pragma once
#include"../Device/ObjectFactory.h"
#include"Geometry.h"
namespace ButiEngine {


	class Renderer;

	struct CameraProjProperty {
		CameraProjProperty() {};
		CameraProjProperty(const UINT widthScale, const UINT heightScale, const UINT x, const UINT y,const bool arg_isPararell=false, UINT arg_layer=0) ;

		int width=0;
		int height=0;
		int left=0;
		int top=0;
		float front = 0.0f;
		float angle=60.0f;
		float farClip = 50.0f;
		float nearClip = 0.1f;
		bool isPararell = false;
		UINT layer = 0;
		float clearDepth = 1.0f;
		TextureTag projectionTexture;
		std::string cameraName;
		Vector4 clearColor;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(width);
			archive(height);
			archive(left);
			archive(top);
			archive(front);
			archive(angle);
			archive(farClip);
			archive(nearClip);
			archive(isPararell);
			archive(layer);
			archive(front);
			archive(clearDepth);
			archive(projectionTexture);
			archive(cameraName);
			archive(clearColor);
		}

	};

	class ICamera :public IObject {
	public:

		std::shared_ptr<Transform> shp_transform = ObjectFactory::Create<Transform>(Vector3(0, 0, 0));


		virtual void Start()=0;
		virtual void Stop()const=0;
		virtual void ChangeMode(const BlendMode& arg_blendMode) = 0;
		virtual void Initialize()override {}
		virtual void PreInitialize()override{}
		virtual std::string GetName() const = 0;
		virtual void SetName(const std::string& arg_name)=0 ;
		virtual void Switch()=0;
		virtual void SetActive(const bool arg_active)=0;
		virtual bool GetActive()const=0;
		virtual void Draw() = 0;
		virtual void ShowUI() = 0;
		virtual CameraProjProperty& GetCameraProperty() = 0;
		virtual int IsContaineVisibility(std::shared_ptr<Geometry::Box_AABB>arg_AABB) = 0;
		virtual void End() = 0;
		virtual void SetProjectionTexture(const TextureTag& arg_tag)=0;
		virtual void BefDraw()=0;
	protected:
	};

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