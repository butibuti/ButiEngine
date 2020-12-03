#include "stdafx.h"

void ButiEngine::ICBuffer::ShowUI()
{
	if (GUI::TreeNode((exName+"##"+std::to_string(slot)).c_str())) {

		GUI::InputTextWithHint("Name", exName.c_str(), GUI::cbufferName, 64, 64);
		GUI::SameLine();

		if (GUI::Button("Change")) {
			exName = (GUI::cbufferName);
		}
		if (OnShowUI()) {
			Update();
		}

		GUI::TreePop();
	}
}