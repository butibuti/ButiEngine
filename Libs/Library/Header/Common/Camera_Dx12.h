#include"stdafx.h"
#include"CBuffer_Dx12.h"
namespace ButiEngine {

	class Camera_Dx12 :public Camera
	{
	public:
		Camera_Dx12(const std::string& cameraName, std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice);
		Camera_Dx12(const CameraProjProperty& arg_cameraViewProp,const std::string& cameraName, std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice);
		~Camera_Dx12();
		void Initialize()override;
		void PreInitialize()override {}
		void Start()override;
		void Stop() const override;
		void ChangeMode(const BlendMode& arg_blendMode)override;
		void ShowUI()override;
	private:

		std::weak_ptr<GraphicDevice_Dx12> wkp_graphicDevice;

		D3D12_VIEWPORT    viewport;

	};
}