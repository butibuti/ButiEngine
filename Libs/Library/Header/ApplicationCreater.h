#include<string>
#include"GameParts/GamePartsInterface.h"
namespace ButiEngine {

	std::shared_ptr<IApplication> CreateDefaultApplicationInstance(const std::string windowName = "Buti", const WindowPopType arg_windowPopType = WindowPopType::normal, const UINT windowWidth = 720, const UINT windowHeight = 480, const bool isFullScreen = false);
	std::shared_ptr<IApplication> CreateEditorApplicationInstance(const std::string windowName = "Buti", const WindowPopType arg_windowPopType = WindowPopType::normal, const UINT windowWidth = 720, const UINT windowHeight = 480, const bool isFullScreen = false);

}