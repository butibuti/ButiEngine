//#include "stdafx.h"

namespace ButiEngine {
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
