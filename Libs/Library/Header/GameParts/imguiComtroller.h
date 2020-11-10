#include "stdafx.h"

namespace ButiEngine {


	namespace CallBacks
	{
		static int ImguiCallBack(ImGuiInputTextCallbackData* data)
		{
			if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion)
			{
				data->InsertChars(data->CursorPos, "..");
			}
			else if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory)
			{
				if (data->EventKey == ImGuiKey_UpArrow)
				{
					data->DeleteChars(0, data->BufTextLen);
					data->InsertChars(0, "Pressed Up!");
					data->SelectAll();
				}
				else if (data->EventKey == ImGuiKey_DownArrow)
				{
					data->DeleteChars(0, data->BufTextLen);
					data->InsertChars(0, "Pressed Down!");
					data->SelectAll();
				}
			}
			else if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit)
			{
				// Toggle casing of first character
				char c = data->Buf[0];
				if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) data->Buf[0] ^= 32;
				data->BufDirty = true;

				// Increment a counter
				int* p_int = (int*)data->UserData;
				*p_int = *p_int + 1;
			}
			return 0;
		}


		static char objectName[128];
		static char cbufferName[128];
	};

    class Window;
    class GraphicDevice_Dx12;
    class ButiimguiController {
    public:
        ButiimguiController(std::unique_ptr<Window>& unq_window, std::shared_ptr<GraphicDevice_Dx12>shp_graphicDevice);
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

		ImGuiIO& GetGUIIO() {
			return io;
		}

    private:
        std::weak_ptr<GraphicDevice_Dx12>wkp_graphicDevice;
		ImGuiIO io;
		ImGuiIO befIo;
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

		
    };
}
