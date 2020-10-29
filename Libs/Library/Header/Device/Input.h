#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include"stdafx.h"
#include<dinput.h>
#include<Xinput.h>
#define  KEY_MAX 256
#define CONTROLLERS_MAX 1
namespace ButiEngine {
	class Application;
	class Input
	{
	public:
		Input();
		~Input();
		void Initialize(std::weak_ptr<Application> arg_wkp_app);
		bool CheckKey(const UINT index);
		bool TriggerKey(const UINT index);
		bool CheckKey(const Keys index);
		bool TriggerKey(const Keys index);
		bool ReleaseKey(const Keys index);
		void PadUpdate();
		void MouseUpdate();
		bool GetAnyButton();
		bool GetAnyButtonTrigger();
		bool GetPadButton(const PadButtons button);
		bool GetPadButtonTriger(const PadButtons button);
		bool GetPadButtonRelease(const PadButtons button);
		bool GetMouseButton(const MouseButtons button);
		bool GetMouseTrigger(const MouseButtons button);
		bool GetMouseReleaseTrigger(const MouseButtons button);


		Vector2 GetLeftStick();
		Vector2 GetRightStick();
		Vector2 GetMouseMove();
		Vector2 GetMousePos();
		void SetMouseCursor(const Vector2& position);
	private:
		HRESULT CreateInput(void);

		HRESULT CreateKey(void);

		HRESULT CreateMouse(void);

		HRESULT SetKeyFormat(void);

		HRESULT SetMouseFormat(void);

		HRESULT SetKeyCooperative(std::weak_ptr<Application> arg_wkp_app);

		HRESULT SetMouseCooperative(std::weak_ptr<Application> arg_wkp_app);



		HRESULT result;

		LPDIRECTINPUT8 input;

		LPDIRECTINPUTDEVICE8 key;
		LPDIRECTINPUTDEVICE8 mouse;

		BYTE currentKeys[KEY_MAX];

		BYTE beffores[KEY_MAX];

		XINPUT_STATE currentPad;
		XINPUT_STATE befforePad;

		DIMOUSESTATE mouseState;
		DIMOUSESTATE beforeMouseState;
		Vector2 mousePos;
		Vector2 mouseMove = Vector2(0, 0);
		POINT mousePoint;
		std::weak_ptr<Application> wkp_app;
		bool isMouseCenterKeep;
	};
}
