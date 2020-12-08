
#ifndef GUIController_H
#define GUIController_H
namespace ButiEngine {



    class IWindow;
    class GraphicDevice_Dx12;

    class ImguiController {
		class ImguiControllerInstance;
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
		void SetResourceTag(TextureTag arg_tag);


		
		
		std::shared_ptr<IObject> GetDraggingObject();

		MeshTag GetMeshTag();
		SoundTag GetSoundTag();
		MotionTag GetMotionTag();
		MaterialTag GetMaterialTag();
		ModelTag GetModelTag();
		ShaderTag GetShaderTag();
		VertexShaderTag GetVertexShaderTag();
		PixelShaderTag GetPixelShaderTag();
		GeometryShaderTag GetGeometryShaderTag();
		TextureTag GetTextureTag();

		GUI::GuiIO& GetGUIIO();

    private:


		std::unique_ptr<ImguiControllerInstance> unq_instance;
		
    };

	class ImguiController::ImguiControllerInstance {
	public:
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
		TextureTag currentTextureTag;
		bool isWindowOpen;
	};
}

#endif // !GUIController_H