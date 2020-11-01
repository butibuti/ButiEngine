// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#pragma once
#ifndef PCH_H
#define PCH_H

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

#include <xaudio2.h>	//sound
#include <xaudio2fx.h>
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

#include<dxgi1_6.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <D3dx12.h>
#include <d2d1_2.h>
#include <dwrite_2.h>
#include <wincodec.h>


#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
// XInput
#include <xinput.h>


#pragma comment( lib, "d3d12.lib" )
#pragma comment( lib, "DXGI.lib" )
#pragma comment( lib, "d2d1.lib" )
#pragma comment( lib, "dwrite.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "comctl32.lib" )


// XInput
#pragma comment( lib, "XInput.lib" )
//Audio
#pragma comment( lib, "xaudio2.lib" )
#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
//WinHTTP
#pragma comment( lib, "Winhttp.lib" )

//Zlib
#pragma comment(lib,"ZDll.lib")

#pragma warning(disable:4192)

//imgui

#include "imgui.h"
#include "./backends/imgui_impl_win32.h"
#include "./backends/imgui_impl_dx12.h"

//cerial
#include "cereal/cereal.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/polymorphic.hpp"
#include "cereal/types/map.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/array.hpp"



using namespace DirectX;
#include"Header/Common.h"

#pragma warning(default:4192)

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"windowscodecs.lib")
#include <d3dcompiler.h>
namespace DirectX
{
#if (DIRECTXMATH_VERSION < 305) && !defined(XM_CALLCONV)
#define XM_CALLCONV __fastcall
	typedef const XMVECTOR& HXMVECTOR;
	typedef const XMMATRIX& FXMMATRIX;
#endif
}


#endif //PCH_H
