#pragma once
#include"stdafx.h"
#define PI 3.141592653589793
namespace ButiEngine {

	class Calculator
	{
	private:
		Calculator();
		~Calculator();
	public:
		static bool IsClossLine(Vector2 a, Vector2 b, Vector2 c, Vector2 d);
		static float Abs(float value);
		static int Abs(int value);
		static double Abs(double value);
		static float Distance(Vector3 pointA, Vector3 pointB);
		static float EaseIn(float t, float b, float c, float d) {
			t /= d;
			return c * t*t + b;
		};
		static Vector3 Normalize(Vector3 value);
		static float ToRadian(float deg);
		static float ToDegree(float rad);
		static Vector2 Float2Verticalize(Vector2);
		static Vector2 Normalize2D(Vector2 arg);
		static Vector2 Float2Addition(Vector2 a, Vector2 b);
		static Vector2 Float2Multiplication(Vector2 a, float scalar);
		static float GetCross2D(Vector2 a, Vector2 b, Vector2 c);
		static Vector3 ScreenPointToWorldPoint(Vector2 screenPos, float z, Vector2 screenRect, DirectX::XMMATRIX * p_view, DirectX::XMMATRIX * p_proj);
		static Vector3 ScreenPointToXYPlane(Vector2 screenPos, Vector2 screenRect, DirectX::XMMATRIX * p_view, DirectX::XMMATRIX * p_proj);
		static std::vector<Vector2> GetPerimeter(std::vector<Vector2> poses);;

		static inline Quat LearpQuat(const Quat& arg_firstQuat, const Quat& arg_secondQuat, const float pase) {
			Quat out=Quat();
			const float len1 = arg_firstQuat.GetLength();
			const float len2 = arg_firstQuat.GetLength();

			if (len1 == 0.0f || len2 == 0.0f)
				return out;

			const float cos_val = (arg_firstQuat[0] * arg_secondQuat[0] + arg_firstQuat[1] * arg_secondQuat[1] + arg_firstQuat[2] * arg_secondQuat[2] + arg_firstQuat[3] * arg_secondQuat[3]) / (len1 * len2);
			const float w = acosf(cos_val);


			const float sin_w = sinf(w);
			const float sin_t_w = sinf(pase* w);
			const float sin_inv_t_w = sinf((1.0f - pase) * w);
			const float mult_q1 = sin_inv_t_w / sin_w;
			const float mult_q2 = sin_t_w / sin_w;

			for (int i = 0; i < 4; i++)
				out[i] = mult_q1 * arg_firstQuat[i] + mult_q2 * arg_secondQuat[i];

			return out;
		}
		static Quat SphereLerp(Quat arg_initQuat, Quat arg_quat, float time) {
			auto output = Quat();
			float ht = arg_initQuat.x * arg_quat.x + arg_initQuat.y * arg_quat.y + arg_initQuat.z * arg_quat.z + arg_initQuat.w * arg_quat.w;
			float hs = 1.0 - ht * ht;
			if (hs <= 0.0) {
				output.x = arg_initQuat.x;
				output.y = arg_initQuat.y;
				output.z = arg_initQuat.z;
				output.w = arg_initQuat.w;
			}
			else {
				hs = std::sqrt(hs);
				if (std::abs(hs) < 0.0001) {
					output.x = (arg_initQuat.x * 0.5 + arg_quat.x * 0.5);
					output.y = (arg_initQuat.y * 0.5 + arg_quat.y * 0.5);
					output.z = (arg_initQuat.z * 0.5 + arg_quat.z * 0.5);
					output.w = (arg_initQuat.w * 0.5 + arg_quat.w * 0.5);
				}
				else {
					float ph = std::acos(ht);
					float pt = ph * time;
					float t0 = std::sin(ph - pt) / hs;
					float t1 = std::sin(pt) / hs;
					output.x = arg_initQuat.x * t0 + arg_quat.x * t1;
					output.y = arg_initQuat.y * t0 + arg_quat.y * t1;
					output.z = arg_initQuat.z * t0 + arg_quat.z * t1;
					output.w = arg_initQuat.w * t0 + arg_quat.w * t1;
				}
			}
			return output;
		
		}
		static inline Vector3 LarpPosition(const Vector3& arg_startPoint, const Vector3& arg_endPoint, const float t) {
			return arg_startPoint + (arg_endPoint - arg_startPoint) * t;

		}

		static inline Vector3 LarpPosition(const Vector3& arg_startPoint, const Vector3& arg_endPoint, const float xt, const float yt, const float zt) {
			return Vector3(arg_startPoint.x + (arg_endPoint.x - arg_startPoint.x) * xt, arg_startPoint.y + (arg_endPoint.y - arg_startPoint.y) * yt, arg_startPoint.z + (arg_endPoint.z - arg_startPoint.z) * zt);

		}

		static const int IntMax = 2147483647;
		static const char CharMax = 127;
		static const short ShortMax = 32767;

		static char GetByteSize(const int arg_check);
		static char GetUnsignedByteSize(const UINT arg_check);

		inline static  Vector3 Cross(const Vector3 & vec0, const Vector3 & vec1)
		{
			return (Vector3)XMVector3Cross(vec0, vec1);
		}
		inline static  Vector2 Cross(const Vector2 & vec0, const Vector2 & vec1) {
			return (Vector2)XMVector2Cross(vec0, vec1);
		}
		inline static  Vector4 Cross(const Vector4 & vec0, const Vector4 & vec1, const Vector4 & vec2);
	};
}

