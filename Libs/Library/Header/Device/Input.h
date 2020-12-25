#pragma once
#include"../Device/Keys.h"
#include"../Device/MouseButtons.h"
#include"../Device/PadButtons.h"
namespace ButiEngine {
	class IApplication;
	class Input
	{
		class InputInstance;
	public:
		Input();
		~Input();
		void Initialize(std::weak_ptr<IApplication> arg_wkp_app);
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
		bool GetMouseWheel();
		float GetMouseWheelMove();
		void SetCursorHide(const bool arg_isCursorHide);

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

		HRESULT SetKeyCooperative(std::weak_ptr<IApplication> arg_wkp_app);

		HRESULT SetMouseCooperative(std::weak_ptr<IApplication> arg_wkp_app);



		HRESULT result;


		std::unique_ptr<InputInstance> unq_instance;

		std::weak_ptr<IApplication> wkp_app;

	};
}
