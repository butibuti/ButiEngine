#include"stdafx.h"
#include"DrawData_Dx12.h"
#include"../../Common/CArrayBuffer_Dx12.h"
namespace ButiEngine {
	struct ModelDrawData_Dx12 :public ModelDrawData,public DrawData_Dx12 {
		ModelDrawData_Dx12(const ModelTag& arg_model, const ShaderTag& arg_shader, std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice,  std::shared_ptr< DrawInformation >arg_shp_drawInfo,std::shared_ptr<Transform>arg_shp_transform);
		

		inline float GetZ()override {
			return  GetMaxZ( wkp_graphicDevice.lock()->GetRawViewMatrix());
		}
		void Initialize()override;

		void PreInitialize()override {};

		void DrawBefore()override;

		void SetTransform(std::shared_ptr<Transform>& arg_transform)override {
			transform = arg_transform->GetMatrix();
		}

	
		void Draw()override;

		void  ModelUpdate() override;
	};
}