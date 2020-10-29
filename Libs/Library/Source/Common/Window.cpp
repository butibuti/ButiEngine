#include"stdafx.h"
#include "..\..\Header\Common\Window.h"

ButiEngine::Window::Window()
{
}

void ButiEngine::Window::Initialize(const std::string arg_windowName, const WindowPopType arg_popType, UINT width , UINT height)
{
	std::wstring wName;
	Util::MultiBytetoWString(arg_windowName, wName);

	HINSTANCE instance = GetModuleHandleW(nullptr);

	WNDCLASSW windowClass = {};
	windowClass.lpfnWndProc = ProceedMessage;
	windowClass.hInstance = instance;
	windowClass.hCursor = (HCURSOR)LoadImageW(nullptr, MAKEINTRESOURCEW(32512)
		, IMAGE_CURSOR, 0, 0, LR_SHARED
	);
	windowClass.lpszClassName = wName.c_str();
	RegisterClassW(&windowClass);

	handle = CreateWindowW(wName.c_str(), wName.c_str(),
		WS_OVERLAPPEDWINDOW
		//	WS_POPUP
		, 0, 0, 0, 0, nullptr, nullptr, instance, nullptr);
	SetSize(width, height);
	ShowWindow(handle,
		//SW_SHOWNORMAL
			(int)arg_popType
	);

}

HWND ButiEngine::Window::GetHandle()
{
	return handle;
}

DirectX::XMUINT2 ButiEngine::Window::GetSize()
{
	RECT clienRect = {};
	GetClientRect(handle, &clienRect);

	return DirectX::XMUINT2(clienRect.right - clienRect.left, clienRect.bottom - clienRect.top);
}

ButiEngine::Vector2 ButiEngine::Window::GetWindowCenterPosition()
{
	auto windowSize = GetSize();
	auto winPos = GetWindowPosition();

	return Vector2(winPos.x + 2 + windowSize.x / 2, winPos.y + 27.0f + windowSize.y / 2);
}

void ButiEngine::Window::SetSize(int width, int height)
{
	RECT windowRect = {};
	RECT clientRect = {};
	GetWindowRect(handle, &windowRect);
	GetClientRect(handle, &clientRect);

	int w = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left) + width;
	int h = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top) + height;
	int x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
	int y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;
	SetWindowPos(handle, nullptr, x, y, w, h, SWP_FRAMECHANGED);
}

bool ButiEngine::Window::Update()
{
	MSG message = {};
	while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE))
	{//ウィンドウの操作がされると呼ばれる
		if (message.message == WM_QUIT)
			return false;
		TranslateMessage(&message);
		DispatchMessageW(&message);//バツぼたん、ウィンドウの拡大縮小。

	}
	return true;
}

ButiEngine::Vector2 ButiEngine::Window::GetWindowPosition()
{
	RECT rect;
	GetWindowRect(handle, &rect);
	return Vector2((float)rect.left,(float) rect.top);
}

void ButiEngine::Window::Release()
{
}

LRESULT ButiEngine::Window::ProceedMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_DESTROY)PostQuitMessage(0);

	return DefWindowProcW(window, message, wParam, lParam);
}
