#pragma once

#ifndef ButiEngineHeader_H

#define ButiEngineHeader_H




#define WIN64_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#pragma once

#include<Windows.h>

// C runtime headder
#include <memory.h>
#include <tchar.h>
#include <wrl.h>

#include <assert.h>
#include <wchar.h>
#include <mmsystem.h>
#include <atlbase.h>
#include <commctrl.h> // for InitCommonControls() 
#include <math.h>
#include <limits.h>
#include<direct.h>



#include<ctime>
#include<chrono>

// STL
#include <vector>
#include <list>
#include <map>
#include<unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <exception>
#include <stdexcept>

#include<dxgi1_6.h>




#pragma comment( lib, "d3d12.lib" )
#pragma comment( lib, "DXGI.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "comctl32.lib" )





#pragma warning(disable:4192)




#pragma comment(lib,"windowscodecs.lib")
#include"Header/Common.h"
#include"Header/GameObjects/DefaultGameComponent/DefaultGameComponent.h"
#pragma warning(default:4192)


#endif // !ButiEngineHeader_H