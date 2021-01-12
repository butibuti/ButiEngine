#include"stdafx.h"
#include "IDrawObject.h"

void ButiEngine::IBoneDrawObject::InverseKinematic()
{

	for (auto itr = vec_IKBone.begin(); itr != vec_IKBone.end(); itr++) {
		(*itr)->CCDInverseKinematic();
	}
}
