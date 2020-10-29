#pragma once
#include"stdafx.h"
namespace ButiEngine {

	struct Vector2;
	struct Vector3;
	struct Vector4;
	struct Quat;
	struct Matrix4x4;

	struct UInt2 :public DirectX::XMUINT2 {
		inline UInt2(int arg_x, int arg_y) {
			x = arg_x;
			y = arg_y;
		}
		inline UInt2() {
			x = 0;
			y = 0;
		}
	};

	struct Matrix4x4 :public DirectX::XMFLOAT4X4
	{
	public:
		inline Matrix4x4() {
			_11 = 1.0f;
			_12 = 0.0f;
			_13 = 0.0f;
			_14 = 0.0f;

			_21 = 0.0f;
			_22 = 1.0f;
			_23 = 0.0f;
			_24 = 0.0f;

			_31 = 0.0f;
			_32 = 0.0f;
			_33 = 1.0f;
			_34 = 0.0f;

			_41 = 0.0f;
			_42 = 0.0f;
			_43 = 0.0f;
			_44 = 1.0f;
		}
		inline Matrix4x4(const DirectX::XMMATRIX other)
		{
			DirectX::XMMATRIX tmp = other;
			DirectX::XMStoreFloat4x4(this, tmp);
		}
		explicit inline Matrix4x4(const Quat& quat);
		inline Matrix4x4& operator=(const DirectX::XMMATRIX& other)
		{
			auto tmp = other;
			DirectX::XMStoreFloat4x4(this, tmp);
			return *this;
		}

		inline Vector4 operator*(const Vector4& vec4);

		inline operator DirectX::XMMATRIX() const
		{
			auto tmp = *this;
			auto output = DirectX::XMLoadFloat4x4(&tmp);
			return output;
		}
		inline Matrix4x4& Identity()
		{
			*this = (Matrix4x4)DirectX::XMMatrixIdentity();
			return *this;
		}
		inline Matrix4x4& Transpose() {

			*this = (Matrix4x4)XMMatrixTranspose(*this);
			return *this;
		}
		inline Matrix4x4 GetTranspose() const{
			return  (Matrix4x4)XMMatrixTranspose(*this);
		}
		inline Matrix4x4& Inverse() {
			*this = (Matrix4x4)XMMatrixInverse(nullptr, *this);
			return *this;
		}
		inline Matrix4x4 GetInverse()const {
			return (Matrix4x4)XMMatrixInverse(nullptr, *this);
		}
		inline Matrix4x4 GetInValidYZ()const {
			Matrix4x4 output = *this;
			output._11 = 1.0f;
			output._21 = 0.0f;
			output._31 = 0.0f;
			return output;
		}
		inline Matrix4x4 GetInValidXZ()const {
			Matrix4x4 output = *this;
			output._12 = 0.0f;
			output._22 = 1.0f;
			output._32 = 0.0f;
			return output;
		}
		inline Matrix4x4 GetInValidXY()const {
			Matrix4x4 output = *this;
			output._13 = 0.0f;
			output._23 = 0.0f;
			output._33 = 1.0f;
			return output;
		}inline Matrix4x4& InValidYZ() {
			this->_11 = 1.0f;
			this->_21 = 0.0f;
			this->_31 = 0.0f;
			return *this;
		}
		inline Matrix4x4& InValidXZ(){
			this->_12 = 0.0f;
			this->_22 = 1.0f;
			this->_32 = 0.0f;
			return *this;
		}
		inline Matrix4x4& InValidXY() {
			
			 this->_13 = 0.0f;
			 this->_23 = 0.0f;
			 this->_33 = 1.0f;
			return *this;
		}
		inline Quat ToQuat()const;

		inline void Decompose(Vector3& rScaling, Quat& rQt, Vector3& rTranslation)const;

		inline Vector3 GetEuler(const bool arg_reverse = false)const;
		inline Vector3 GetEulerOneValue()const;

		inline Vector3 GetEulerDegrees(const bool arg_reverse=false)const;

		inline Matrix4x4& CreateFromEuler(const float arg_x, const float arg_y, const float arg_z) {
			*this= DirectX::XMMatrixRotationX(
				arg_x
			) *
				DirectX::XMMatrixRotationY(
					arg_y
				) *
				DirectX::XMMatrixRotationZ(
					arg_z
				);
			return *this;
		}
		inline bool Same(const Matrix4x4& other,const float epsilon=0.001f) const{

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (abs(this->m[i][j] - other.m[i][j])>epsilon) {
						return false;
					}
				}
			}

			return true;
		}
		~Matrix4x4() {}
	};
	struct Vector2 :public DirectX::XMFLOAT2
	{
		inline Vector2(const DirectX::XMVECTOR other)
		{

			DirectX::XMVECTOR temp = other;
			DirectX::XMStoreFloat2(this, temp);
		};

		inline Vector2(float x, float y)
		{
			this->x = x;
			this->y = y;
		}

		inline Vector2() {
			this->x = 0;
			this->y = 0;
		}

		inline ~Vector2() {}


		inline  float* GetData()
		{
			float output[2] = { x,y };
			return output;
		}
		inline  void GetData(float* out)
		{
			auto data = GetData();
			for (int i = 0; i < 2; i++) {
				out[i] = data[i];
			}
		}

		inline Vector2& operator=(const DirectX::XMVECTOR other)
		{
			DirectX::XMVECTOR temp = other;
			DirectX::XMStoreFloat2(this, temp);
			return *this;
		}

		inline Vector2& operator +=(const Vector2& vec)
		{
			*this = *this + vec;
			return *this;
		}

		inline Vector2& operator +=(float value)
		{
			*this = *this + value;
			return *this;
		}

		inline Vector2 operator +(float value)
		{
			Vector2 temp(value, value);
			return (Vector2)XMVectorAdd(*this, temp);
		}

		inline Vector2& operator -=(const Vector2& vec)
		{
			*this = *this - vec;
			return *this;
		}

		inline Vector2& operator -=(float value)
		{
			*this = *this - value;
			return *this;
		}


		inline Vector2 operator -(float value)
		{
			Vector2 temp(value, value);
			return (Vector2)XMVectorSubtract(*this, temp);
		}

		inline Vector2& operator *=(float value)
		{
			*this = *this * value;
			return *this;
		}

		inline Vector2& operator /=(float value)
		{
			*this = *this / value;
			return *this;
		}

		inline const Vector2 operator -() const
		{
			return Vector2();
		}


		inline Vector2& Floor(int len)
		{
			// TODO: return ステートメントをここに挿入します
		}

		inline Vector2& Round(int len)
		{
			// TODO: return ステートメントをここに挿入します
		}

		inline Vector2& Ceil(int len)
		{
			// TODO: return ステートメントをここに挿入します
		}

		inline float Dot(const Vector2& vec1) const
		{
			return ((Vector2)DirectX::XMVector2Dot(DirectX::XMVECTOR(*this), vec1)).x;
		}

		inline bool isNaN() const
		{
			return XMVector2IsNaN(*this);
		}

		inline bool isInfinite() const
		{
			return XMVector2IsInfinite(*this);
		}

		inline float GetLengthSqr() const
		{
			return ((Vector2)DirectX::XMVector2LengthSq(DirectX::XMVECTOR(*this))).x;
		}

		inline operator DirectX::XMVECTOR() const
		{
			DirectX::XMFLOAT2 temp = *this;
			DirectX::XMVECTOR output = DirectX::XMLoadFloat2(&temp);
			return output;
		}

		inline float GetLength() const {
			return ((Vector2)DirectX::XMVector2Length(DirectX::XMVECTOR(*this))).x;
		}

		inline float GetDistance(Vector2& other) const {
			auto i = sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y));
			return i;
		}

		inline void Normalize()
		{
			*this = DirectX::XMVector2Normalize(DirectX::XMVECTOR(*this));
		}
	};

	struct  Vector3 :public DirectX::XMFLOAT3
	{
		static Vector3 XAxis;
		static Vector3 YAxis;
		static Vector3 ZAxis;

		inline Vector3(const DirectX::XMVECTOR other)
		{
			DirectX::XMVECTOR temp = other;
			DirectX::XMStoreFloat3(this, temp);
		}
		inline Vector3(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		inline Vector3()
		{
			x = 0;
			y = 0;
			z = 0;
		}

		inline Vector2 GetVector2() {
			return Vector2(x, y);
		}

		inline ~Vector3()
		{
		}
		inline  float* GetData() {
			float output[3] = { x,y,z };
			return output;
		}

		inline  void GetData(float* out)
		{
			auto data = GetData();
			for (int i = 0; i < 3; i++) {
				out[i] = data[i];
			}
		}
		inline Vector3& operator=(const DirectX::XMVECTOR other)
		{
			DirectX::XMVECTOR temp = other;
			DirectX::XMStoreFloat3(this, temp);
			return *this;

		}

		inline Vector3 operator=(const Vector3& other)
		{
			if (this != &other) {
				x = other.x;
				y = other.y;
				z = other.z;
			}
			return *this;
		}

		inline Vector3& operator=(const DirectX::XMFLOAT3& other)
		{
			DirectX::XMVECTOR temp = DirectX::XMLoadFloat3(&other);
			DirectX::XMStoreFloat3(this, temp);
			return *this;
		}

		inline Vector3& operator +=(const Vector3& vec)
		{
			*this = *this + vec;
			return *this;
		}

		inline Vector3& operator +=(float value)
		{
			*this = *this + value;
			return *this;
		}


		inline Vector3 operator +(float value)
		{
			Vector3 temp(value, value, value);
			return (Vector3)XMVectorAdd(*this, temp);
		}

		inline Vector3& operator -=(const Vector3& vec)
		{
			*this = *this - vec;
			return *this;
		}

		inline Vector3& operator -=(float value)
		{
			*this = *this - value;
			return *this;
		}

		inline Vector3 operator -(float value)
		{
			Vector3 temp(value, value, value);
			return (Vector3)XMVectorSubtract(*this, temp);
		}

		inline Vector3& operator *=(float value)
		{
			*this = *this * value;
			return *this;
		}

		inline Vector3& operator *=(Matrix4x4 value)
		{
			*this = *this * value;
			return *this;
		}

		inline Vector3& operator *(Matrix4x4 value) const
		{
			auto tmp = (Vector3)XMVector3Transform(*this, value);
			return tmp;
		}

		inline Vector3 operator *(const Vector3& other)const {

			return Vector3(other.x * x, other.y * y, other.z * z);
		}

		inline Vector3& operator *=(const Vector3& other) {
			*this = Vector3(other.x * x, other.y * y, other.z * z);
			return  *this;
		}

		inline Vector3& operator /=(float value)
		{
			*this = *this / value;
			return *this;
		}

		inline const Vector3 operator -() const
		{
			Vector3 temp(-1.0f, -1.0f, -1.0f);
			return (Vector3)XMVectorSubtract(*this, temp);
		}

		inline const bool operator==(const Vector3& other)const {
			if (other.x != this->x || other.y != this->y || other.z != this->z) {
				return false;
			}
			return true;
		}

		inline const bool operator!=(const Vector3& other)const {

			return !(*this == other);
		}

		inline const float Get(const UINT index)const {
			return *((&x) + index);
		}

		inline Vector3& Floor(int len)
		{
			// TODO: return ステートメントをここに挿入します
		}

		inline Vector3& Round(int len)
		{
			// TODO: return ステートメントをここに挿入します
		}

		inline Vector3& Ceil(int len)
		{
			// TODO: return ステートメントをここに挿入します
		}

		inline float Dot(const Vector3& vec1) const
		{
			return ((Vector3)XMVector3Dot(*this, vec1)).x;
		}

		inline Vector3& Cross(const Vector3& vec1)
		{
			*this = (Vector3)XMVector3Cross(*this, vec1);
			return *this;
		}

		inline Vector3 GetCross(const Vector3& vec1)const
		{
			return (Vector3)XMVector3Cross(*this, vec1);
		}

		inline bool isNaN() const
		{
			return XMVector3IsNaN(*this);
		}

		inline bool isInfinite() const
		{
			return XMVector3IsInfinite(*this);
		}

		inline const Vector3& ToDegrees() {
			x = XMConvertToDegrees(x);
			y = XMConvertToDegrees(y);
			z = XMConvertToDegrees(z);

			return *this;
		}
		inline  Vector3 GetDegrees() {
			Vector3 output = Vector3(XMConvertToDegrees(x), XMConvertToDegrees(y), XMConvertToDegrees(z));
			

			return output;
		}
		inline Vector3 GetRemoveNan() {
			if (XMVector3IsNaN(*this)) {
				auto output = *this;

				if (output.x == nanf("")) {
					output.x = 0;
				}
				if (output.y == nanf("")) {
					output.y = 0;
				}
				if (output.z == nanf("")) {
					output.z = 0;
				}
			}
			return *this;
		}
		inline const Vector3& ToRadian() {
			x = XMConvertToRadians(x);
			y = XMConvertToRadians(y);
			z = XMConvertToRadians(z);

			return *this;
		}

		inline const Vector3& SetLimit(const Vector3& arg_max, const Vector3& arg_min) {
			x = max(arg_min.x, x);
			y = max(arg_min.y, y);
			z = max(arg_min.z, z);

			x = min(arg_max.x, x);
			y = min(arg_max.y, y);
			z = min(arg_max.z, z);
			return *this;
		}
		inline Vector3 GetFreeze(bool arg_freezeX, bool arg_freezeY, bool arg_freezeZ) {
			auto output = *this;
			if (arg_freezeX)
				output.x = 0;
			if (arg_freezeY)
				output.y = 0;
			if (arg_freezeZ)
				output.z = 0;

			return output;
		}

		inline float GetLengthSqr() const
		{
			return ((Vector3)XMVector3LengthSq(*this)).x;
		}

		inline operator DirectX::XMVECTOR() const
		{
			DirectX::XMFLOAT3 temp = *this;
			DirectX::XMVECTOR output = DirectX::XMLoadFloat3(&temp);
			return output;
		};

		inline float GetLength() const
		{
			return ((Vector3)DirectX::XMVector3Length(DirectX::XMVECTOR(*this))).x;
		}

		inline void Normalize()
		{
			*this = DirectX::XMVector3Normalize(DirectX::XMVECTOR(*this));
		}
		inline Vector3 GetNormalize() const
		{
			return DirectX::XMVector3Normalize(DirectX::XMVECTOR(*this));
		}
	};

	struct Vector4 :public DirectX::XMFLOAT4
	{
		inline Vector4(const DirectX::XMVECTOR other)
		{
			DirectX::XMVECTOR temp = other;
			DirectX::XMStoreFloat4(this, temp);
		}
		inline Vector4(float x, float y, float z, float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}
		inline Vector4(const Vector3& xyz, float w)
		{
			this->x = xyz.x;
			this->y = xyz.y;
			this->z = xyz.z;
			this->w = w;
		}

		explicit inline Vector4(const XMFLOAT4& v) :
			DirectX::XMFLOAT4(v)
		{
		}
		inline Vector4()
		{
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}

		inline  float* GetData() const
		{
			float output[4] = { x,y,z,w };
			return output;
		};
		inline  void GetData(float* out) const
		{
			auto data = GetData();
			for (int i = 0; i < 4; i++) {
				out[i] = data[i];
			}
		}
		inline Vector4& operator=(const DirectX::XMVECTOR& other)
		{
			DirectX::XMVECTOR temp = other;
			DirectX::XMStoreFloat4(this, temp);
			return *this;
		};
		inline ~Vector4() {};
		inline Vector4& operator +=(const Vector4& vec) {
			*this = *this + vec;
			return *this;
		};


		inline Vector4& operator +=(float value) {
			*this = *this + value;
			return *this;
		};

		inline Vector4 operator +(float value) {
			Vector4 temp(value, value, value, value);
			return (Vector4)XMVectorAdd(*this, temp);
		}


		inline Vector4& operator -=(const Vector4& vec)
		{
			*this = *this - vec;
			return *this;
		}

		inline Vector4& operator -=(float value)
		{
			*this = *this - value;
			return *this;
		};

		inline Vector4 operator -(float value)
		{
			Vector4 temp(value, value, value, value);
			return (Vector4)XMVectorSubtract(*this, temp);
		};

		inline Vector4& operator *=(float value)
		{
			*this = *this * value;
			return *this;
		}

		inline Vector4& operator /=(float value)
		{
			*this = *this / value;
			return *this;
		}

		inline const Vector4 operator -() const
		{
			Vector4 temp(1.0f, 1.0f, 1.0f, 1.0f);
			return (Vector4)XMVectorSubtract(*this, temp);
		}


		inline Vector4& Floor(int len) {}

		inline Vector4& Round(int len) {}

		inline Vector4& Ceil(int len) {}

		inline float Dot(const Vector4& vec1) const
		{
			return ((Vector3)XMVector4Dot(*this, vec1)).x;
		}

		inline bool isNaN() const
		{
			return XMVector4IsNaN(*this);
		}

		inline bool isInfinite() const
		{
			return XMVector4IsInfinite(*this);
		}

		inline float GetLengthSqr() const
		{
			return ((Vector3)DirectX::XMVector4LengthSq(DirectX::XMVECTOR(*this))).x;
		}
		inline Vector4& RemoveNan() {
			if (XMVector4IsNaN(*this)) {
				
				if (isnan((*this).x)) {
					(*this).x = 0;
				}
				if (isnan( (*this).y )) {
					(*this).y = 0;
				}
				if (isnan((*this).z)) {
					(*this).z = 0;
				}
				if (isnan((*this).w)) {
					(*this).w = 0;
				}
			}
			return *this;
		}
		inline operator DirectX::XMVECTOR() const
		{
			DirectX::XMFLOAT4 temp = *this;
			DirectX::XMVECTOR output = DirectX::XMLoadFloat4(&temp);
			return output;
		}

		inline float GetLength() const
		{
			return ((Vector4)DirectX::XMVector4Length(DirectX::XMVECTOR(*this))).x;
		}

		inline void Normalize()
		{
			*this = DirectX::XMVector4Normalize(DirectX::XMVECTOR(*this));
		}
	};

	class MathHelper
	{
	public:
		inline static double Ceil(double dSrc, int iLen)
		{
			double	dRet;

			dRet = dSrc * pow(10.0, iLen);
			dRet = (double)(int)(dRet + 0.9);

			return dRet * pow(10.0, -iLen);
		}
		inline static double Floor(double dSrc, int iLen)
		{
			double dRet;

			dRet = dSrc * pow(10.0, iLen);
			dRet = (double)(int)(dRet);

			return dRet * pow(10.0, -iLen);
		}
		inline static double Round(double dSrc, int iLen)
		{
			double	dRet;

			dRet = dSrc * pow(10.0, iLen);
			dRet = (double)(int)(dRet + 0.5);

			return dRet * pow(10.0, -iLen);
		}
		template<typename T>
		inline static T GetMaximum(const T& v1, const T& v2) {
			return v1 > v2 ? v1 : v2;
		}
		template<typename T>
		inline static T GetMinimum(const T& v1, const T& v2) {
			return v1 < v2 ? v1 : v2;
		}

		inline static float Dot(const Vector2& vec0, const Vector2& vec1)
		{
			return ((Vector2)XMVector2Dot(vec0, vec1)).x;
		}

		inline static float LengthSqr(const Vector2& vec)
		{
			return ((Vector2)XMVector2LengthSq(vec)).x;
		}

		inline static float Length(const Vector2& vec)
		{
			return ((Vector2)XMVector2Length(vec)).x;
		}

		inline static const Vector2 Normalize(const Vector2& vec)
		{
			return (Vector2)XMVector2Normalize(vec);
		}
		inline static float Dot(const Vector3& vec0, const Vector3& vec1)
		{
			return ((Vector3)XMVector3Dot(vec0, vec1)).x;
		}

		inline static float LengthSqr(const Vector3& vec)
		{
			return ((Vector3)XMVector3LengthSq(vec)).x;
		}

		inline static float Length(const Vector3& vec)
		{
			return ((Vector3)XMVector3Length(vec)).x;
		}

		inline static const Vector3 Normalize(const Vector3& vec)
		{
			return (Vector3)XMVector3Normalize(vec);
		}

		inline static const Vector3 Cross(const Vector3& vec0, const Vector3& vec1)
		{
			return (Vector3)XMVector3Cross(vec0, vec1);
		}

		inline static const Vector3 Reflect(const Vector3& vec, const Vector3& normal)
		{
			return (Vector3)XMVector3Reflect(vec, normal);
		}

		inline static const Vector3 Slide(const Vector3& vec, const Vector3& normal)
		{
			//vecと法線から直行線の長さ（内積で求める）
			float Len = Dot(vec, normal);
			//その長さに伸ばす
			Vector3 Contact = normal * Len;
			//スライドする方向は現在のベクトルから引き算
			return (vec - Contact);
		}
		inline static const Matrix4x4 inverse(const Matrix4x4& mat)
		{
			XMVECTOR Vec;
			return (Matrix4x4)XMMatrixInverse(&Vec, mat);
		}
	private:
		MathHelper() {};
	};



	struct Quat : public Vector4
	{

		inline Quat() :Vector4()
		{
			identity();
		}
		inline Quat(const Quat& quat) :
			Vector4(quat.x, quat.y, quat.z, quat.w)
		{
		}
		explicit inline Quat(const XMFLOAT4& v) :
			Vector4(v)
		{
		}
		inline Quat(float x, float y, float z, float w) :
			Vector4(x, y, z, w)
		{
		}

		inline Quat(const Vector3& vec, float r) :
			Vector4()
		{
			*this = (Quat)XMQuaternionRotationAxis(vec, r);
		}

		explicit inline Quat(float val) :
			Vector4()
		{
			x = val;
			y = val;
			z = val;
			w = val;
		}
		explicit inline Quat(const XMVECTOR& other) :
			Vector4()
		{
			XMVECTOR temp = other;
			XMStoreFloat4((XMFLOAT4*)this, temp);
		}

		explicit inline Quat(const Matrix4x4& m) {
			*this = m.ToQuat();
		}

		inline operator XMVECTOR() const {
			XMFLOAT4 temp = *this;
			XMVECTOR Vec = XMLoadFloat4(&temp);
			return Vec;
		}
		inline Quat& operator =(const Quat& quat)
		{
			if (this != &quat) {
				x = quat.x;
				y = quat.y;
				z = quat.z;
				w = quat.w;
			}
			return *this;
		}

		inline Quat& operator=(const XMFLOAT4& other) {
			(XMFLOAT4)*this = other;
			return *this;
		}

		inline Quat& operator=(const XMVECTOR& other) {
			XMVECTOR temp = other;
			XMStoreFloat4((XMFLOAT4*)this, temp);
			return *this;
		}

		inline bool operator==(const Quat& other)const {
			return XMQuaternionEqual(*this, other);
		}

		inline bool operator!=(const Quat& other)const {
			return !XMQuaternionEqual(*this, other);
		}

		inline Quat& setXYZ(const Vector3& vec)
		{
			x = vec.x;
			y = vec.y;
			z = vec.z;
			return *this;
		}

		inline const Vector3 getXYZ() const
		{
			return Vector3(x, y, z);
		}

		inline Quat& setX(float arg_x)
		{
			x = arg_x;
			return *this;
		}

		inline Quat& setY(float arg_y)
		{
			y = arg_y;
			return *this;
		}

		inline Quat& setZ(float arg_z)
		{
			z = arg_z;
			return *this;
		}

		inline Quat& setW(float arg_w)
		{
			w = arg_w;
			return *this;
		}

		inline float getX()const {
			return x;
		}

		inline float getY()const {
			return y;
		}

		inline float getZ()const {
			return z;
		}

		inline float getW()const {
			return w;
		}

		inline Quat& SetElem(int idx, float value) {

			*(&x + idx) = value;
			return *this;

		}

		inline float GetElem(int idx) const {
			return *(&x + idx);
		}

		inline float& operator [](int idx)
		{
			return *(&x + idx);
		}


		inline float operator [](int idx) const
		{
			return *(&x + idx);
		}


		inline const Quat operator +(const Quat& quat) const
		{
			return (Quat)XMVectorAdd(*this, quat);
		}


		inline const Quat operator -(const Quat& quat) const
		{
			return (Quat)XMVectorSubtract(*this, quat);
		}


		inline const Quat operator *(const Quat& quat) const
		{
			return (Quat)XMQuaternionMultiply(*this, quat);
		}

		inline const Quat operator *(float val) const
		{
			Quat temp(val, val, val, val);
			return (Quat)XMVectorMultiply(*this, temp);
		}

		inline const Quat& CreateFromAxisRotate(const Vector3& arg_axis,const  float angle) {
			*this = XMQuaternionRotationAxis(arg_axis, angle);
			return *this;
		}

		inline Matrix4x4 ToMatrix() {
			return Matrix4x4(*this);
		}
		inline Quat& CreateFromEular(const Vector3& arg_eular) {

			Matrix4x4 rotationMatrix= DirectX::XMMatrixRotationX(
				arg_eular.x
			) *
				DirectX::XMMatrixRotationY(
					arg_eular.y
				) *
				DirectX::XMMatrixRotationZ(
					arg_eular.z
				);
			*this = rotationMatrix.ToQuat();
			return *this;
		}

		inline Quat& operator *=(const Quat& quat) {
			*this = *this * quat;
			return *this;
		}

		inline Quat& Normalize() {
			*this = (Quat)XMQuaternionNormalize(*this);
			return *this;
		}

		inline float Dot(const Quat& quat)const {
			return ((Quat)XMQuaternionDot(*this, quat)).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	相手との共役を返す
		@param[in]	quat	相手
		@return	*thisの参照
		*/
		//--------------------------------------------------------------------------------------
		inline Quat& conj(const Quat& quat) {
			*this = (Quat)XMQuaternionConjugate(quat);
			return *this;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	単位クオータニオンを設定する
		@return	*thisの参照
		*/
		//--------------------------------------------------------------------------------------
		inline  Quat& identity() {
			*this = (Quat)XMQuaternionIdentity();
			return *this;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	X軸回転のクオータニオンを設定する
		@param[in]	radians 回転値
		@return	*thisの参照
		*/
		//--------------------------------------------------------------------------------------
		inline Quat& rotationX(float radians) {
			*this = (Quat)XMQuaternionRotationAxis(XMVECTOR(Vector3(1, 0, 0)), radians);
			return *this;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Y軸回転のクオータニオンを設定する
		@param[in]	radians 回転値
		@return	*thisの参照
		*/
		//--------------------------------------------------------------------------------------
		inline Quat& rotationY(float radians) {
			*this = (Quat)XMQuaternionRotationAxis(Vector3(0, 1, 0), radians);
			return *this;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Z軸回転のクオータニオンを設定する
		@param[in]	radians 回転値
		@return	*thisの参照
		*/
		//--------------------------------------------------------------------------------------
		inline Quat& rotationZ(float radians) {
			*this = (Quat)XMQuaternionRotationAxis(Vector3(0, 0, 1), radians);
			return *this;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定の軸回転のクオータニオンを設定する
		@param[in]	radians 回転値
		@param[in]	unitVec 回転軸
		@return	*thisの参照
		*/
		//--------------------------------------------------------------------------------------
		inline Quat& rotation(float radians, const Vector3& unitVec) {
			*this = (Quat)XMQuaternionRotationAxis(unitVec, radians);
			return *this;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定の軸回転のクオータニオンを設定する(引数が逆バージョン)
		@param[in]	unitVec 回転軸
		@param[in]	radians 回転値
		@return	*thisの参照
		*/
		//--------------------------------------------------------------------------------------
		inline Quat& rotation(const Vector3& unitVec, float radians) {
			*this = (Quat)XMQuaternionRotationAxis(unitVec, radians);
			return *this;
		}

		/*!
		@brief	回転ベクトルからクオータニオンを設定する
		@param[in]	rotVec 回転ベクトル
		@return	*thisの参照
		*/
		//--------------------------------------------------------------------------------------
		inline Quat& rotationRollPitchYawFromVector(const Vector3& rotVec) {
			*this = (Quat)XMQuaternionRotationRollPitchYawFromVector(rotVec);
			return *this;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クォータニオンを回転ベクトルに変換して返す
		@return	演算後の値
		*/
		//--------------------------------------------------------------------------------------
		inline const Vector3 toRotVec() const {
			Quat Temp = *this;
			Temp.Normalize();
			Matrix4x4 mt(Temp);
			Vector3 Rot;
			if (mt._32 == 1.0f) {
				Rot.x = XM_PI / 2.0f;
				Rot.y = 0;
				Rot.z = -atan2(mt._21, mt._11);
			}
			else if (mt._32 == -1.0f) {
				Rot.x = -XM_PI / 2.0f;
				Rot.y = 0;
				Rot.z = -atan2(mt._21, mt._11);
			}
			else {
				Rot.x = -asin(mt._32);
				Rot.y = -atan2(-mt._31, mt._33);
				Rot.z = atan2(mt._12, mt._11);
			}
			return Rot;
		}


		//--------------------------------------------------------------------------------------
		/*!
		@brief	*thisの逆クォータニオンを設定する
		@return	*thisの参照
		*/
		//--------------------------------------------------------------------------------------
		inline Quat& inverse() {
			*this = (Quat)XMQuaternionInverse(*this);
			return *this;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定した法線の方向を向く回転を設定する
		@param[in]	norm 法線
		@return	*thisの参照
		*/
		//--------------------------------------------------------------------------------------
		inline Quat& facing(const Vector3& norm) {
			Vector3 DefUp(0, 1.0f, 0);
			Vector3 Temp = norm;
			Vector2 TempVec2(Temp.x, Temp.z);
			if (MathHelper::Length(TempVec2) < 0.1f) {
				DefUp = Vector3(0, 0, 1.0f);
			}
			Temp.Normalize();
			Matrix4x4 RotMatrix;
			RotMatrix = XMMatrixLookAtLH(Vector3(0, 0, 0), Temp, DefUp);
			RotMatrix = MathHelper::inverse(RotMatrix);
			*this = RotMatrix.ToQuat();
			(*this).Normalize();
			return *this;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定した法線のY軸のみ方向を向く回転を設定する
		@param[in]	norm 法線
		@return	*thisの参照
		*/
		//--------------------------------------------------------------------------------------
		inline Quat& facingY(const Vector3& norm) {
			Vector3 Temp = norm;
			Temp.Normalize();
			*this = XMQuaternionRotationRollPitchYaw(0, atan2(Temp.x, Temp.z), 0);
			(*this).Normalize();
			return *this;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	*thisにNaNが含まれるかどうかを返す
		@return	含まれていたらtrue
		*/
		//--------------------------------------------------------------------------------------
		inline bool isNaN() const {
			return XMQuaternionIsNaN(*this);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	*thisに無限大が含まれるかどうかを返す
		@return	含まれていたらtrue
		*/
		//--------------------------------------------------------------------------------------
		inline bool isInfinite() const {
			return XMQuaternionIsInfinite(*this);
		}
	};

	struct Line {
		Vector3 point;
		Vector3 velocity;
		Line() {}
		Line(const Vector3& p, const Vector3& v) :point(p), velocity(v) {}

		// 線上の座標を取得
		//  t: ベクトルに掛け算する係数
		inline virtual Vector3 GetPoint(float t) const {
			return point + velocity * t;
		}
	};

	struct Segment : public Line {
		Segment() {}
		Segment(const Vector3& startPoint, const Vector3& endPoint) :Line(startPoint, endPoint - startPoint), endPos(endPoint) {}

		// 終点を取得
		inline Vector3 GetEndPoint() const {
			return endPos;
		}
		inline Vector3 GetPoint(float t) const override {
			return(endPos - point) * t;
		}
		Vector3 endPos;
	};

	struct QuadraticBezierCurve {
	public:
		QuadraticBezierCurve(const Vector3& pointA, const Vector3& pointB, const Vector3& pointC) :lineAB(pointA, pointB), lineBC(pointB, pointC) {}
		inline Vector3 GetPoint(float t) {
			quadratic.point = lineAB.GetPoint(t);
			quadratic.endPos = lineBC.GetPoint(t);

			return quadratic.GetPoint(t);
		}
	private:
		Segment lineAB;
		Segment lineBC;
		Segment quadratic;
	};
	struct CubicBezierCurve {
	public:
		CubicBezierCurve(const Vector3& pointA, const Vector3& pointB, const Vector3& pointC, const Vector3& pointD) :curveABC(pointA, pointB, pointC), curveBCD(pointB, pointC, pointD) {}

		inline Vector3 GetPoint(float t) {
			cubic.point = curveABC.GetPoint(t);
			cubic.endPos = curveBCD.GetPoint(t);

			return cubic.GetPoint(t);
		}
	private:
		QuadraticBezierCurve curveABC;
		QuadraticBezierCurve curveBCD;
		Segment cubic;
	};

	struct Line2D {
		Vector2 point;
		Vector2 velocity;
		Line2D() {}
		Line2D(const Vector2& p, const Vector2& v) :point(p), velocity(v) {}

		// 線上の座標を取得
		//  t: ベクトルに掛け算する係数
		inline virtual Vector2 GetPoint(float t) const {
			return point + velocity * t;
		}
	};

	struct Segment2D : public Line2D {
		Segment2D() {}
		Segment2D(const Vector2& startPoint, const Vector2& endPoint) :Line2D(startPoint, endPoint - startPoint), endPos(endPoint) {}

		// 終点を取得
		inline Vector2 GetEndPoint() const {
			return endPos;
		}
		inline Vector2 GetPoint(float t) const override {
			return point + (endPos - point) * t;
		}
		Vector2 endPos;
	};

	struct QuadraticBezierCurve2D {
	public:
		QuadraticBezierCurve2D() {}
		QuadraticBezierCurve2D(const Vector2& pointA, const Vector2& pointB, const Vector2& pointC) :lineAB(pointA, pointB), lineBC(pointB, pointC) {}
		inline Vector2 GetPoint(float t) {
			quadratic.point = lineAB.GetPoint(t);
			quadratic.endPos = lineBC.GetPoint(t);

			return quadratic.GetPoint(t);
		}
		Segment2D lineAB;
		Segment2D lineBC;
	private:
		Segment2D quadratic;
	};
	struct CubicBezierCurve2D {
	public:
		CubicBezierCurve2D(const Vector2& pointA, const Vector2& pointB, const Vector2& pointC, const Vector2& pointD)
		{
			curveABC = QuadraticBezierCurve2D(pointA, pointB, pointC);
			curveBCD = QuadraticBezierCurve2D(pointB, pointC, pointD);
		}

		inline Vector2 GetPoint(float t) {
			cubic.point = curveABC.GetPoint(t);
			cubic.endPos = curveBCD.GetPoint(t);

			return cubic.GetPoint(t);
		}

		inline float GetDerivative(float t) {
			return (3.0f * curveABC.lineAB.point.x * t + 2.0f * curveABC.lineAB.endPos.x) * t + curveABC.lineBC.endPos.x;
		}

		inline float GetYFromNuton(float x) {
			float epsilon = 1e-5f; // 閾値
			float x2, t0, t1, t2, d2, i;
			for (t2 = x, i = 0; i < 16; i++) {
				x2 = GetPoint(t2).x - x;
				if (abs(x2) < epsilon) {
					return GetPoint(t2).y;
				}
				d2 = GetDerivative(t2);
				if (abs(d2) < 1e-6f) {
					break;
				}
				t2 = t2 - x2 / d2;
			}
			t0 = 0.0f;
			t1 = 1.0f;
			t2 = x;
			if (t2 < t0) {
				return GetPoint(t0).y;
			}
			if (t2 > t1) {
				return GetPoint(t1).y;
			}
			while (t0 < t1) {
				x2 = GetPoint(t2).x;
				if (abs(x2 - x) < epsilon) {
					return GetPoint(t2).y;
				}
				if (x > x2) {
					t0 = t2;
				}
				else {
					t1 = t2;
				}
				t2 = (t1 - t0) * 0.5f + t0;
			}


			return  GetPoint(t2).y;
		}

		inline float GetYFromHalf(const float x,const char testCount=12) {

			Vector2 a = curveABC.lineBC.endPos; 
			Vector2 b = curveABC.lineAB.endPos;
			if (a.x == a.y && b.x == b.y) {
				return x;
			}
			float t = x;

			const float k0 = 1 + 3 * a.x - 3 * b.x;
			const float k1 = 3 * b.x - 6 * a.x;
			const float k2 = 3 * a.x;

			float epsilon = 0.0005f; // 閾値

			for (int i = 0; i < testCount; i++) {
				auto ft = k0 * t * t * t + k1 * t * t + k2 * t - x;

				if (ft <= epsilon && ft >= -epsilon) {
					break;
				}
				t -= t / 2;
			}
			auto r = 1 - t;

			return t * t * t + 3 * t * t * r * b.y + 3 * t * r * r * a.y;
		}
	private:
		QuadraticBezierCurve2D curveABC;
		QuadraticBezierCurve2D curveBCD;
		Segment2D cubic;
	};

	////////////////////////////////////////////////////

	inline Vector4 ButiEngine::Matrix4x4::operator*(const Vector4& vec4)
	{
		return XMVector4Transform(vec4, *this);
	}

	inline ButiEngine::Matrix4x4::Matrix4x4(const Quat& quat)
	{
		if (!quat.isNaN())
			*this = (Matrix4x4)XMMatrixRotationQuaternion(XMVECTOR(quat));
		else {
			_11 = 1.0f;
			_12 = 0.0f;
			_13 = 0.0f;
			_14 = 0.0f;

			_21 = 0.0f;
			_22 = 1.0f;
			_23 = 0.0f;
			_24 = 0.0f;

			_31 = 0.0f;
			_32 = 0.0f;
			_33 = 1.0f;
			_34 = 0.0f;

			_41 = 0.0f;
			_42 = 0.0f;
			_43 = 0.0f;
			_44 = 1.0f;
		}
	}

	inline Quat ButiEngine::Matrix4x4::ToQuat() const
	{
		Vector3 scale, pos;
		Quat qt;
		Decompose(scale, qt, pos);
		return qt;
	}
	inline void ButiEngine::Matrix4x4::Decompose(Vector3& rScaling, Quat& rQt, Vector3& rTranslation) const
	{
		XMVECTOR Scale;
		XMVECTOR Qt;
		XMVECTOR Translation;
		if (XMMatrixDecompose(&Scale, &Qt, &Translation, *this)) {
			rScaling = Scale;
			rQt = Qt;
			rTranslation = Translation;
		}
		else {
			//スケーリングマイナスの場合の処理
			auto XLen = MathHelper::Length(Vector3(_11, _12, _13));
			auto YLen = MathHelper::Length(Vector3(_21, _22, _23));
			auto ZLen = MathHelper::Length(Vector3(_31, _32, _33));
			//スケーリングが0の要素がある
			if (XLen == 0.0f) {
				XLen = 0.0001f;
			}
			if (YLen == 0.0f) {
				YLen = 0.0001f;
			}
			if (ZLen == 0.0f) {
				ZLen = 0.0001f;
			}
			rScaling = Vector3(XLen, YLen, ZLen);
			rTranslation = Vector3(_41, _42, _43);

			Vector3 vX = Vector3(_11, _12, _13) / XLen;
			Vector3 vY = Vector3(_21, _22, _23) / XLen;
			Vector3 vZ = Vector3(_31, _32, _33) / XLen;

			Matrix4x4 retM;
			retM.Identity();
			retM._11 = vX.x;
			retM._12 = vX.y;
			retM._13 = vX.z;

			retM._21 = vY.x;
			retM._22 = vY.y;
			retM._23 = vY.z;

			retM._31 = vZ.x;
			retM._32 = vZ.y;
			retM._33 = vZ.z;

			rQt = (Quat)XMQuaternionRotationMatrix(retM);

		}
	}

	static const float EulerXLimit = 90.0f / 180.0f * XM_PI;
	inline Vector3 Matrix4x4::GetEuler(const bool arg_reverse) const
	{

		if (fabs(this->m[0][2] - 1) > 0.00001 && fabs(this->m[0][2] + 1) > 0.00001) {

			float yTheta = asin(-this->m[0][2]);///-90~90


			if (!arg_reverse)
			{
				auto yThetaCos = cos(yTheta);

				float xTheta = atan2(this->_23 / yThetaCos, this->m[2][2] / yThetaCos);///-90~90


				float zTheta = atan2(this->m[0][1] / yThetaCos, this->m[0][0] / yThetaCos);///-90~90

				return Vector3(xTheta, yTheta, zTheta);
			}
			else {
				float yTheta2 = XM_PI - yTheta;        ///90~180,-180~-90

				auto yThetaCos = cos(yTheta2);
				float xTheta2 = atan2(this->_23 / yThetaCos, this->m[2][2] / yThetaCos);///90~180,-180~-90));
				float zTheta2 = atan2(this->m[0][1] / yThetaCos, this->m[0][0] / yThetaCos);///90~180,-180~-90
				return Vector3(xTheta2, yTheta2, zTheta2);
			}
		}
		else {
			float xTheta = 0;
			float yTheta = 0;
			float zTheta = 0;
			if (abs(this->m[0][2] - 1) < 0.00001) {
				yTheta = -XM_PI / 2;
			}
			else {
				yTheta = XM_PI / 2;
			}
			auto ythetaCos = cos(yTheta);
			zTheta = atan2(this->m[0][1] / ythetaCos, this->m[0][0] / ythetaCos);
			xTheta = atan2(this->_23 / ythetaCos, this->m[2][2] / ythetaCos);
			return Vector3(xTheta, yTheta, zTheta);
		}


	}
	inline Vector3 Matrix4x4::GetEulerOneValue() const
	{

		if (fabs(this->m[0][2] - 1) > 0.00001 && fabs(this->m[0][2] + 1) > 0.00001) {

			float yTheta = asin(-this->m[0][2]);///-90~90


				auto yThetaCos = cos(yTheta);

				float xTheta = atan2(this->_23 / yThetaCos, this->m[2][2] / yThetaCos);///-90~90


				float zTheta = atan2(this->m[0][1] / yThetaCos, this->m[0][0] / yThetaCos);///-90~90


				Matrix4x4 test;
				test.CreateFromEuler(xTheta, yTheta, zTheta);

				if(this->Same(test))
				return  Vector3(xTheta, yTheta, zTheta);


			
				float yTheta2 = XM_PI - yTheta;        ///90~180,-180~-90

				yThetaCos = cos(yTheta2);
				float xTheta2 = atan2(this->_23 / yThetaCos, this->m[2][2] / yThetaCos);///90~180,-180~-90));
				float zTheta2 = atan2(this->m[0][1] / yThetaCos, this->m[0][0] / yThetaCos);///90~180,-180~-90
				return Vector3(xTheta2, yTheta2, zTheta2);
			
		}
		else {
			float xTheta = 0;
			float yTheta = 0;
			float zTheta = 0;
			if (abs(this->m[0][2] - 1) < 0.00001) {
				yTheta = -XM_PI / 2;
			}
			else {
				yTheta = XM_PI / 2;
			}
			auto ythetaCos = cos(yTheta);
			zTheta = atan2(this->m[0][1] / ythetaCos, this->m[0][0] / ythetaCos);
			xTheta = atan2(this->_23 / ythetaCos, this->m[2][2] / ythetaCos);
			return Vector3(xTheta, yTheta, zTheta);
		}


	}
	inline Vector3 Matrix4x4::GetEulerDegrees(const bool arg_reverse)const {
		auto outVec = GetEuler(arg_reverse);

		outVec.x = XMConvertToDegrees(outVec.x);
		outVec.y = XMConvertToDegrees(outVec.y);
		outVec.z = XMConvertToDegrees(outVec.z);

		return outVec;
	}


	typedef Vector4 Color;
	typedef Vector2 Point2D;

	class Rectangle {
	public:
		float width;
		float height;
		float outerCircleRadius;
		Vector2 position;
		inline Rectangle() :width(0), height(0), position(Vector2()), outerCircleRadius(0) {};
		inline Rectangle(float arg_width, float  arg_height, Vector2 arg_position) :width(arg_width), height(arg_height), position(arg_position), outerCircleRadius(GetRectangleOuterCircleRadius((int)width, (int)height)) {};
		inline Rectangle(float arg_width, float  arg_height, Vector2 arg_position, float arg_outerCircleRadius) :width(arg_width), height(arg_height), position(arg_position), outerCircleRadius(arg_outerCircleRadius) {};
		inline Rectangle(float arg_width, float  arg_height, float x, float y) :width(arg_width), height(arg_height), position(Vector2(x, y)), outerCircleRadius(GetRectangleOuterCircleRadius((int)width, (int)height)) {};
		inline std::vector< Point2D> GetVertecies()
		{
			std::vector<Point2D> out{
				   Point2D(position.x - width / 2, position.y - height / 2),
				   Point2D(position.x + width / 2, position.y - height / 2),
				   Point2D(position.x + width / 2, position.y + height / 2),
				   Point2D(position.x - width / 2, position.y + height / 2)
			};

			return out;
		}

		inline void GetVertecies(Point2D* out) {
			out = GetVertecies().data();
		}

		inline bool IsContain(Point2D arg_point2D) {
			if (arg_point2D.x >= position.x - width / 2 && arg_point2D.x <= position.x + width / 2
				&& arg_point2D.y >= position.y - height / 2 && arg_point2D.y <= position.y + height / 2) {
				return true;
			}
			return false;
		}
		inline bool IsIntersects(std::shared_ptr< Rectangle> other) {

			/*auto d = other->position.GetDistance(position);
			if (other->position.GetDistance(position) > other->outerCircleRadius + outerCircleRadius) {
				return false;
			}*/
			auto otherVertecies = other->GetVertecies();
			for (auto i = 0; i < 4; i++)
			{
				if (IsContain(otherVertecies.at(i))) {
					return true;
				}
			}
			return false;
		}
		inline float GetTop() {

			return position.y - height / 2;
		}
		inline float GetBottom() {

			return position.y + height / 2;
		}
		inline float GetLeft() {
			return position.x - width / 2;
		}
		inline float GetRight() {

			return position.x + width / 2;
		}
		inline static float GetRectangleOuterCircleRadius(const int width, const int height) {
			auto aW = width; auto aH = height;
			auto s = (aW + aH);
			auto t = (s - aW) * (s - aH) * (s - aW) * (s - aH);
			auto u = (aW * aH * 2) * (aW * aH * 2) * (aW * aW + aH * aH);


			auto output = std::sqrt(u / t) / 4;
			return output;
		}
		inline static float GetRectangleOuterCircleRadius(const Rectangle& ref_rect) {
			auto s = (ref_rect.width + ref_rect.height);
			auto t = (s - ref_rect.width) * (s - ref_rect.height) * (s - ref_rect.width) * (s - ref_rect.height);
			auto u = (ref_rect.width * ref_rect.height * 2) * (ref_rect.width * ref_rect.height * 2) * (ref_rect.width * ref_rect.width + ref_rect.height * ref_rect.height);
			auto output = std::sqrt(u / t) / 4;
			return output;
		}
	};




}