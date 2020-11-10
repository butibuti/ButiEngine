#include "stdafx.h"


void ButiEngine::ICBuffer::ShowUI()
{
	if (ImGui::TreeNode((exName+"##"+std::to_string(slot)).c_str())) {

		ImGui::InputTextWithHint("Name", exName.c_str(), CallBacks::cbufferName, 64, 64, CallBacks::ImguiCallBack);
		ImGui::SameLine();

		if (ImGui::Button("Change")) {
			exName = (CallBacks::cbufferName);
		}
		if (OnShowUI()) {
			Update();
		}

		ImGui::TreePop();
	}
}