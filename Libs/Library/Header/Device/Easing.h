#include "stdafx.h"
namespace ButiEngine {
	namespace Easing {
        static float EaseInSine(const float x) {
        return 1 - std::cos((x * DirectX::XM_PI) / 2);
        }

        static float EaseOutSine(const float x) {
               return std::sin((x * DirectX::XM_PI) / 2);
        }
        static float EaseInOutSine(const float x) {
               return -(std::cos(DirectX::XM_PI * x) - 1) / 2;
        }
        static float EaseInQuad(const float x) {
               return x * x;
        }
        static float EaseOutQuad(const float x) {
                return 1 - (1 - x) * (1 - x);
        }
        static float EaseInOutQuad(const float x) {
                return x < 0.5 ? 2 * x * x : 1 - std::pow(-2 * x + 2, 2) / 2;
        }
        static float EaseInCubic(const float x) {
                return x * x * x;
        }
       static float EaseOutCubic(const float x) {
                return 1 - std::pow(1 - x, 3);
        }
        static float EaseInOutCubic(const float x) {
                return x < 0.5 ? 4 * x * x * x : 1 - std::pow(-2 * x + 2, 3) / 2;
        }
        static float EaseInQuart(const float x) {
                return x * x * x * x;
        }
        static float EaseOutQuart(const float x) {
                return 1 - std::pow(1 - x, 4);

        }
        static float EaseInOutQuart(const float x) {
                return x < 0.5 ? 8 * x * x * x * x : 1 - std::pow(-2 * x + 2, 4) / 2;
        }
        static float EaseInCirc(const float x) {
                return 1 - std::sqrt(1 - std::pow(x, 2));
        }
        static float EaseOutCirc(const float x) {
                return std::sqrt(1 - std::pow(x - 1, 2));
        }
        static float EaseInOutCirc(const float x) {
                return x < 0.5
                  ? (1 - std::sqrt(1 - std::pow(2 * x, 2))) / 2
                  : (std::sqrt(1 - std::pow(-2 * x + 2, 2)) + 1) / 2;
        }

        const  float c1 = 1.70158;
        const float c2 = c1 * 1.525;

        static float EaseInBack(const float x) {
                float c3 = c1 + 1;

                return c3 * x * x * x - c1 * x * x;
        }
        static float EaseOutBack(const float x) {
                float c3 = c1 + 1;

                return 1 + c3 * std::pow(x - 1, 3) + c1 * std::pow(x - 1, 2);
        }
        static float EaseInOutBack(const float x) {

                return x < 0.5
                  ? (std::pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
                  : (std::pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
        }
        const  float c4 = (2 * DirectX::XM_PI) / 3;
        static float EaseInElastic(const float x) {
               

                return x ==  0
                  ? 0
                  : x ==  1
                  ? 1
                  : -std::pow(2, 10 * x - 10) * std::sin((x * 10 - 10.75) * c4);
        }
        static float EaseOutElastic(const float x) {

                return x ==  0
                  ? 0
                  : x ==  1
                  ? 1
                  : std::pow(2, -10 * x) * std::sin((x * 10 - 0.75) * c4) + 1;
        }
        const float c5 = (2 * DirectX::XM_PI) / 4.5;;
        static float EaseInOutElastic(const float x) {

                return x ==  0
                  ? 0
                  : x ==  1
                  ? 1
                  : x < 0.5
                  ? -(std::pow(2, 20 * x - 10) * std::sin((20 * x - 11.125) * c5)) / 2
                  : (std::pow(2, -20 * x + 10) * std::sin((20 * x - 11.125) * c5)) / 2 + 1;
        }
        static float EaseInQuint(const float x) {
                return x * x * x * x * x;
        }
        static float EaseOutQuint(const float x) {
                return 1 - std::pow(1 - x, 5);
        }
        static float EaseInOutQuint(const float x) {
                return x < 0.5 ? 16 * x * x * x * x * x : 1 - std::pow(-2 * x + 2, 5) / 2;
        }
        static float EaseInExpo(const float x) {
                return x ==  0 ? 0 : std::pow(2, 10 * x - 10);
        }
        static float EaseOutExpo(const float x) {
                return x ==  1 ? 1 : 1 - std::pow(2, -10 * x);
        }
        static float EaseInOutExpo(const float x) {
                return x == 0
                  ? 0
                  : x ==  1
                  ? 1
                  : x < 0.5 ? std::pow(2, 20 * x - 10) / 2
                  : (2 - std::pow(2, -20 * x + 10)) / 2;
        }
        static float Parabola(const float x) {
                return -(x - 0.5) * (x - 0.5) * 4 + 1;
        }


	}


}