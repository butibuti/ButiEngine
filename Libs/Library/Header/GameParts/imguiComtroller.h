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
    private:
        std::weak_ptr<GraphicDevice_Dx12>wkp_graphicDevice;
    };
}
