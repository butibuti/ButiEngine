#pragma once
#include"stdafx.h"
namespace ButiEngine {
	class RelativeTimer :public Timer
	{
	public:
		RelativeTimer():Timer(0){}
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
			ImGui::Checkbox("On/Off", &isOn);

			ImGui::DragFloat("Time", &nowCountFrame, 0.1f, 0, maxCountFrame);
			ImGui::DragFloat("MaxTime", &maxCountFrame, 0.1f, 0, 1000);


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
		TimeBomb(float arg_maxCountFrame,  std::shared_ptr<T> arg_shp_instance) :Timer(arg_maxCountFrame) {
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
