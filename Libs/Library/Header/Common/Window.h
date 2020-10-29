#pragma once
#include"stdafx.h"

namespace ButiEngine {

	enum class WindowPopType {
		normal=SW_SHOWNORMAL,max=SW_SHOWMAXIMIZED
	};

	class Window
	{
	public:
		Window();
		void Initialize(const std::string arg_windowName,const WindowPopType arg_popType ,  UINT width=0, UINT height = 0);
		HWND GetHandle();
		DirectX::XMUINT2 GetSize();
		Vector2 GetWindowCenterPosition();
		void SetSize(int width, int height);
		bool Update();
		Vector2 GetWindowPosition();
		void Release();
	private:
		HWND handle;

		static LRESULT CALLBACK ProceedMessage(HWND window,
			UINT message, WPARAM wParam, LPARAM lParam);
	};
}