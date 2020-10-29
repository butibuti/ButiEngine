#pragma once
#include"RelativeTimer.h"
namespace ButiEngine {
	class AbsoluteTimer :public Timer
	{
	public:
		AbsoluteTimer(float arg_maxCountFrame) :Timer(arg_maxCountFrame) {}
		inline bool Update() override{
			if (!isOn) {
				return false;
			}
			nowCountFrame++;
			if (nowCountFrame >= maxCountFrame) {
				nowCountFrame = 0;
				return true;
			}
			return false;
		}
	};

}