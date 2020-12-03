#include "stdafx.h"

namespace ButiEngine {



    class Window;
    class GraphicDevice_Dx12;
    class ImguiController {
    public:
        ImguiController(std::unique_ptr<IWindow>& unq_window, std::shared_ptr<GraphicDevice_Dx12>shp_graphicDevice);
        void Start();
        void Update();
        void Draw();
        void Release();
		void SetDraggingObject(std::shared_ptr<IObject> arg_shp_draggingObject);
		void SetResourceTag(MeshTag arg_tag);
		void SetResourceTag(SoundTag arg_tag);
		void SetResourceTag(MotionTag arg_tag);
		void SetResourceTag(MaterialTag arg_tag);
		void SetResourceTag(ModelTag arg_tag);
		void SetResourceTag(ShaderTag arg_tag);
		void SetResourceTag(VertexShaderTag arg_tag);
		void SetResourceTag(PixelShaderTag arg_tag);
		void SetResourceTag(GeometryShaderTag arg_tag);


		
		
		std::shared_ptr<IObject> GetDraggingObject() {
			return shp_draggingObject;
		}

		MeshTag GetMeshTag() {
			return currentMeshTag;
		}
		SoundTag GetSoundTag() {
			return currentSoundTag;
		}
		MotionTag GetMotionTag() {
			return currentMotionTag;
		}
		MaterialTag GetMaterialTag() {
			return currentMaterialTag;
		}
		ModelTag GetModelTag() {
			return currentModelTag;
		}
		ShaderTag GetShaderTag() {
			return currentShaderTag;
		}
		VertexShaderTag GetVertexShaderTag() {
			return currentVertexShaderTag;
		}
		PixelShaderTag GetPixelShaderTag() {
			return currentPixelShaderTag;
		}
		GeometryShaderTag GetGeometryShaderTag() {
			return currentGeometryShaderTag;
		}

		GUI::GuiIO& GetGUIIO() {
			return io;
		}

    private:
        std::weak_ptr<GraphicDevice_Dx12>wkp_graphicDevice;
		GUI::GuiIO io;
		GUI::GuiIO befIo;
		std::shared_ptr<IObject> shp_draggingObject;
		MeshTag currentMeshTag;
		MaterialTag currentMaterialTag;
		SoundTag currentSoundTag;
		ModelTag currentModelTag;
		MotionTag currentMotionTag;
		ShaderTag currentShaderTag;
		VertexShaderTag currentVertexShaderTag;
		PixelShaderTag currentPixelShaderTag;
		GeometryShaderTag currentGeometryShaderTag;
		bool isWindowOpen;
		
    };
}
