#pragma once
#include"stdafx.h"
namespace ButiEngine {

	class ShaderVariable_Gradation_LineMove :public ModelMatrix
	{
	public:
		Vector4 color;
		Vector4 gradationPase;
		Vector4 yPosAndRange;//x=pos,y=range,z=moveMax,w=moveMin
	};
}