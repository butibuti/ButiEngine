#pragma once
#include"Header/GameObjects/Behavior.h"
namespace ButiEngine {

	class SplineCurve {
	public:
		SplineCurve(std::vector<Vector3> arg_points) {
			points = arg_points;
		}
		static Vector3 CatmullRom(float t, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3) {
			Vector3 a = -p0 + 3.0f * p1 - 3.0f * p2 + p3;
			Vector3 b = 2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3;
			Vector3 c = -p0 + p2;
			Vector3 d = 2.0f * p1;

			return 0.5f * ((a * t * t * t) + (b * t * t) + (c * t) + d);
		}
		static Vector3 CatmullRomFirst(float t, Vector3 p0, Vector3 p1, Vector3 p2) {
			Vector3 b = p0 - 2.0f * p1 + p2;
			Vector3 c = -3.0f * p0 + 4.0f * p1 - p2;
			Vector3 d = 2.0f * p0;

			return 0.5f * ((b * t * t) + (c * t) + d);
		}
		static Vector3 CatmullRomLast(float t, Vector3 p0, Vector3 p1, Vector3 p2) {
			Vector3 b = p0 - 2.0f * p1 + p2;
			Vector3 c = -p0 + p2;
			Vector3 d = 2.0f * p1;

			return 0.5f * ((b * t * t) + (c * t) + d);
		}
		Vector3 GetPoint(const float t)const {
			int lineCount = points.size()+ (closed ? 1 : 0) - 1;

			float eachDividedCount = 1.0f / lineCount;
			int p = t/ eachDividedCount;
			float divideT = (t - p * eachDividedCount) / eachDividedCount;

					if (!closed && p == 0) {
						return CatmullRomFirst(divideT,
							points[p],
							points[p + 1],
							points[p + 2]
						);
					}
					else if (!closed && p == points.size() - 2) {
						return CatmullRomLast(divideT,
							points[p - 1],
							points[p],
							points[p + 1]
						);
					}
					else {
						return CatmullRom(divideT,
							points[p == 0 ? points.size() - 1 : p - 1],
							points[p],
							points[(p + 1) % points.size()],
							points[(p + 2) % points.size()]
						);
					}


		}
	private:
		std::vector<Vector3> points;
		bool closed = false;
	};

	class SampleBehavior:public Behavior
	{
		std::string GetBehaviorName()override {
			return "SampleBehavior";
		}
		void OnUpdate()override;
		void OnSet()override;
		std::shared_ptr<Behavior> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void OnShowUI()override;
	private:
		SplineCurve spline = SplineCurve({Vector3(-5, 1, 10),Vector3(-1, 3, 5), Vector3(4, 5, 2), Vector3(5, 1, -1) });
		float t = 0;
	};

}


CEREAL_REGISTER_TYPE(ButiEngine::SampleBehavior);

CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::Behavior, ButiEngine::SampleBehavior)