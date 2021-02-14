#pragma once
#include"stdafx.h"

//imgui

#include "imgui.h"
#include "./backends/imgui_impl_win32.h"
#include "./backends/imgui_impl_dx12.h"
#include "..\..\Header\Device\GUI.h"

int DefaultTextCallBack(ImGuiInputTextCallbackData* data)
{
	if (data->EventFlag == ImGuiInputTextFlags_CallbackCompletion)
	{
		data->InsertChars(data->CursorPos, "..");
	}
	else if (data->EventFlag == ImGuiInputTextFlags_CallbackHistory)
	{
		if (data->EventKey == ImGuiKey_UpArrow)
		{
			data->DeleteChars(0, data->BufTextLen);
			data->InsertChars(0, "Pressed Up!");
			data->SelectAll();
		}
		else if (data->EventKey == ImGuiKey_DownArrow)
		{
			data->DeleteChars(0, data->BufTextLen);
			data->InsertChars(0, "Pressed Down!");
			data->SelectAll();
		}
	}
	else if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit)
	{
		// Toggle casing of first character
		char c = data->Buf[0];
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) data->Buf[0] ^= 32;
		data->BufDirty = true;

		// Increment a counter
		int* p_int = (int*)data->UserData;
		*p_int = *p_int + 1;
	}
	return 0;
}

ButiEngine::GUI::GuiIO ConvertIO( ImGuiIO& arg_io) {
	ButiEngine::GUI::GuiIO out;

	out.BackendFlags = arg_io.BackendFlags;
	out.ConfigFlags = arg_io.ConfigFlags;
	out. BackendFlags=arg_io.BackendFlags;
	out.    DisplaySize=ButiEngine:: Vector2( arg_io.DisplaySize.x, arg_io.DisplaySize.y);
	out.      DeltaTime= arg_io.DeltaTime;
	out.      IniSavingRate = arg_io.IniSavingRate;
	const char* IniFilename = arg_io.IniFilename;
	const char* LogFilename = arg_io.LogFilename;
	out.      MouseDoubleClickTime = arg_io.MouseDoubleClickTime;
	out.      MouseDoubleClickMaxDist= arg_io.MouseDoubleClickMaxDist;
	out.      MouseDragThreshold = arg_io.MouseDragThreshold;
	memcpy_s(out.KeyMap, 22, arg_io.KeyMap, 22);
	out.      KeyRepeatDelay = arg_io.KeyRepeatDelay;
	out.      KeyRepeatRate = arg_io.KeyRepeatRate;
	out.UserData = arg_io.UserData;
	out.      FontGlobalScale = arg_io.FontGlobalScale;
	out.       FontAllowUserScaling = arg_io.FontAllowUserScaling;

	out.    DisplayFramebufferScale = ButiEngine::Vector2(arg_io.DisplayFramebufferScale.x, arg_io.DisplayFramebufferScale.y);

	// Miscellaneous options
	out.       MouseDrawCursor = arg_io.MouseDrawCursor;
	out.       ConfigMacOSXBehaviors = arg_io.ConfigMacOSXBehaviors;
	out.       ConfigInputTextCursorBlink = arg_io.ConfigInputTextCursorBlink;
	out.       ConfigWindowsResizeFromEdges = arg_io.ConfigWindowsResizeFromEdges;
	out.       ConfigWindowsMoveFromTitleBarOnly = arg_io.ConfigWindowsMoveFromTitleBarOnly;
	out.      ConfigWindowsMemoryCompactTimer = arg_io.ConfigWindowsMemoryCompactTimer;

	// Optional: Platform/Renderer backend name (informational only! will be displayed in About Window) + User data for backend/wrappers to store their own stuff.
	const char* BackendPlatformName = arg_io.BackendPlatformName;
	const char* BackendRendererName = arg_io.BackendRendererName;
	out.BackendPlatformUserData = arg_io.BackendPlatformUserData;
	out.BackendRendererUserData=arg_io.BackendRendererUserData;
	out.BackendLanguageUserData=arg_io.BackendLanguageUserData;



	out.GetClipboardTextFn = arg_io.GetClipboardTextFn;
	out.SetClipboardTextFn = arg_io.SetClipboardTextFn;
	out.ClipboardUserData=arg_io.ClipboardUserData;

	

	//out.eSetInputScreenPosFn = arg_io.ImeSetInputScreenPosFn;
	//out.eWindowHandle = arg_io.ImeWindowHandle;


	out.MousePos =ButiEngine::Vector2( arg_io.MousePos.x, arg_io.MousePos.y);
	memcpy_s(out.MouseDown,5,arg_io.MouseDown,5);
	out.      MouseWheel=arg_io.MouseWheel;
	out.      MouseWheelH=arg_io.MouseWheelH;
	out.       KeyCtrl=arg_io.KeyCtrl;
	out.       KeyShift=arg_io.KeyShift;
	out.       KeyAlt=arg_io.KeyAlt;
	out.       KeySuper=arg_io.KeySuper;

	memcpy_s(out.KeysDown, 512, arg_io.KeysDown, 512);
	memcpy_s(out.NavInputs, 21, arg_io.NavInputs, 21);

	// Functions              
	out.       WantCaptureMouse=arg_io.WantCaptureMouse;
	out.       WantCaptureKeyboard=arg_io.WantCaptureKeyboard;
	out.       WantTextInput=arg_io.WantTextInput;
	out.       WantSetMousePos=arg_io.WantSetMousePos;
	out.       WantSaveIniSettings=arg_io.WantSaveIniSettings;
	out.       NavActive=arg_io.NavActive;
	out.       NavVisible=arg_io.NavVisible;
	out.      Framerate=arg_io.Framerate;
	out.       MetricsRenderVertices=arg_io.MetricsRenderVertices;
	out.       MetricsRenderIndices=arg_io.MetricsRenderIndices;
	out.       MetricsRenderWindows=arg_io.MetricsRenderWindows;
	out.       MetricsActiveWindows=arg_io.MetricsActiveWindows;
	out.       MetricsActiveAllocations=arg_io.MetricsActiveAllocations;
	out.   MouseDelta= ButiEngine::Vector2(arg_io.MouseDelta.x, arg_io.MouseDelta.y);


	int KeyMods;
	out.MousePosPrev = ButiEngine::Vector2(arg_io.MousePos.x, arg_io.MousePos.y);
	for (int i = 0; i < 5; i++) {
		out.MouseClickedPos[i].x = arg_io.MouseClickedPos[i].x;
		out.MouseClickedPos[i].y = arg_io.MouseClickedPos[i].y;
	}
	memcpy_s(out.MouseClickedTime, 5, arg_io.MouseClickedTime, 5);
	memcpy_s(out.MouseClicked, 5, arg_io.MouseClicked, 5);
	memcpy_s(out.MouseDoubleClicked, 5, arg_io.MouseDoubleClicked, 5);
	memcpy_s(out.MouseReleased, 5, arg_io.MouseReleased, 5);
	memcpy_s(out.MouseDownOwned, 5, arg_io.MouseDownOwned, 5);
	memcpy_s(out.MouseDownWasDoubleClick, 5, arg_io.MouseDownWasDoubleClick, 5);
	memcpy_s(out.MouseDownDuration, 5, arg_io.MouseDownDuration, 5);
	memcpy_s(out.MouseDownDurationPrev, 5, arg_io.MouseDownDurationPrev, 5);
	memcpy_s(out.MouseDragMaxDistanceAbs, 5, arg_io.MouseDragMaxDistanceAbs, 5);
	memcpy_s(out.MouseDragMaxDistanceSqr, 5, arg_io.MouseDragMaxDistanceSqr, 5);

	memcpy_s(out.KeysDownDuration, 512, arg_io.KeysDownDuration, 512);
	memcpy_s(out.KeysDownDurationPrev, 512, arg_io.KeysDownDurationPrev, 512);

	memcpy_s(out.KeysDownDuration, 512, arg_io.KeysDownDuration, 512);
	memcpy_s(out.KeysDownDurationPrev, 512, arg_io.KeysDownDurationPrev, 512);
	memcpy_s(out.NavInputsDownDuration,21, arg_io.NavInputsDownDuration, 21);
	memcpy_s(out.NavInputsDownDurationPrev, 21, arg_io.NavInputsDownDurationPrev, 21);
	out.      PenPressure = arg_io.PenPressure;
	out.InputQueueSurrogate = arg_io.InputQueueSurrogate;

	return out;
}

void ButiEngine::GUI::PushID(const std::string& str_id)
{
	ImGui::PushID(str_id.c_str());
}

void ButiEngine::GUI::PushID(const std::string& str_id_begin, const std::string& str_id_end)
{
	ImGui::PushID(str_id_begin.c_str(), str_id_end.c_str());
}

void ButiEngine::GUI::PushID(const void* ptr_id)
{
	ImGui::PushID(ptr_id);
}

void ButiEngine::GUI::PushID(int int_id)
{
	ImGui::PushID(int_id);
}

void ButiEngine::GUI::PopID()
{
	ImGui::PopID();
}

unsigned int ButiEngine::GUI::GetID(const std::string& str_id)
{
	return ImGui::GetID(str_id.c_str());
}

unsigned int ButiEngine::GUI::GetID(const std::string& str_id_begin, const std::string& str_id_end)
{
	return ImGui::GetID(str_id_begin.c_str(), str_id_end.c_str());
}

unsigned int ButiEngine::GUI::GetID(const void* ptr_id)
{
	return ImGui::GetID(ptr_id);
}

bool ButiEngine::GUI::InputText(const std::string& label, char* buf, size_t buf_size, GuiInputTextFlags flags,  void* user_data)
{if (state != GUIState::active) {return false;}
	return ImGui::InputText(label.c_str(),buf,buf_size,flags, DefaultTextCallBack,user_data);
}

bool ButiEngine::GUI::InputTextMultiline(const std::string& label, char* buf, size_t buf_size, const Vector2& size, GuiInputTextFlags flags, void* user_data )
{if (state != GUIState::active) {return false;}
	return ImGui::InputTextMultiline(label.c_str(), buf, buf_size, ImVec2(size.x, size.y), flags, DefaultTextCallBack, user_data);
}

bool ButiEngine::GUI::InputTextWithHint(const std::string& label, const std::string& hint, char* buf, size_t buf_size, GuiInputTextFlags flags,  void* user_data)
{if (state != GUIState::active) {return false;}
	return ImGui::InputTextWithHint(label.c_str(), hint.c_str(),buf,buf_size, flags, DefaultTextCallBack, user_data);
}

bool ButiEngine::GUI::InputFloat(const std::string& label, float& v, float step, float step_fast, const std::string& format, GuiInputTextFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::InputFloat(label.c_str(), &v, step, step_fast, format.c_str(), flags);
}

bool ButiEngine::GUI::InputFloat2(const std::string& label, Vector2& v, const std::string& format, GuiInputTextFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::InputFloat2(label.c_str(), &v.x,  format.c_str(), flags);
}

bool ButiEngine::GUI::InputFloat3(const std::string& label, Vector3& v, const std::string& format, GuiInputTextFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::InputFloat3(label.c_str(), &v.x, format.c_str(), flags);
}

bool ButiEngine::GUI::InputFloat4(const std::string& label, Vector4& v, const std::string& format, GuiInputTextFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::InputFloat4(label.c_str(), &v.x, format.c_str(), flags);
}

bool ButiEngine::GUI::InputFloat(const std::string& label, float* v, float step, float step_fast, const std::string& format, GuiInputTextFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::InputFloat(label.c_str(), v, step, step_fast, format.c_str(), flags);
}

bool ButiEngine::GUI::InputFloat2(const std::string& label, float* v, const std::string& format, GuiInputTextFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::InputFloat2(label.c_str(), v, format.c_str(), flags);
}

bool ButiEngine::GUI::InputFloat3(const std::string& label, float* v, const std::string& format, GuiInputTextFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::InputFloat3(label.c_str(), v, format.c_str(), flags);
}

bool ButiEngine::GUI::InputFloat4(const std::string& label, float* v, const std::string& format, GuiInputTextFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::InputFloat4(label.c_str(), v, format.c_str(), flags);
}

bool ButiEngine::GUI::InputInt(const std::string& label, int& v, int step, int step_fast, GuiInputTextFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::InputInt(label.c_str(), &v, step, step_fast, flags);
}

bool ButiEngine::GUI::InputInt2(const std::string& label, int v[2], GuiInputTextFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::InputInt2(label.c_str(), v, flags);
}

bool ButiEngine::GUI::InputInt3(const std::string& label, int v[3], GuiInputTextFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::InputInt3(label.c_str(), v, flags);
}

bool ButiEngine::GUI::InputInt4(const std::string& label, int v[4], GuiInputTextFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::InputInt4(label.c_str(), v, flags);
}

bool ButiEngine::GUI::InputDouble(const std::string& label, double& v, double step, double step_fast, const std::string& format, GuiInputTextFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::InputDouble(label.c_str(), &v, step, step_fast, format.c_str(), flags);
}

bool ButiEngine::GUI::DragFloat(const std::string& label, float& v, float v_speed, float v_min, float v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::DragFloat(label.c_str(), &v, v_speed, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::DragFloat2(const std::string& label, Vector2& v, float v_speed, float v_min, float v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::DragFloat2(label.c_str(), &v.x, v_speed, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::DragFloat3(const std::string& label, Vector3& v, float v_speed, float v_min, float v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::DragFloat3(label.c_str(), &v.x, v_speed, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::DragFloat4(const std::string& label, Vector4& v, float v_speed, float v_min, float v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::DragFloat4(label.c_str(), &v.x, v_speed, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::DragFloat(const std::string& label, float* v, float v_speed, float v_min, float v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::DragFloat(label.c_str(), v, v_speed, v_min, v_max, format.c_str(), flags);
}
bool ButiEngine::GUI::DragFloat2(const std::string& label, float* v, float v_speed, float v_min, float v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::DragFloat2(label.c_str(), v, v_speed, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::DragFloat3(const std::string& label, float* v, float v_speed, float v_min, float v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::DragFloat3(label.c_str(), v, v_speed, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::DragFloat4(const std::string& label, float* v, float v_speed, float v_min, float v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return  ImGui::DragFloat4(label.c_str(), v, v_speed, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::DragFloatRange2(const std::string& label, float& v_current_min, float& v_current_max, float v_speed, float v_min, float v_max, const std::string& format, const std::string& format_max, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::DragFloatRange2(label.c_str(), &v_current_min, &v_current_max, v_speed, v_min, v_max, format.c_str(), format_max.c_str(), flags);
}

bool ButiEngine::GUI::DragInt(const std::string& label, int& v, float v_speed, int v_min, int v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::DragInt(label.c_str(), &v, v_speed, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::DragInt(const std::string& label, int* v, float v_speed, int v_min, int v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::DragInt(label.c_str(), v, v_speed, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::DragInt2(const std::string& label, int v[2], float v_speed, int v_min, int v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::DragInt2(label.c_str(), v, v_speed, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::DragInt3(const std::string& label, int v[3], float v_speed, int v_min, int v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::DragInt3(label.c_str(), v, v_speed, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::DragInt4(const std::string& label, int v[4], float v_speed, int v_min, int v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::DragInt4(label.c_str(), v, v_speed, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::DragIntRange2(const std::string& label, int& v_current_min, int& v_current_max, float v_speed, int v_min, int v_max, const std::string& format, const std::string& format_max, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::DragIntRange2(label.c_str(), &v_current_min, &v_current_max, v_speed, v_min, v_max, format.c_str(), format_max.c_str(), flags);
}

bool ButiEngine::GUI::SliderFloat(const std::string& label, float& v, float v_min, float v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::SliderFloat(label.c_str(),&v,v_min,v_max,format.c_str(),flags);
}

bool ButiEngine::GUI::SliderFloat2(const std::string& label, Vector2& v, float v_min, float v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::SliderFloat2(label.c_str(), &v.x, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::SliderFloat3(const std::string& label, Vector3& v, float v_min, float v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::SliderFloat3(label.c_str(), &v.x, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::SliderFloat4(const std::string& label, Vector4& v, float v_min, float v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::SliderFloat4(label.c_str(), &v.x, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::SliderFloat(const std::string& label, float* v, float v_min, float v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::SliderFloat(label.c_str(), v, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::SliderFloat2(const std::string& label, float* v, float v_min, float v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::SliderFloat2(label.c_str(), v, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::SliderFloat3(const std::string& label, float* v, float v_min, float v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::SliderFloat3(label.c_str(), v, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::SliderFloat4(const std::string& label, float* v, float v_min, float v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::SliderFloat4(label.c_str(), v, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::SliderAngle(const std::string& label, float& v_rad, float v_degrees_min, float v_degrees_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::SliderAngle(label.c_str(), &v_rad, v_degrees_min, v_degrees_max, format.c_str(), flags);
}

bool ButiEngine::GUI::SliderInt(const std::string& label, int& v, int v_min, int v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::SliderInt(label.c_str(), &v, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::SliderInt2(const std::string& label, int v[2], int v_min, int v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::SliderInt2(label.c_str(), v, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::SliderInt3(const std::string& label, int v[3], int v_min, int v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::SliderInt2(label.c_str(), v, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::SliderInt4(const std::string& label, int v[4], int v_min, int v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::SliderInt2(label.c_str(), v, v_min, v_max, format.c_str(), flags);
}

bool ButiEngine::GUI::SliderScalar(const std::string& label, int data_type, void* p_data, const void* p_min, const void* p_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::SliderScalar(label.c_str(), data_type, p_data, p_min, p_max, format.c_str(),flags);
}

bool ButiEngine::GUI::SliderScalarN(const std::string& label, int data_type, void* p_data, int components, const void* p_min, const void* p_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::SliderScalarN(label.c_str(), data_type, p_data, components, p_min, p_max, format.c_str(), flags);
}

bool ButiEngine::GUI::VSliderFloat(const std::string& label, const Vector2& size, float& v, float v_min, float v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::VSliderFloat(label.c_str(),ImVec2( size.x,size.y),&v,v_min,v_max,format.c_str(),flags);
}

bool ButiEngine::GUI::VSliderInt(const std::string& label, const Vector2& size, int& v, int v_min, int v_max, const std::string& format, GuiSliderFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::VSliderInt(label.c_str(), ImVec2(size.x, size.y), &v, v_min, v_max, format.c_str(), flags);
}

void ButiEngine::GUI::TextUnformatted(const std::string& text, const std::string& text_end)
{
	ImGui::TextUnformatted(text.c_str(), text_end.c_str());
}

void ButiEngine::GUI::Text(const std::string& fmt, ...)
{
	ImGui::Text(fmt.c_str());
}

void ButiEngine::GUI::Text(const Vector2& v)
{
	GUI::Text("X:"+ std::to_string(v.x)+ ",Y:" + std::to_string(v.y));
}

void ButiEngine::GUI::Text(const Vector3& v)
{
	GUI::Text("X:" + std::to_string(v.x) + ",Y:" + std::to_string(v.y) + ",Z:" + std::to_string(v.z));
}

void ButiEngine::GUI::Text(const Vector4& v)
{
	GUI::Text("X:" + std::to_string(v.x) + ",Y:" + std::to_string(v.y) + ",Z:" + std::to_string(v.z) + ",W:" + std::to_string(v.w));
}

void ButiEngine::GUI::Text(const int v)
{
	GUI::Text(std::to_string(v));
}

void ButiEngine::GUI::Text(const float v)
{
	GUI::Text(std::to_string(v));
}

void ButiEngine::GUI::Text(const double v)
{
	GUI::Text(std::to_string(v));
}

void ButiEngine::GUI::Text(const unsigned int v)
{
	GUI::Text(std::to_string(v));
}

void ButiEngine::GUI::TextV(const std::string& fmt, va_list args)
{
	ImGui::TextV(fmt.c_str(),args);
}

void ButiEngine::GUI::TextColored(const Vector4& col, const std::string& fmt, ...)
{
	ImGui::TextColored(ImVec4(col.x, col.y, col.z, col.w), fmt.c_str());
}

void ButiEngine::GUI::TextColoredV(const Vector4& col, const std::string& fmt, va_list args)
{
	ImGui::TextColoredV(ImVec4(col.x, col.y, col.z, col.w), fmt.c_str(),args);
}

void ButiEngine::GUI::TextDisabled(const std::string& fmt, ...)
{
	ImGui::TextDisabled(fmt.c_str());
}

void ButiEngine::GUI::TextDisabledV(const std::string& fmt, va_list args)
{
	ImGui::TextDisabledV(fmt.c_str(),args);
}

void ButiEngine::GUI::TextWrapped(const std::string& fmt, ...)
{
	ImGui::TextWrapped(fmt.c_str());
}

void ButiEngine::GUI::TextWrappedV(const std::string& fmt, va_list args)
{
	ImGui::TextWrappedV(fmt.c_str(),args);
}

void ButiEngine::GUI::LabelText(const std::string& label, const std::string& fmt, ...)
{
	ImGui::LabelText(label.c_str(), fmt.c_str());
}

void ButiEngine::GUI::LabelTextV(const std::string& label, const std::string& fmt, va_list args)
{
	ImGui::LabelTextV(label.c_str(), fmt.c_str(),args);
}

void ButiEngine::GUI::BulletText(const std::string& fmt, ...)
{
	ImGui::BulletText(fmt.c_str());
}

void ButiEngine::GUI::BulletTextV(const std::string& fmt, va_list args)
{
	ImGui::BulletTextV(fmt.c_str(),args);
}

bool ButiEngine::GUI::Button(const std::string& label, const Vector2& size)
{
	return ImGui::Button(label.c_str(), ImVec2(size.x, size.y));
}

bool ButiEngine::GUI::SmallButton(const std::string& label)
{if (state != GUIState::active) {return false;}
	return ImGui::SmallButton(label.c_str());
}

bool ButiEngine::GUI::InvisibleButton(const std::string& str_id, const Vector2& size, GuiButtonFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::InvisibleButton(str_id.c_str(), ImVec2(size.x, size.y),flags);
}

bool ButiEngine::GUI::ArrowButton(const std::string& str_id, GuiDir dir)
{if (state != GUIState::active) {return false;}
	return ImGui::ArrowButton(str_id.c_str(), dir);
}



bool ButiEngine::GUI::Checkbox(const std::string& label, bool* v)
{if (state != GUIState::active) {return false;}
	return ImGui::Checkbox(label.c_str(), v);
}

bool ButiEngine::GUI::CheckboxFlags(const std::string& label, unsigned int& flags, unsigned int flags_value)
{if (state != GUIState::active) {return false;}
	return ImGui::CheckboxFlags(label.c_str(), &flags,flags_value);
}

bool ButiEngine::GUI::RadioButton(const std::string& label, bool active)
{if (state != GUIState::active) {return false;}
	return ImGui::RadioButton(label.c_str(), active);
}

bool ButiEngine::GUI::RadioButton(const std::string& label, int& v, int v_button)
{if (state != GUIState::active) {return false;}
	return ImGui::RadioButton(label.c_str(),& v,v_button);
}

void ButiEngine::GUI::ProgressBar(float fraction, const Vector2& size_arg, const std::string& overlay)
{
	ImVec2 ret(size_arg.x, size_arg.y);
	ImGui::ProgressBar(fraction, ret, overlay.c_str());
}

void ButiEngine::GUI::Bullet()
{
	ImGui::Bullet();
}

bool ButiEngine::GUI::BeginCombo(const std::string& label, const std::string& preview_value, GuiComboFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::BeginCombo(label.c_str(), preview_value.c_str(), flags);
}

void ButiEngine::GUI::EndCombo()
{
	ImGui::EndCombo();
}

bool ButiEngine::GUI::Combo(const std::string& label, int& current_item, const std::vector<std::string>& arg_item, int items_count, int popup_max_height_in_items)
{if (state != GUIState::active) {return false;}
	char** l_items= (char**) malloc(sizeof(char*)*items_count);

	for (int i = 0; i < arg_item.size(); i++) {if (state != GUIState::active) {return false;}
		l_items[i] = (char*)malloc(sizeof(char) * arg_item[i].size());

		memcpy_s(l_items[i], arg_item[i].size(), arg_item[i].c_str(), arg_item[i].size());

	}
	bool ret= ImGui::Combo(label.c_str(), &current_item,l_items , items_count, popup_max_height_in_items);


	for (int i = 0; i < arg_item.size(); i++) {if (state != GUIState::active) {return false;}
		free(l_items[i]);

	}

	free(l_items);

	return ret;
}

bool ButiEngine::GUI::Combo(const std::string& label, int& current_item, const std::string& items_separated_by_zeros, int popup_max_height_in_items)
{if (state != GUIState::active) {return false;}
	return ImGui::Combo(label.c_str(), &current_item,  items_separated_by_zeros.c_str(), popup_max_height_in_items);
}

bool ButiEngine::GUI::Combo(const std::string& label, int& current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items)
{if (state != GUIState::active) {return false;}
	return ImGui::Combo(label.c_str(), &current_item, items_getter, data, items_count, popup_max_height_in_items);
}


bool ButiEngine::GUI::TreeNode(const std::string& label)
{if (state != GUIState::active) {return false;}
	return ImGui::TreeNode(label.c_str());
}

bool ButiEngine::GUI::TreeNode(const std::string& str_id, const std::string& fmt, ...)
{if (state != GUIState::active) {return false;}
	return ImGui::TreeNode(str_id.c_str(), fmt.c_str());
}

bool ButiEngine::GUI::TreeNode(const void* ptr_id, const std::string& fmt, ...)
{if (state != GUIState::active) {return false;}
	return ImGui::TreeNode(ptr_id, fmt.c_str());
}

bool ButiEngine::GUI::TreeNodeV(const std::string& str_id, const std::string& fmt, va_list args)
{if (state != GUIState::active) {return false;}
	return ImGui::TreeNodeV(str_id.c_str(), fmt.c_str(), args);
}

bool ButiEngine::GUI::TreeNodeV(const void* ptr_id, const std::string& fmt, va_list args)
{if (state != GUIState::active) {return false;}
	return ImGui::TreeNodeV(ptr_id, fmt.c_str(), args);
}

bool ButiEngine::GUI::TreeNodeEx(const std::string& label, GuiTreeNodeFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::TreeNodeEx(label.c_str(), flags);
}

bool ButiEngine::GUI::TreeNodeEx(const std::string& str_id, GuiTreeNodeFlags flags, const std::string& fmt, ...)
{if (state != GUIState::active) {return false;}
	return ImGui::TreeNodeEx(str_id.c_str(), flags,fmt.c_str());
}

bool ButiEngine::GUI::TreeNodeEx(const void* ptr_id, GuiTreeNodeFlags flags, const std::string& fmt, ...)
{if (state != GUIState::active) {return false;}
	return ImGui::TreeNodeEx(ptr_id, flags, fmt.c_str());
}

bool ButiEngine::GUI::TreeNodeExV(const std::string& str_id, GuiTreeNodeFlags flags, const std::string& fmt, va_list args)
{if (state != GUIState::active) {return false;}
	return ImGui::TreeNodeEx(str_id.c_str(), flags, fmt.c_str(),args);
}

bool ButiEngine::GUI::TreeNodeExV(const void* ptr_id, GuiTreeNodeFlags flags, const std::string& fmt, va_list args)
{if (state != GUIState::active) {return false;}
	return ImGui::TreeNodeEx(ptr_id, flags, fmt.c_str(), args);
}

void ButiEngine::GUI::TreePush(const std::string& str_id)
{
	ImGui::TreePush(str_id.c_str());
}

void ButiEngine::GUI::TreePush(const void* ptr_id)
{
	ImGui::TreePush(ptr_id);
}

void ButiEngine::GUI::TreePop()
{
	ImGui::TreePop();
}

float ButiEngine::GUI::GetTreeNodeToLabelSpacing()
{
	return ImGui::GetTreeNodeToLabelSpacing();
}

bool ButiEngine::GUI::CollapsingHeader(const std::string& label, GuiTreeNodeFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::CollapsingHeader(label.c_str(), flags);
}

bool ButiEngine::GUI::CollapsingHeader(const std::string& label, bool* p_open, GuiTreeNodeFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::CollapsingHeader(label.c_str(), p_open, flags);
}

void ButiEngine::GUI::SetNextItemOpen(bool is_open, int cond)
{
	ImGui::SetNextItemOpen(is_open, cond);
}

bool ButiEngine::GUI::Selectable(const std::string& label, bool selected, GuiSelectableFlags flags, const Vector2& size)
{if (state != GUIState::active) {return false;}
	return ImGui::Selectable(label.c_str(),selected,flags,ImVec2(size.x,size.y));
}

bool ButiEngine::GUI::Selectable(const std::string& label, bool* p_selected, GuiSelectableFlags flags, const Vector2& size)
{if (state != GUIState::active) {return false;}
	return ImGui::Selectable(label.c_str(), p_selected, flags, ImVec2(size.x, size.y));
}

bool ButiEngine::GUI::ListBox(const std::string& label, int& current_item, const std::vector<std::string>& arg_item, int height_in_items)
{
	if (state != GUIState::active) { return false; }
	char** list = (char**)malloc(sizeof(char*) * arg_item.size());

	for (int i = 0; i < arg_item.size();i++) {
		list[i] =(char*) malloc(sizeof(char) * arg_item[i].size());

		memcpy_s(list[i], arg_item[i].size(), arg_item[i].c_str(), arg_item[i].size());

	}

	return ImGui::ListBox(label.c_str(), &current_item,list, arg_item.size(), height_in_items);

	for (int i = 0; i < arg_item.size(); i++) {
		free(list[i]);

	}

	free(list);
}

bool ButiEngine::GUI::ListBox(const std::string& label, int& current_item, char** arg_item, int items_count, int height_in_items)
{if (state != GUIState::active) {return false;}
	return ImGui::ListBox(label.c_str(), &current_item,arg_item , items_count, height_in_items);
}

bool ButiEngine::GUI::ListBox(const std::string& label, int& current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items)
{if (state != GUIState::active) {return false;}
	return ImGui::ListBox(label.c_str(),&current_item,items_getter,data,items_count,height_in_items);
}

bool ButiEngine::GUI::ListBoxHeader(const std::string& label, const Vector2& size)
{if (state != GUIState::active) {return false;}
	return ImGui::ListBoxHeader(label.c_str(), ImVec2(size.x, size.y));
}

bool ButiEngine::GUI::ListBoxHeader(const std::string& label, int items_count, int height_in_items)
{if (state != GUIState::active) {return false;}
	return ImGui::ListBoxHeader(label.c_str(), items_count, height_in_items);
}

void ButiEngine::GUI::ListBoxFooter()
{
	ImGui::ListBoxFooter();
}

void ButiEngine::GUI::PlotLines(const std::string& label, const float& values, int values_count, int values_offset, const std::string& overlay_text, float scale_min, float scale_max, Vector2 graph_size, int stride)
{
	ImGui::PlotLines(label.c_str(), &values, values_count, values_offset, overlay_text.c_str(), scale_min, scale_max,ImVec2(graph_size.x,graph_size.y),stride);
}

void ButiEngine::GUI::PlotLines(const std::string& label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const std::string& overlay_text, float scale_min, float scale_max, Vector2 graph_size)
{
	ImGui::PlotLines(label.c_str(),values_getter,data,values_count,values_offset,overlay_text.c_str(),scale_min,scale_max,ImVec2(graph_size.x,graph_size.y));
}

void ButiEngine::GUI::PlotHistogram(const std::string& label, const float& values, int values_count, int values_offset, const std::string& overlay_text, float scale_min, float scale_max, Vector2 graph_size, int stride)
{
	ImGui::PlotHistogram(label.c_str(), &values, values_count, values_offset, overlay_text.c_str(), scale_min, scale_max, ImVec2(graph_size.x, graph_size.y));
}

void ButiEngine::GUI::PlotHistogram(const std::string& label, float(*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const std::string& overlay_text, float scale_min, float scale_max, Vector2 graph_size)
{
	ImGui::PlotHistogram(label.c_str(),values_getter,data,values_count,values_offset,overlay_text.c_str(),scale_min,scale_max,ImVec2(graph_size.x,graph_size.y));
}

void ButiEngine::GUI::Value(const std::string& prefix, bool v)
{
	ImGui::Value(prefix.c_str(), v);
}

void ButiEngine::GUI::Value(const std::string& prefix, int v)
{
	ImGui::Value(prefix.c_str(), v);
}

void ButiEngine::GUI::Value(const std::string& prefix, unsigned int v)
{
	ImGui::Value(prefix.c_str(), v);
}

void ButiEngine::GUI::Value(const std::string& prefix, float v, const std::string& float_format)
{
	ImGui::Value(prefix.c_str(), v);
}

void ButiEngine::GUI::EndMenuBar()
{
	ImGui::EndMenuBar();
}

bool ButiEngine::GUI::BeginMainMenuBar()
{
	return ImGui::BeginMainMenuBar();
}

void ButiEngine::GUI::EndMainMenuBar()
{
	ImGui::EndMainMenuBar();
}

bool ButiEngine::GUI::BeginMenu(const std::string& label, bool enabled)
{
	return ImGui::BeginMenu(label.c_str(), enabled);
}

void ButiEngine::GUI::EndMenu()
{
	ImGui::EndMenu();
}

bool ButiEngine::GUI::MenuItem(const std::string& label, const std::string& shortcut, bool selected, bool enabled)
{if (state != GUIState::active) {return false;}
	return ImGui::MenuItem(label.c_str(), shortcut.c_str(), selected, enabled);
}

bool ButiEngine::GUI::MenuItem(const std::string& label, const std::string& shortcut, bool* p_selected, bool enabled)
{if (state != GUIState::active) {return false;}
	return ImGui::MenuItem(label.c_str(), shortcut.c_str(), p_selected, enabled);
}

void ButiEngine::GUI::BeginTooltip()
{
	ImGui::BeginTooltip();
}

void ButiEngine::GUI::EndTooltip()
{
	ImGui::EndTooltip();
}

void ButiEngine::GUI::SetTooltip(const std::string& fmt, ...)
{
	ImGui::SetTooltip(fmt.c_str());
}

void ButiEngine::GUI::SetTooltipV(const std::string& fmt, va_list args)
{
	ImGui::SetTooltipV(fmt.c_str(), args);
}

bool ButiEngine::GUI::BeginPopup(const std::string& str_id, GuiPopupFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::BeginPopup(str_id.c_str(), flags);
}

bool ButiEngine::GUI::BeginPopupModal(const std::string& name, bool* p_open, GuiPopupFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::BeginPopupModal(name.c_str(), p_open, flags);
}
void ButiEngine::GUI::EndPopup()
{
	ImGui::EndPopup();
}

void ButiEngine::GUI::OpenPopup(const std::string& str_id, GuiPopupFlags popup_flags)
{
	ImGui::OpenPopup(str_id.c_str(), popup_flags);
}

void ButiEngine::GUI::OpenPopupOnItemClick(const std::string& str_id, GuiPopupFlags popup_flags)
{
	ImGui::OpenPopupOnItemClick(str_id.c_str(), popup_flags);
}

void ButiEngine::GUI::CloseCurrentPopup()
{
	ImGui::CloseCurrentPopup();
}

bool ButiEngine::GUI::BeginPopupContextItem(const std::string& str_id, GuiPopupFlags popup_flags)
{if (state != GUIState::active) {return false;}
	return ImGui::BeginPopupContextItem(str_id.c_str(), popup_flags);
}

bool ButiEngine::GUI::BeginPopupContextWindow(const std::string& str_id, GuiPopupFlags popup_flags)
{if (state != GUIState::active) {return false;}
	return ImGui::BeginPopupContextWindow(str_id.c_str(), popup_flags);
}

bool ButiEngine::GUI::BeginPopupContextVoid(const std::string& str_id, GuiPopupFlags popup_flags)
{if (state != GUIState::active) {return false;}
	return ImGui::BeginPopupContextVoid(str_id.c_str(), popup_flags);
}

bool ButiEngine::GUI::IsPopupOpen(const std::string& str_id, GuiPopupFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::IsPopupOpen(str_id.c_str(), flags);
}

void ButiEngine::GUI::Columns(int count, const std::string& id, bool border)
{
	ImGui::Columns(count, id.c_str(), border);
}

void ButiEngine::GUI::NextColumn()
{
	ImGui::NextColumn();
}

int ButiEngine::GUI::GetColumnIndex()
{
	return ImGui::GetColumnIndex();
}

float ButiEngine::GUI::GetColumnWidth(int column_index)
{
	return ImGui::GetColumnWidth(column_index);
}

void ButiEngine::GUI::SetColumnWidth(int column_index, float width)
{
	return ImGui::SetColumnWidth(column_index,width);
}

float ButiEngine::GUI::GetColumnOffset(int column_index)
{
	return ImGui::GetColumnOffset(column_index);
}

void ButiEngine::GUI::SetColumnOffset(int column_index, float offset_x)
{
	ImGui::SetColumnOffset(column_index, offset_x);
}

int ButiEngine::GUI::GetColumnsCount()
{
	return ImGui::GetColumnsCount();
}

bool ButiEngine::GUI::BeginTabBar(const std::string& str_id, GuiTabBarFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::BeginTabBar(str_id.c_str(), flags);
}

void ButiEngine::GUI::EndTabBar()
{
	ImGui::EndTabBar();
}

bool ButiEngine::GUI::BeginTabItem(const std::string& label, bool* p_open, GuiTabBarFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::BeginTabItem(label.c_str(), p_open, flags);
}

void ButiEngine::GUI::EndTabItem()
{
	ImGui::EndTabItem();
}

bool ButiEngine::GUI::TabItemButton(const std::string& label, GuiTabBarFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::TabItemButton(label.c_str(), flags);
}

void ButiEngine::GUI::SetTabItemClosed(const std::string& tab_or_docked_window_label)
{
	ImGui::SetTabItemClosed(tab_or_docked_window_label.c_str());
}

void ButiEngine::GUI::LogToTTY(int auto_open_depth)
{
	ImGui::LogToTTY(auto_open_depth);
}

void ButiEngine::GUI::LogToFile(int auto_open_depth, const std::string& filename)
{
	ImGui::LogToFile(auto_open_depth, filename.c_str());
}

void ButiEngine::GUI::LogToClipboard(int auto_open_depth)
{
	ImGui::LogToClipboard(auto_open_depth);
}

void ButiEngine::GUI::LogFinish()
{
	ImGui::LogFinish();
}

void ButiEngine::GUI::LogButtons()
{
	ImGui::LogButtons();
}

void ButiEngine::GUI::LogText(const std::string& fmt, ...)
{
	ImGui::LogText(fmt.c_str());
}

void ButiEngine::GUI::PushClipRect(const Vector2& clip_rect_min, const Vector2& clip_rect_max, bool intersect_with_current_clip_rect)
{
	ImGui::PushClipRect(ImVec2(clip_rect_min.x, clip_rect_min.y), ImVec2(clip_rect_max.x, clip_rect_max.y), intersect_with_current_clip_rect);
}

void ButiEngine::GUI::PopClipRect()
{
	ImGui::PopClipRect();
}

void ButiEngine::GUI::SetItemDefaultFocus()
{
	ImGui::SetItemDefaultFocus();
}

void ButiEngine::GUI::SetKeyboardFocusHere(int offset)
{
	ImGui::SetKeyboardFocusHere(offset);
}

bool ButiEngine::GUI::IsItemHovered(GuiHoveredFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::IsItemHovered(flags);
}

bool ButiEngine::GUI::IsItemActive()
{if (state != GUIState::active) {return false;}
	return ImGui::IsItemActive();
}

bool ButiEngine::GUI::IsItemFocused()
{if (state != GUIState::active) {return false;}
	return ImGui::IsItemFocused();
}

bool ButiEngine::GUI::IsItemClicked(int mouse_button)
{if (state != GUIState::active) {return false;}
	return ImGui::IsItemClicked(mouse_button);
}

bool ButiEngine::GUI::IsItemVisible()
{if (state != GUIState::active) {return false;}
	return ImGui::IsItemVisible();
}

bool ButiEngine::GUI::IsItemEdited()
{if (state != GUIState::active) {return false;}
	return ImGui::IsItemEdited();
}

bool ButiEngine::GUI::IsItemActivated()
{if (state != GUIState::active) {return false;}
	return ImGui::IsItemActivated();
}

bool ButiEngine::GUI::IsItemDeactivated()
{if (state != GUIState::active) {return false;}
	return ImGui::IsItemDeactivated();
}

bool ButiEngine::GUI::IsItemDeactivatedAfterEdit()
{if (state != GUIState::active) {return false;}
	return ImGui::IsItemDeactivatedAfterEdit();
}

bool ButiEngine::GUI::IsItemToggledOpen()
{if (state != GUIState::active) {return false;}
	return ImGui::IsItemToggledOpen();
}

bool ButiEngine::GUI::IsAnyItemHovered()
{if (state != GUIState::active) {return false;}
	return ImGui::IsAnyItemHovered();
}

bool ButiEngine::GUI::IsAnyItemActive()
{if (state != GUIState::active) {return false;}
	return ImGui::IsAnyItemActive();
}

bool ButiEngine::GUI::IsAnyItemFocused()
{if (state != GUIState::active) {return false;}
	return ImGui::IsAnyItemFocused();
}

ButiEngine::Vector2 ButiEngine::GUI::GetItemRectMin()
{
	auto temp = ImGui::GetItemRectMin();
	return Vector2(temp.x,temp.y);
}

ButiEngine::Vector2 ButiEngine::GUI::GetItemRectMax()
{
	auto temp = ImGui::GetItemRectMax();
	return Vector2(temp.x, temp.y);
}

ButiEngine::Vector2 ButiEngine::GUI::GetItemRectSize()
{
	auto temp = ImGui::GetItemRectSize();
	return Vector2(temp.x, temp.y);
}

void ButiEngine::GUI::SetItemAllowOverlap()
{
	ImGui::SetItemAllowOverlap();
}

bool ButiEngine::GUI::IsRectVisible(const Vector2& size)
{if (state != GUIState::active) {return false;}
	return ImGui::IsRectVisible(ImVec2(size.x, size.y));
}

bool ButiEngine::GUI::IsRectVisible(const Vector2& rect_min, const Vector2& rect_max)
{if (state != GUIState::active) {return false;}
	return ImGui::IsRectVisible(ImVec2(rect_min.x, rect_min.y), ImVec2(rect_max.x, rect_max.y));
}

double ButiEngine::GUI::GetTime()
{
	return ImGui::GetTime();
}

int ButiEngine::GUI::GetFrameCount()
{
	return ImGui::GetFrameCount();
}

std::string ButiEngine::GUI::GetStyleColorName(GuiCol idx)
{
	return ImGui::GetStyleColorName(idx);
}


void ButiEngine::GUI::CalcListClipping(int items_count, float items_height, int& out_items_display_start, int& out_items_display_end)
{
	ImGui::CalcListClipping(items_count, items_height, &out_items_display_start, &out_items_display_end);
}

bool ButiEngine::GUI::BeginChildFrame(unsigned int id, const Vector2& size, GuiWindowFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::BeginChildFrame(id, ImVec2(size.x, size.y), flags);
}

void ButiEngine::GUI::EndChildFrame()
{
	ImGui::EndChildFrame();
}

ButiEngine::Vector2 ButiEngine::GUI::CalcTextSize(const std::string& text, const std::string& text_end, bool hide_text_after_double_hash, float wrap_width)
{
	auto temp = ImGui::CalcTextSize(text.c_str(), text_end.c_str(), hide_text_after_double_hash, wrap_width);
	return Vector2(temp.x,temp.y);
}

ButiEngine::Vector4 ButiEngine::GUI::ColorConvertU32ToFloat4(unsigned int in)
{
	auto temp = ImGui::ColorConvertU32ToFloat4(in);
	return Vector4(temp.x,temp.y,temp.z,temp.w);
}

unsigned int ButiEngine::GUI::ColorConvertFloat4ToU32(const Vector4& in)
{
	return ImGui::ColorConvertFloat4ToU32(ImVec4(in.x, in.y, in.z, in.w));
}

void ButiEngine::GUI::ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v)
{
	ImGui::ColorConvertRGBtoHSV(r, g, b, out_h, out_s, out_v);
}

void ButiEngine::GUI::ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
	ImGui::ColorConvertHSVtoRGB(h, s, v, out_r, out_g, out_b);
}


void ButiEngine::GUI::CaptureKeyboardFromApp(bool want_capture_keyboard_value)
{
	ImGui::CaptureKeyboardFromApp(want_capture_keyboard_value);
}

void ButiEngine::GUI::CaptureMouseFromApp(bool want_capture_mouse_value)
{
	ImGui::CaptureMouseFromApp(want_capture_mouse_value);
}

std::string ButiEngine::GUI::GetClipboardText()
{
	return ImGui::GetClipboardText();
}

void ButiEngine::GUI::SetClipboardText(const std::string& text)
{
	ImGui::SetClipboardText(text.c_str());
}

void ButiEngine::GUI::LoadIniSettingsFromDisk(const std::string& ini_filename)
{
	ImGui::LoadIniSettingsFromDisk(ini_filename.c_str());
}

void ButiEngine::GUI::LoadIniSettingsFromMemory(const std::string& ini_data, size_t ini_size)
{
	ImGui::LoadIniSettingsFromMemory(ini_data.c_str(), ini_size);
}

void ButiEngine::GUI::SaveIniSettingsToDisk(const std::string& ini_filename)
{
	ImGui::SaveIniSettingsToDisk(ini_filename.c_str());
}

std::string ButiEngine::GUI::SaveIniSettingsToMemory(size_t* out_ini_size)
{
	return ImGui::SaveIniSettingsToMemory(out_ini_size);
}

void ButiEngine::GUI::SetAllocatorFunctions(void* (*alloc_func)(size_t sz, void* user_data), void(*free_func)(void* ptr, void* user_data), void* user_data)
{
	ImGui::SetAllocatorFunctions(alloc_func, free_func,user_data);
}

void* ButiEngine::GUI::MemAlloc(size_t size)
{
	return ImGui::MemAlloc(size);
}

void ButiEngine::GUI::MemFree(void* ptr)
{
	ImGui::MemFree(ptr);
}

unsigned int ButiEngine::GUI::GetColorU32(GuiCol idx, float alpha_mul)
{
	return ImGui::GetColorU32(idx, alpha_mul);
}

unsigned int ButiEngine::GUI::GetColorU32(const Vector4& col)
{
	return ImGui::GetColorU32(ImVec4(col.x,col.y,col.z,col.w));
}

unsigned int ButiEngine::GUI::GetColorU32(unsigned int col)
{
	return ImGui::GetColorU32(col);
}

bool ButiEngine::GUI::ColorEdit3(const std::string& label, Vector3& col, GuiColorEditFlags flags)
{
	return ImGui::ColorEdit3(label.c_str(), &col.x, flags);
}

bool ButiEngine::GUI::ColorEdit4(const std::string& label, Vector4& col, GuiColorEditFlags flags)
{
	return ImGui::ColorEdit4(label.c_str(), &col.x, flags);
}

bool ButiEngine::GUI::ColorPicker3(const std::string& label, Vector3 &col, GuiColorEditFlags flags)
{
	return ImGui::ColorPicker3(label.c_str(), &col.x, flags);
}

bool ButiEngine::GUI::ColorPicker4(const std::string& label, Vector4& col, GuiColorEditFlags flags, const float* ref_col)
{
	return ImGui::ColorPicker4(label.c_str(), &col.x, flags);
}

bool ButiEngine::GUI::ColorButton(const std::string& desc_id, const Vector4& col, GuiColorEditFlags flags, Vector2 size)
{
	return ImGui::ColorButton(desc_id.c_str(), ImVec4(col.x, col.y, col.z, col.w), flags, ImVec2(size.x, size.y));
}

void ButiEngine::GUI::SetColorEditOptions(GuiColorEditFlags flags)
{
	ImGui::SetColorEditOptions(flags);
}

void ButiEngine::GUI::Line(const Vector2& p1, const Vector2& p2, unsigned int col, float thickness)
{
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(p1.x, p1.y), ImVec2(p2.x, p2.y), col, thickness);
}

float ButiEngine::GUI::GetFontSize()
{
	return ImGui::GetFontSize();
}



ButiEngine::GUI::GuiIO ButiEngine::GUI::GetIO()
{
	return ConvertIO(ImGui::GetIO());
}

void ButiEngine::GUI::SetState(GUIState arg_state)
{
	state = arg_state;
}

bool ButiEngine::GUI::Begin(const std::string& name, bool* p_open, GuiWindowFlags flags)
{
	if (state == GUIState::noActive) {
		state = GUIState::active;
		//ImGui::DockSpace()
		return ImGui::Begin(name.c_str(), p_open, flags);
	}
	return false;
}

void ButiEngine::GUI::End()
{
	if (state == GUIState::active) {
		state = GUIState::noActive;

		ImGui::End();
	}
}

void ButiEngine::GUI::PushStyleColor(int idx, UINT col)
{
	ImGui::PushStyleColor(idx, col);
}

void ButiEngine::GUI::PushStyleColor(int idx, const Vector4& col)
{
	ImGui::PushStyleColor(idx, ImVec4(col.x, col.y, col.z, col.w));
}

void ButiEngine::GUI::PopStyleColor(int count)
{
	ImGui::PopStyleColor(count);
}

void ButiEngine::GUI::PushStyleVar(int idx, float val)
{
	ImGui::PushStyleVar(idx,val);
}

void ButiEngine::GUI::PushStyleVar(int idx, const Vector2& val)
{
	ImGui::PushStyleVar(idx, ImVec2(val.x, val.y));
}

void ButiEngine::GUI::PopStyleVar(int count)
{
	ImGui::PopStyleVar(count);
}

void ButiEngine::GUI::SetNextWindowBgAlpha(float alpha)
{
	ImGui::SetNextWindowBgAlpha(alpha);
}

bool ButiEngine::GUI::BeginChild(const std::string& str_id, const Vector2& size, bool border, GuiWindowFlags flags)
{
	if (state != GUIState::active) {return false;}

	return ImGui::BeginChild(str_id.c_str(), ImVec2(size.x, size.y), border, flags);
}

bool ButiEngine::GUI::BeginChild(unsigned int id, const Vector2& size, bool border, GuiWindowFlags flags)
{if (state != GUIState::active) {return false;}
	return ImGui::BeginChild(id, ImVec2(size.x, size.y), border, flags);
}

void ButiEngine::GUI::EndChild()
{if (state != GUIState::active) {return ;}
	ImGui::EndChild();
}

bool ButiEngine::GUI::IsWindowAppearing()
{if (state != GUIState::active) {return false;}
	return ImGui::IsWindowAppearing();
}

bool ButiEngine::GUI::IsWindowCollapsed()
{
	return ImGui::IsWindowCollapsed();
}

bool ButiEngine::GUI::IsWindowFocused(GuiFocusedFlags flags)
{
	return ImGui::IsWindowFocused(flags);
}

bool ButiEngine::GUI::IsWindowHovered(GuiHoveredFlags flags)
{
	return ImGui::IsWindowHovered(flags);
}


ButiEngine::Vector2 ButiEngine::GUI::GetWindowPos()
{if (state != GUIState::active) {return Vector2();}
	auto temp = ImGui::GetWindowPos();
	return Vector2(temp.x,temp.y);
}

ButiEngine::Vector2 ButiEngine::GUI::GetWindowSize()
{
	if (state != GUIState::active) { return Vector2(); }
	auto temp = ImGui::GetWindowSize();
	return Vector2(temp.x, temp.y);
}

float ButiEngine::GUI::GetWindowWidth()
{if (state != GUIState::active) {return false;}
	return ImGui::GetWindowWidth();
}

float ButiEngine::GUI::GetWindowHeight()
{if (state != GUIState::active) {return false;}
	return ImGui::GetWindowHeight();
}

ButiEngine::Vector2 ButiEngine::GUI::GetContentRegionMax()
{
	auto temp = ImGui::GetContentRegionMax();
	return Vector2(temp.x, temp.y);
}

ButiEngine::Vector2 ButiEngine::GUI::GetContentRegionAvail()
{
	auto temp = ImGui::GetContentRegionAvail();
	return Vector2(temp.x, temp.y);
}

ButiEngine::Vector2 ButiEngine::GUI::GetWindowContentRegionMin()
{
	auto temp = ImGui::GetWindowContentRegionMin();
	return Vector2(temp.x, temp.y);
}

ButiEngine::Vector2 ButiEngine::GUI::GetWindowContentRegionMax()
{
	auto temp = ImGui::GetWindowContentRegionMax();
	return Vector2(temp.x, temp.y);
}

float ButiEngine::GUI::GetWindowContentRegionWidth()
{
	return ImGui::GetWindowContentRegionWidth();
}

void ButiEngine::GUI::PushItemWidth(float item_width)
{
	ImGui::PushItemWidth(item_width);
}

void ButiEngine::GUI::PopItemWidth()
{
	ImGui::PopItemWidth();
}

void ButiEngine::GUI::SetNextItemWidth(float item_width)
{
	ImGui::SetNextItemWidth(item_width);
}

float ButiEngine::GUI::CalcItemWidth()
{
	return ImGui::CalcItemWidth();
}

void ButiEngine::GUI::PushTextWrapPos(float wrap_local_pos_x)
{
	ImGui::PushTextWrapPos(wrap_local_pos_x);
}

void ButiEngine::GUI::PopTextWrapPos()
{
	ImGui::PopTextWrapPos();
}

void ButiEngine::GUI::PushAllowKeyboardFocus(bool allow_keyboard_focus)
{
	ImGui::PushAllowKeyboardFocus(allow_keyboard_focus);
}

void ButiEngine::GUI::PopAllowKeyboardFocus()
{
	ImGui::PopAllowKeyboardFocus();
}

void ButiEngine::GUI::PushButtonRepeat(bool repeat)
{
	ImGui::PushButtonRepeat(repeat);
}

void ButiEngine::GUI::PopButtonRepeat()
{
	ImGui::PopButtonRepeat();
}

void ButiEngine::GUI::Separator()
{if (state != GUIState::active) {return ;}
	ImGui::Separator();
}

void ButiEngine::GUI::SameLine(float offset_from_start_x, float spacing)
{if (state != GUIState::active) {return ;}
	ImGui::SameLine(offset_from_start_x, spacing);
}

void ButiEngine::GUI::NewLine()
{if (state != GUIState::active) {return ;}
	ImGui::NewLine();
}

void ButiEngine::GUI::Spacing()
{if (state != GUIState::active) {return ;}
	ImGui::Spacing();
}

void ButiEngine::GUI::Dummy(const Vector2& size)
{if (state != GUIState::active) {return ;}
	ImGui::Dummy(ImVec2(size.x, size.y));
}

void ButiEngine::GUI::Indent(float indent_w)
{if (state != GUIState::active) {return ;}
	ImGui::Indent(indent_w);
}

void ButiEngine::GUI::Unindent(float indent_w)
{if (state != GUIState::active) {return ;}
	ImGui::Unindent(indent_w);
}

void ButiEngine::GUI::BeginGroup()
{if (state != GUIState::active) {return ;}
	ImGui::BeginGroup();
}

void ButiEngine::GUI::EndGroup()
{if (state != GUIState::active) {return ;}
	ImGui::EndGroup();
}

void ButiEngine::GUI::AlignTextToFramePadding()
{if (state != GUIState::active) {return ;}
	ImGui::AlignTextToFramePadding();
}

float ButiEngine::GUI::GetTextLineHeight()
{if (state != GUIState::active) {return false;}
	return ImGui::GetTextLineHeight();
}

float ButiEngine::GUI::GetTextLineHeightWithSpacing()
{if (state != GUIState::active) {return false;}
	return ImGui::GetTextLineHeightWithSpacing();
}

float ButiEngine::GUI::GetFrameHeight()
{if (state != GUIState::active) {return false;}
	return ImGui::GetFrameHeight();
}

float ButiEngine::GUI::GetFrameHeightWithSpacing()
{if (state != GUIState::active) {return false;}
	return ImGui::GetFrameHeightWithSpacing();
}
