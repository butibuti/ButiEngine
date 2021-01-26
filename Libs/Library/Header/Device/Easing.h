#include <cmath>
namespace ButiEngine {

    #define PI 3.141592653589793

    namespace Easing {
        enum class EasingType {
            EaseIn, EaseOut, EaseInOut,
            EaseInSin, EaseOutSin, EaseInOutSin,
            EaseInQuad, EaseOutQuad, EaseInOutQuad,
            EaseInCubic, EaseOutCubic, EaseInOutCubic,
            EaseInQuart, EaseOutQuart, EaseInOutQuart,
            EaseInCirc, EaseOutCirc, EaseInOutCirc,
            EaseInBack, EaseOutBack, EaseInOutBack,
            EaseInElastic, EaseOutElastic, EaseInOutElastic,
            EaseInQuint, EaseOutQuint, EaseInOutQuint,
            EaseInExpo, EaseOutExpo, EaseInOutExpo,
            Parabola, Liner,none, CubeRotate90, CubeRotate180, CubeRotateMin90, CubeRotateMin180
        };
        static std::string  EasingTypeNames[] = {
            "EaseIn"," EaseOut"," EaseInOut",
            "EaseInSin"," EaseOutSin"," EaseInOutSin",
            "EaseInQuad"," EaseOutQuad"," EaseInOutQuad",
            "EaseInCubic","EaseOutCubic"," EaseInOutCubic",
            "EaseInQuart","EaseOutQuart"," EaseInOutQuart",
            "EaseInCirc"," EaseOutCirc"," EaseInOutCirc",
            "EaseInBack"," EaseOutBack"," EaseInOutBack",
            "EaseInElastic"," EaseOutElastic"," EaseInOutElastic",
            "EaseInQuint"," EaseOutQuint"," EaseInOutQuint",
            "EaseInExpo"," EaseOutExpo"," EaseInOutExpo",
            "Parabola"," Liner","none",
            "CubeRotate90","CubeRotate180","CubeRotateMin90","CubeRotateMin180"
        };
        static float EaseIn(const float x) {
            return x * x;
        }

        static float EaseOut(const float x) {
            return x * (2 - x);
        }
        static float EaseInOut(const float x) {
            return x * x * (3 - 2 * x);
        }
        static float EaseInSin(const float x) {
            return 1 - std::cos((x * PI) / 2);
        }

        static float EaseOutSin(const float x) {
            return std::sin((x * PI) / 2);
        }
        static float EaseInOutSin(const float x) {
            return -(std::cos(PI * x) - 1) / 2;
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


            return x == 0
                ? 0
                : x == 1
                ? 1
                : -std::pow(2, 10 * x - 10) * std::sin((x * 10 - 10.75) * c4);
        }
        static float EaseOutElastic(const float x) {

            return x == 0
                ? 0
                : x == 1
                ? 1
                : std::pow(2, -10 * x) * std::sin((x * 10 - 0.75) * c4) + 1;
        }
        const float c5 = (2 * PI) / 4.5;;
        static float EaseInOutElastic(const float x) {

            return x == 0
                ? 0
                : x == 1
                ? 1
                : x < 0.5
                ? -(std::pow(2, 20 * x - 10) * std::sin((20.0 * x - 11.125) * c5)) / 2
                : (std::pow(2, -20 * x + 10) * std::sin((20.0 * x - 11.125) * c5)) / 2 + 1;
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
            return x == 0 ? 0 : std::pow(2, 10 * x - 10);
        }
        static float EaseOutExpo(const float x) {
            return x == 1 ? 1 : 1 - std::pow(2, -10 * x);
        }
        static float EaseInOutExpo(const float x) {
            return x == 0
                ? 0
                : x == 1
                ? 1
                : x < 0.5 ? std::pow(2, 20 * x - 10) / 2
                : (2 - std::pow(2, -20 * x + 10)) / 2;
        }
        static float Parabola(const float x) {
            return -(x - 0.5) * (x - 0.5) * 4 + 1;
        }

        static float Liner(const float x) {
            return x;
        }
        static float none(const float x) {
            return 0;
        }
        static float CubeRotate90(const float x) {
            return std::sin((x * 0.5 + 0.25) * PI) - std::sin((0.25) * PI);
        }
        static float CubeRotate180(const float x) {
            return  CubeRotate90(x) * PI/2;
        }
        static float CubeRotateMin90(const float x) {
            return -(std::sin((x * 0.5 + 0.25) * PI) - std::sin((0.25) * PI));
        }
        static float CubeRotateMin180(const float x) {
            return  CubeRotateMin90(x) * PI/2;
        }

        static  float (*EaseFunctions[])(const float) = { EaseIn, EaseOut, EaseInOut,
                 EaseInSin, EaseOutSin, EaseInOutSin,
                 EaseInQuad, EaseOutQuad, EaseInOutQuad,
                 EaseInCubic, EaseOutCubic, EaseInOutCubic,
                 EaseInQuart, EaseOutQuart, EaseInOutQuart,
                 EaseInCirc, EaseOutCirc, EaseInOutCirc,
                 EaseInBack, EaseOutBack, EaseInOutBack,
                 EaseInElastic, EaseOutElastic, EaseInOutElastic,
                 EaseInQuint, EaseOutQuint, EaseInOutQuint,
                 EaseInExpo, EaseOutExpo, EaseInOutExpo,
                 Parabola, Liner,none,CubeRotate90 ,CubeRotate180,CubeRotateMin90 ,CubeRotateMin180 };
        static float GetEase(float x, EasingType type) {
            return (EaseFunctions)[(UINT)type](x);
        }

    }


}