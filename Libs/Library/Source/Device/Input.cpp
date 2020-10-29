#include"stdafx.h"
#pragma once
#include "../../Header/Device/Input.h"
#include "../../Header/Device/Keys.h"
#include "../../Header/Device/PadButtons.h"
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"xinput.lib")
#define Release(X) {if((X)!=nullptr) (X)->Release();(X)=nullptr;}
using namespace::ButiEngine;


Input::Input() :result(S_OK), input(nullptr), key(nullptr)
{
}
Input::~Input() {
	Release(key);
	Release(mouse);
	Release(input);
}
void ButiEngine::Input::Initialize(std::weak_ptr<Application> arg_wkp_app)
{
	memset(&currentKeys, 0, sizeof(currentKeys));
	memset(&beffores, 0, sizeof(beffores));
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
	return(currentPad.Gamepad.wButtons != 0);
}
bool Input::GetAnyButtonTrigger()
{
	return (currentPad.Gamepad.wButtons != 0 && befforePad.Gamepad.wButtons == 0);
}
bool ButiEngine::Input::GetPadButton(const PadButtons num)
{
	return Util::GetBitFlag(currentPad.Gamepad.wButtons, (int)num);
}
bool ButiEngine::Input::GetPadButtonTriger(const PadButtons num)
{
	return Util::GetBitFlag(currentPad.Gamepad.wButtons, (int)num)&& !Util::GetBitFlag(befforePad.Gamepad.wButtons, (int)num);
}
bool ButiEngine::Input::GetPadButtonRelease(const PadButtons num)
{
	return !Util::GetBitFlag(currentPad.Gamepad.wButtons, (int)num) && Util::GetBitFlag(befforePad.Gamepad.wButtons, (int)num);
}

bool Input::GetMouseButton(const MouseButtons num)
{
	return mouseState.rgbButtons[(int)num] == 128;
}

bool Input::GetMouseTrigger(const MouseButtons num)
{
	return (beforeMouseState.rgbButtons[(int)num] != 128) && mouseState.rgbButtons[(int)num] == 128;
}

bool Input::GetMouseReleaseTrigger(const MouseButtons num)
{
	return (beforeMouseState.rgbButtons[(int)num] == 128) && mouseState.rgbButtons[(int)num] != 128;
}



Vector2 Input::GetLeftStick()
{
	float x =
		currentPad.Gamepad.sThumbLX;
	float y = currentPad.Gamepad.sThumbLY;

	if (x < 0)x += 1;
	if (y < 0)y += 1;
	return Vector2(x / 32767, y / 32767);
}

Vector2 Input::GetRightStick()
{
	float x =
		currentPad.Gamepad.sThumbRX;
	float y = currentPad.Gamepad.sThumbRY;

	if (x < 0)x += 1;
	if (y < 0)y += 1;
	return Vector2(x / 32767, y / 32767);
}

Vector2 Input::GetMouseMove()
{
	return mouseMove;
}

Vector2 Input::GetMousePos()
{
	return mousePos;
}

HRESULT Input::CreateInput(void) {
	result = DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)(&input), NULL);
	return result;
}

HRESULT Input::CreateKey(void) {
	result = input->CreateDevice(GUID_SysKeyboard, &key, NULL);
	return result;
}

HRESULT Input::CreateMouse(void)
{
	result = input->CreateDevice(GUID_SysMouse, &mouse, NULL);
	return result;
}

HRESULT Input::SetKeyFormat(void) {
	result = key->SetDataFormat(&c_dfDIKeyboard);
	return result;
}

HRESULT Input::SetMouseFormat(void)
{
	result = mouse->SetDataFormat(&c_dfDIMouse);
	return result;
}

HRESULT Input::SetKeyCooperative(std::weak_ptr<Application> arg_wkp_app) {
	result = key->SetCooperativeLevel(arg_wkp_app.lock()->GetWindow()->GetHandle(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	key->Acquire();
	return result;
}

HRESULT Input::SetMouseCooperative(std::weak_ptr<Application> arg_wkp_app)
{
	result = mouse->SetCooperativeLevel(arg_wkp_app.lock()->GetWindow()->GetHandle(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	DIPROPDWORD diProperty;
	diProperty.diph.dwSize = sizeof(diProperty);
	diProperty.diph.dwHeaderSize = sizeof(diProperty.diph);
	diProperty.diph.dwObj = 0;
	diProperty.diph.dwHow = DIPH_DEVICE;
	diProperty.dwData = DIPROPAXISMODE_REL;
	result = mouse->SetProperty(DIPROP_AXISMODE, &diProperty.diph);
	mouse->Acquire();

	return result;
}

void ButiEngine::Input::SetMouseCursor(const Vector2& position)
{

	SetCursorPos(position.x, position.y);
	mousePos = position;
}


bool Input::CheckKey(const UINT index) {
	bool checkFlag = false;

	if (currentKeys[index] & 0x80) {
		checkFlag = true;
	}
	return checkFlag;
}
bool Input::CheckKey(const Keys index) {
	bool checkFlag = false;
	int nIndex = (int)index;
	if (currentKeys[nIndex] & 0x80) {
		checkFlag = true;
	}
	return checkFlag;
}
bool Input::TriggerKey(const UINT index) {
	bool checkFlag = false;


	if ((currentKeys[index] & 0x80) && !(beffores[index] & 0x80)) {
		checkFlag = true;
	}

	return checkFlag;
}
bool Input::TriggerKey(const Keys index) {
	bool checkFlag = false;
	int nIndex = (int)index;

	if ((currentKeys[nIndex] & 0x80) && !(beffores[nIndex] & 0x80)) {
		checkFlag = true;
	}

	return checkFlag;
}

bool ButiEngine::Input::ReleaseKey(const Keys index)
{
	bool checkFlag = false;
	int nIndex = (int)index;

	if (!(currentKeys[nIndex] & 0x80) && (beffores[nIndex] & 0x80)) {
		checkFlag = true;
	}

	return checkFlag;
}

void Input::PadUpdate()
{
	DWORD dwResult;

	befforePad = currentPad;
	for (DWORD i = 0; i < CONTROLLERS_MAX; i++)
	{
		ZeroMemory(&currentPad, sizeof(currentPad));

		dwResult = XInputGetState(i, &currentPad);

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
	beforeMouseState = mouseState;
	mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);

	GetCursorPos(&mousePoint);
	mouseMove.x =   mousePos.x- mousePoint.x;
	mouseMove.y =   mousePos.y- mousePoint.y;

	mousePos.x = mousePoint.x; mousePos.y = mousePoint.y;

	for (int i = 0; i < 256; i++) {
		beffores[i] = currentKeys[i];
	}

	key->GetDeviceState(sizeof(currentKeys), &currentKeys);
}
