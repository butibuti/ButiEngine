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

	class AbsoluteTimer :public Timer
	{
	public:
		AbsoluteTimer(float arg_maxCountFrame) :Timer(arg_maxCountFrame) {}
		inline bool Update() override {
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

	class RelativeTimer :public Timer
	{
	public:
		RelativeTimer() :Timer(0) {}
		RelativeTimer(float arg_maxCountFrame) :Timer(arg_maxCountFrame) {}
		inline bool Update()override {
			if (!isOn) {
				return false;
			}
			nowCountFrame += 1; //Game::GetInstance()->GetGameTime()->GetSpeed();
			if (nowCountFrame >= maxCountFrame) {
				nowCountFrame = 0;
				return true;
			}
			return false;
		}

		std::shared_ptr<RelativeTimer> Clone() {
			auto cloned = ObjectFactory::Create<RelativeTimer>(maxCountFrame);

			cloned->isOn = isOn;

			return cloned;
		}

		void ShowGUI() {
			GUI::Checkbox("On/Off", &isOn);

			GUI::DragFloat("Time", &nowCountFrame, 0.1f, 0, maxCountFrame);
			GUI::DragFloat("MaxTime", &maxCountFrame, 0.1f, 0, 1000);


		}
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(nowCountFrame);
			archive(maxCountFrame);
			archive(isOn);
		}
	};

	template <class T>
	class TimeBomb :public Timer
	{

	public:
		using TsFunction = void (T::*)();
		TimeBomb(float arg_maxCountFrame, std::shared_ptr<T> arg_shp_instance) :Timer(arg_maxCountFrame) {
			shp_instance = arg_shp_instance;
		}
		inline bool Update()override {
			if (!isOn) {
				return false;
			}
			nowCountFrame += 1; //Game::GetInstance()->GetGameTime()->GetSpeed();
			if (nowCountFrame >= maxCountFrame) {
				nowCountFrame = 0;
				(*(shp_instance.lock()).*bombFunc)();
				return true;
			}
			return false;
		}
		inline bool Update_UnExecute() {
			if (!isOn) {
				return false;
			}
			nowCountFrame += 1; //Game::GetInstance()->GetGameTime()->GetSpeed();
			if (nowCountFrame >= maxCountFrame) {
				nowCountFrame = maxCountFrame;
				return false;
			}
			return false;
		}
		TsFunction bombFunc;

	private:
		std::weak_ptr<T> shp_instance;
	};
}

