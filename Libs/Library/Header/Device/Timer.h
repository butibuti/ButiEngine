#pragma once
#include <memory>
namespace ButiEngine {

	class Timer :public IObject
	{
	public:
		Timer(float arg_maxCountFrame);
		virtual bool Update() = 0;
		bool IsOn();
		void Start();
		void Stop();
		void Initialize()override;
		void PreInitialize()override;
		void SetCount(int arg_nowCount);
		void ChangeCountFrame(int arg_maxCount);
		void Reset();
		inline float GetPercent() const {
			if (maxCountFrame == 0)return 0;
			return (float)nowCountFrame / (float)maxCountFrame;
		}
		inline float GetRemainFrame()const {
			return maxCountFrame - nowCountFrame;
		}
	protected:
		float maxCountFrame;
		float nowCountFrame;
		bool isOn;
	};
}

