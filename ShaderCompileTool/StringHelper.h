#pragma once

#include<Windows.h>

// C runtime headder
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <wrl.h>

#include <initguid.h>
#include <assert.h>
#include <wchar.h>
#include <mmsystem.h>
#include <atlbase.h>
#include <commctrl.h> // for InitCommonControls() 
#include <shellapi.h> // for ExtractIcon()
#include <new.h>      // for placement new
#include <shlobj.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <Winhttp.h>
#include<direct.h>

#include <mmreg.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>
#include <msxml6.h>


// STL
#include <vector>
#include <list>
#include <map>
#include<unordered_map>
#include <set>
#include <stack>
#include <functional>
#include <algorithm>
#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <sstream>
#include <string>
#include <new>
#include <memory>
#include <exception>
#include <stdexcept>
#include <type_traits>
#include <typeindex>

#include <d3d11_2.h>
#include<dxgi1_4.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <D3dx12.h>
#include <d2d1_2.h>
#include <d2d1effects_1.h>
#include <dwrite_2.h>
#include <wincodec.h>


#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
// XInput includes
#include <xinput.h>


#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3d12.lib" )
#pragma comment( lib, "DXGI.lib" )
#pragma comment( lib, "d2d1.lib" )
#pragma comment( lib, "dwrite.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "comctl32.lib" )



#pragma warning(disable:4192)

using namespace DirectX;

#pragma warning(default:4192)

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"windowscodecs.lib")
#include <d3dcompiler.h>

namespace  ButiEngine {
	class StringHelper
	{
	public:
		~StringHelper();
		static std::vector<std::string> Split(const std::string& source, const std::string& cuttingSouece);
		static std::string Cut(const std::string& source, const std::string& frontCuttingSouece, const std::string& backCuttingSouece, bool isContaisCutSource = true);
		static std::string Remove(const std::string& source, const std::string& removeSouece);
		static std::string RemoveComment(const std::string& source);
		static bool Contains(const std::string& source, const std::string& findSource);
		static bool Contains(const std::string& source, const char& findSource);
		static void WStringToSafetyConvert(std::wstring& source);
		static std::string GetDirectory(const std::string& source);
		static std::string GetFileName(const std::string& source, const bool& isContainExtension);
	private:
		StringHelper();
	};

}