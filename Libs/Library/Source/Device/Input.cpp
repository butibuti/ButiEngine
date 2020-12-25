
#pragma once
#include "../../Header/Device/Input.h"
#include "../../Header/Device/Keys.h"
#include "../../Header/Device/PadButtons.h"

#include"stdafx.h"
#include<dinput.h>
#include <xinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"xinput.lib")
using namespace::ButiEngine; 


#define DIRECTINPUT_VERSION 0x0800
#define  KEY_MAX 256
#define CONTROLLERS_MAX 1

class Input::InputInstance {
public:
	InputInstance():input(nullptr), key(nullptr) {

	}

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
	bool isMouseCenterKeep;
};

Input::Input() :result(S_OK)
{
	unq_instance = std::make_unique<InputInstance>();
}
Input::~Input() {
	unq_instance->key->Release();
	unq_instance->mouse->Release();
	unq_instance->input->Release();;
}
void ButiEngine::Input::Initialize(std::weak_ptr<IApplication> arg_wkp_app)
{
	memset(&unq_instance->currentKeys, 0, sizeof(unq_instance->currentKeys));
	memset(&unq_instance->beffores, 0, sizeof(unq_instance->beffores));
	CreateInput();
	CreateKey();
	CreateMouse();
	SetKeyFormat();
	SetMouseFormat();
	SetKeyCooperative(arg_wkp_app);
	SetMouseCooperative(arg_wkp_app);
	wkp_app = arg_wkp_app;
	MouseUpdate();
}
bool Input::GetAnyButton()
{
	return(unq_instance->currentPad.Gamepad.wButtons != 0);
}
bool Input::GetAnyButtonTrigger()
{
	return (unq_instance->currentPad.Gamepad.wButtons != 0 && unq_instance->befforePad.Gamepad.wButtons == 0);
}
bool ButiEngine::Input::GetPadButton(const PadButtons num)
{
	return Util::GetBitFlag(unq_instance->currentPad.Gamepad.wButtons, (int)num);
}
bool ButiEngine::Input::GetPadButtonTriger(const PadButtons num)
{
	return Util::GetBitFlag(unq_instance->currentPad.Gamepad.wButtons, (int)num)&& !Util::GetBitFlag(unq_instance->befforePad.Gamepad.wButtons, (int)num);
}
bool ButiEngine::Input::GetPadButtonRelease(const PadButtons num)
{
	return !Util::GetBitFlag(unq_instance->currentPad.Gamepad.wButtons, (int)num) && Util::GetBitFlag(unq_instance->befforePad.Gamepad.wButtons, (int)num);
}

bool Input::GetMouseButton(const MouseButtons num)
{
	return unq_instance->mouseState.rgbButtons[(int)num] == 128;
}

bool Input::GetMouseTrigger(const MouseButtons num)
{
	return (unq_instance->beforeMouseState.rgbButtons[(int)num] != 128) && unq_instance->mouseState.rgbButtons[(int)num] == 128;
}

bool Input::GetMouseReleaseTrigger(const MouseButtons num)
{
	return (unq_instance->beforeMouseState.rgbButtons[(int)num] == 128) && unq_instance->mouseState.rgbButtons[(int)num] != 128;
}

bool ButiEngine::Input::GetMouseWheel()
{
	if(unq_instance->mouseState.lZ!=0)
	return true;
	else {
		return false;
	}
}

float ButiEngine::Input::GetMouseWheelMove()
{
	return unq_instance->mouseState.lZ;
}

void ButiEngine::Input::SetCursorHide(const bool arg_isCursorHide)
{
	if (arg_isCursorHide) {
		ShowCursor(false);
	}
	else {
		ShowCursor(true);
	}
}



Vector2 Input::GetLeftStick()
{
	float x =
		unq_instance->currentPad.Gamepad.sThumbLX;
	float y = unq_instance->currentPad.Gamepad.sThumbLY;

	if (x < 0)x += 1;
	if (y < 0)y += 1;
	return Vector2(x / 32767, y / 32767);
}

Vector2 Input::GetRightStick()
{
	float x =
		unq_instance->currentPad.Gamepad.sThumbRX;
	float y = unq_instance->currentPad.Gamepad.sThumbRY;

	if (x < 0)x += 1;
	if (y < 0)y += 1;
	return Vector2(x / 32767, y / 32767);
}

Vector2 Input::GetMouseMove()
{
	return unq_instance->mouseMove;
}

Vector2 Input::GetMousePos()
{
	return unq_instance->mousePos;
}

HRESULT Input::CreateInput(void) {
	result = DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)(&unq_instance->input), NULL);
	return result;
}

HRESULT Input::CreateKey(void) {
	result = unq_instance->input->CreateDevice(GUID_SysKeyboard, &unq_instance->key, NULL);
	return result;
}

HRESULT Input::CreateMouse(void)
{
	result = unq_instance->input->CreateDevice(GUID_SysMouse, &unq_instance->mouse, NULL);
	return result;
}

HRESULT Input::SetKeyFormat(void) {
	result = unq_instance->key->SetDataFormat(&c_dfDIKeyboard);
	return result;
}

HRESULT Input::SetMouseFormat(void)
{
	result = unq_instance->mouse->SetDataFormat(&c_dfDIMouse);
	return result;
}

HRESULT Input::SetKeyCooperative(std::weak_ptr<IApplication> arg_wkp_app) {
	result = unq_instance->key->SetCooperativeLevel(arg_wkp_app.lock()->GetWindow()->GetHandle(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	unq_instance->key->Acquire();
	return result;
}

HRESULT Input::SetMouseCooperative(std::weak_ptr<IApplication> arg_wkp_app)
{
	result = unq_instance->mouse->SetCooperativeLevel(arg_wkp_app.lock()->GetWindow()->GetHandle(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	DIPROPDWORD diProperty;
	diProperty.diph.dwSize = sizeof(diProperty);
	diProperty.diph.dwHeaderSize = sizeof(diProperty.diph);
	diProperty.diph.dwObj = 0;
	diProperty.diph.dwHow = DIPH_DEVICE;
	diProperty.dwData = DIPROPAXISMODE_REL;
	result = unq_instance->mouse->SetProperty(DIPROP_AXISMODE, &diProperty.diph);
	unq_instance->mouse->Acquire();

	return result;
}

void ButiEngine::Input::SetMouseCursor(const Vector2& position)
{

	SetCursorPos(position.x, position.y);
	unq_instance->mousePos = position;
}


bool Input::CheckKey(const UINT index) {
	bool checkFlag = false;

	if (unq_instance->currentKeys[index] & 0x80) {
		checkFlag = true;
	}
	return checkFlag;
}
bool Input::CheckKey(const Keys index) {
	bool checkFlag = false;
	int nIndex = (int)index;
	if (unq_instance->currentKeys[nIndex] & 0x80) {
		checkFlag = true;
	}
	return checkFlag;
}
bool Input::TriggerKey(const UINT index) {
	bool checkFlag = false;


	if ((unq_instance->currentKeys[index] & 0x80) && !(unq_instance->beffores[index] & 0x80)) {
		checkFlag = true;
	}

	return checkFlag;
}
bool Input::TriggerKey(const Keys index) {
	bool checkFlag = false;
	int nIndex = (int)index;

	if ((unq_instance->currentKeys[nIndex] & 0x80) && !(unq_instance->beffores[nIndex] & 0x80)) {
		checkFlag = true;
	}

	return checkFlag;
}

bool ButiEngine::Input::ReleaseKey(const Keys index)
{
	bool checkFlag = false;
	int nIndex = (int)index;

	if (!(unq_instance->currentKeys[nIndex] & 0x80) && (unq_instance->beffores[nIndex] & 0x80)) {
		checkFlag = true;
	}

	return checkFlag;
}

void Input::PadUpdate()
{
	DWORD dwResult;

	unq_instance->befforePad = unq_instance->currentPad;
	for (DWORD i = 0; i < CONTROLLERS_MAX; i++)
	{
		ZeroMemory(&unq_instance->currentPad, sizeof(unq_instance->currentPad));

		dwResult = XInputGetState(i, &unq_instance->currentPad);

		if (dwResult == ERROR_SUCCESS)
		{
			// Controller is connected
			dwResult = dwResult;
		}
		else
		{
			// Controller is not connected 
			//std::cout<<"Controller is not connected"<<std::endl;
			dwResult = dwResult;
		}
	}

}

void Input::MouseUpdate()
{
	unq_instance->beforeMouseState = unq_instance->mouseState;
	auto hr= unq_instance->mouse->GetDeviceState(sizeof(DIMOUSESTATE), &unq_instance->mouseState);

	if (hr !=S_OK) {
		unq_instance->mouse->Acquire();
		hr = unq_instance->mouse->GetDeviceState(sizeof(DIMOUSESTATE), &unq_instance->mouseState);
	}
	GetCursorPos(&unq_instance->mousePoint);
	unq_instance->mouseMove.x = unq_instance->mousePos.x- unq_instance->mousePoint.x;
	unq_instance->mouseMove.y = unq_instance->mousePos.y- unq_instance->mousePoint.y;

	unq_instance->mousePos.x = unq_instance->mousePoint.x; unq_instance->mousePos.y = unq_instance->mousePoint.y;

	for (int i = 0; i < 256; i++) {
		unq_instance->beffores[i] = unq_instance->currentKeys[i];
	}

	unq_instance->key->GetDeviceState(sizeof(unq_instance->currentKeys), &unq_instance->currentKeys);
}
