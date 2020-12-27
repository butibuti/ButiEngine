#pragma once
#include"ButiMath.h"
#include<memory>

namespace ButiEngine {

	class Transform :public IObject
	{

	public:

		inline Transform() {
			localMatrix = nullptr;
			rotation = Matrix4x4::RollX(
				DirectX::XMConvertToRadians(0)
			) *
				Matrix4x4::RollY(
					DirectX::XMConvertToRadians(0)
				) *
				Matrix4x4::RollZ(
					DirectX::XMConvertToRadians(0)
				);
		}
		inline Transform(const Vector3& arg_position, const Vector3& arg_rotate, const Vector3& arg_scale) {

			localPosition = arg_position;
			rotation = DirectX::XMMatrixRotationX(
				DirectX::XMConvertToRadians(arg_rotate.x)
			) *
				DirectX::XMMatrixRotationY(
					DirectX::XMConvertToRadians(arg_rotate.y)
				) *
				DirectX::XMMatrixRotationZ(
					DirectX::XMConvertToRadians(arg_rotate.z)
				);
			scale = arg_scale;
			localMatrix = nullptr;
		}
		inline Transform(const Vector3& arg_position, const Matrix4x4& arg_rotate, const Vector3& arg_scale) {

			localPosition = arg_position;
			rotation = arg_rotate;
			scale = arg_scale;
			localMatrix = nullptr;
		}
		inline Transform(const Vector3& pos) {
			localPosition = pos;
			rotation = DirectX::XMMatrixRotationX(
				DirectX::XMConvertToRadians(0)
			) *
				DirectX::XMMatrixRotationY(
					DirectX::XMConvertToRadians(0)
				) *
				DirectX::XMMatrixRotationZ(
					DirectX::XMConvertToRadians(0)
				);
			localMatrix = nullptr;
		}
		void Initialize()override {}
		void PreInitialize()override {}
		inline Matrix4x4 ToMatrix()
		{

			return GetMatrix().GetTranspose();


		}
		inline Matrix4x4 GetMatrix()
		{
			Matrix4x4 output = GetLocalMatrix();
			if (baseTransform != nullptr) {
				auto baseMatrix = baseTransform->GetMatrix();
				output = (XMMATRIX)output * (XMMATRIX)baseMatrix;
			}
			return output;
		}
		inline Matrix4x4 GetMatrix_WithoutScale()
		{
			Matrix4x4 output = rotation;
			output._41 = localPosition.x;
			output._42 = localPosition.y;
			output._43 = localPosition.z;
			if (baseTransform != nullptr) {
				auto baseMatrix = baseTransform->GetMatrix_WithoutScale();
				output = (XMMATRIX)output * (XMMATRIX)baseMatrix;
			}

			return output;
		}
		inline Matrix4x4 GetTranslateMatrix()
		{
			Matrix4x4 output;
			Vector3 world = GetWorldPosition();

			output._41 = world.x;
			output._42 = world.y;
			output._43 = world.z;

			return output;
		}

		inline const Matrix4x4 GetLocalMatrix()
		{
			if (localMatrix)
			{
				return *localMatrix;
			}
			localMatrix = std::make_shared<Matrix4x4>();
			{

				*localMatrix = Matrix4x4().Scale(scale)*rotation
					
					;
				localMatrix->_41 = localPosition.x;
				localMatrix->_42 = localPosition.y;
				localMatrix->_43 = localPosition.z;

			}

			return *localMatrix;

		}

		std::shared_ptr<Transform> Clone() {
			auto output = ObjectFactory::Create<Transform>(localPosition,rotation,scale);
			if(baseTransform)
			output->SetBaseTransform(baseTransform, true);
			return output;
		}

		inline Vector3 GetWorldPosition()
		{
			return Vector3(GetMatrix()._41, GetMatrix()._42, GetMatrix()._43);
		}
		inline const Matrix4x4& GetLocalRotation()const
		{
			return rotation;
		}

		inline Matrix4x4 GetWorldRotation()const
		{
			auto output = rotation;
			if (baseTransform == nullptr) {
				return output;
			}
			output = (XMMATRIX)output * baseTransform->GetWorldRotation();
			return output;
		}

		inline const Matrix4x4& SetLocalRotationIdentity() {
			if (localMatrix) {
				localMatrix = nullptr;
			}
			return rotation = Matrix4x4();
		}

		inline const Matrix4x4& SetLocalRotation(const Matrix4x4& arg_rotation) {

			if (localMatrix) {
				localMatrix = nullptr;
			}
			return rotation = arg_rotation;
		}
		inline const Matrix4x4& SetWorldRotation(const Matrix4x4& arg_rotation) {

			if (localMatrix) {
				localMatrix = nullptr;
			}

			if (baseTransform) {
				rotation = arg_rotation * baseTransform->GetWorldRotation().GetInverse();
				return rotation;
			}else

			return rotation = arg_rotation;
		}
		inline const Matrix4x4& SetLocalRotation(const Vector3& arg_vec3_rotation) {

			if (localMatrix) {
				localMatrix = nullptr;
			}
			return rotation = Matrix4x4::RollX(
				DirectX::XMConvertToRadians(arg_vec3_rotation.x)
			) *
				Matrix4x4::RollY(
					DirectX::XMConvertToRadians(arg_vec3_rotation.y)
				) *
				Matrix4x4::RollZ(
					DirectX::XMConvertToRadians(arg_vec3_rotation.z)
				);
		}
		inline const Matrix4x4& RollLocalRotationX_Degrees(const float arg_x) {
			return RollLocalRotationX_Radian(XMConvertToRadians(arg_x));

		}
		inline const Matrix4x4& RollLocalRotationX_Radian(const float arg_x) {

			if (localMatrix) {
				localMatrix = nullptr;
			}
			return  rotation = Matrix4x4::RollX(
				arg_x
			) * rotation;

		}
		inline const Matrix4x4& RollWorldRotationX_Degrees(const float arg_x) {

			return RollWorldRotationX_Radian(XMConvertToRadians(arg_x));

		}
		inline const Matrix4x4& RollWorldRotationX_Radian(const float arg_x) {

			if (localMatrix) {
				localMatrix = nullptr;
			}
			return rotation = rotation * Matrix4x4::RollX(
				arg_x
			);

		}

		inline const Matrix4x4& RollLocalRotationY_Degrees(const float arg_y) {
			return RollLocalRotationY_Radian(XMConvertToRadians(arg_y));

		}
		inline const Matrix4x4& RollLocalRotationY_Radian(const float arg_y) {

			if (localMatrix) {
				localMatrix = nullptr;
			}
			return  rotation = Matrix4x4::RollY(
				arg_y
			) * rotation;

		}
		inline const Matrix4x4& RollWorldRotationY_Degrees(const float arg_y) {

			return RollWorldRotationY_Radian(XMConvertToRadians(arg_y));

		}
		inline const Matrix4x4& RollWorldRotationY_Radian(const float arg_y) {

			if (localMatrix) {
				localMatrix = nullptr;
			}
			return rotation = rotation * Matrix4x4::RollY(
				arg_y
			);

		}

		inline const Matrix4x4& RollLocalRotationZ_Degrees(const float arg_z) {
			return RollLocalRotationZ_Radian(XMConvertToRadians(arg_z));

		}
		inline const Matrix4x4& RollLocalRotationZ_Radian(const float arg_z) {

			if (localMatrix) {
				localMatrix = nullptr;
			}
			return  rotation = Matrix4x4::RollZ(
				arg_z
			) * rotation;

		}
		inline const Matrix4x4& RollWorldRotationZ_Degrees(const float arg_z) {

			return RollWorldRotationZ_Radian(XMConvertToRadians(arg_z));

		}
		inline const Matrix4x4& RollWorldRotationZ_Radian(const float arg_z) {

			if (localMatrix) {
				localMatrix = nullptr;
			}
			return rotation = rotation * Matrix4x4::RollZ(
				arg_z
			);

		}
		inline const Matrix4x4& RollLocalRotation(const Vector3& arg_vec3_rotation)
		{
			if (localMatrix) {
				localMatrix = nullptr;
			}
			return rotation = DirectX::XMMatrixRotationX(
				DirectX::XMConvertToRadians(arg_vec3_rotation.x)
			) *
				DirectX::XMMatrixRotationY(
					DirectX::XMConvertToRadians(arg_vec3_rotation.y)
				) *
				DirectX::XMMatrixRotationZ(
					DirectX::XMConvertToRadians(arg_vec3_rotation.z)
				) * (XMMATRIX)rotation;
		}
		inline const Matrix4x4& RollWorldBase(const Vector3& arg_vec3_rotation)
		{
			if (localMatrix) {
				localMatrix = nullptr;
			}
			return rotation = (XMMATRIX)rotation * DirectX::XMMatrixRotationX(
				DirectX::XMConvertToRadians(arg_vec3_rotation.x)
			) *
				DirectX::XMMatrixRotationY(
					DirectX::XMConvertToRadians(arg_vec3_rotation.y)
				) *
				DirectX::XMMatrixRotationZ(
					DirectX::XMConvertToRadians(arg_vec3_rotation.z)
				);
		}
		inline const Matrix4x4& RollWorldRotation(const Quat& arg_rotation) {
			if (localMatrix) {
				localMatrix = nullptr;
			}
			if (baseTransform) {
				rotation *=  arg_rotation.ToMatrix()* baseTransform->GetWorldRotation().GetInverse();
			}else
			rotation *= arg_rotation.ToMatrix();

			return rotation;
		}

		inline void RollIdentity() {
			if (localMatrix) {
				localMatrix = nullptr;
			}
			rotation.Identity();
		}

		inline const Vector3& Translate(const Vector3& arg_velocity) {
			if (localMatrix) {
				localMatrix->_41 += arg_velocity.x;
				localMatrix->_42 += arg_velocity.y;
				localMatrix->_43 += arg_velocity.z;
			}

			return  localPosition+=arg_velocity;
		}
		inline const Vector3& TranslateX(const float arg_moveX) {
			if (localMatrix) {
				localMatrix->_41 += arg_moveX;
			}
			localPosition.x += arg_moveX;
			return  localPosition;
		}

		inline const Vector3& TranslateY(const float arg_moveY) {
			if (localMatrix) {
				localMatrix->_42 += arg_moveY;
			}
			localPosition.y += arg_moveY;
			return  localPosition;
		}

		inline const Vector3& TranslateZ(const float arg_moveZ) {
			if (localMatrix) {
				localMatrix->_43 += arg_moveZ;
			}
			localPosition.z += arg_moveZ;
			return  localPosition;
		}


		inline const Matrix4x4& RollLocalRotation(const Matrix4x4& arg_rotation) {
			if (localMatrix) {
				localMatrix = nullptr;
			}
			rotation = (XMMATRIX)rotation * (XMMATRIX)arg_rotation;


			return rotation;
		}

		inline const Matrix4x4& SetLookAtRotation(const Vector3& arg_targetPos, const Vector3& arg_upAxis = Vector3(0, 1, 0)) {

			Vector3 z = ((Vector3)(arg_targetPos - GetWorldPosition())).GetNormalize();
			Vector3 x = arg_upAxis.GetCross(z).GetNormalize();
			Vector3 y = z.GetCross(x).GetNormalize();

			rotation = Matrix4x4();
			rotation._11 = x.x; rotation._12 = x.y; rotation._13 = x.z;
			rotation._21 = y.x; rotation._22 = y.y; rotation._23 = y.z;
			rotation._31 = z.x; rotation._32 = z.y; rotation._33 = z.z;
			if (localMatrix) {
				localMatrix = nullptr;
			}

			return rotation;
		}
		inline  Matrix4x4 GetLookAtRotation(const Vector3& arg_targetPos, const Vector3& arg_upAxis) {
			Vector3 z = ((Vector3)(arg_targetPos - GetWorldPosition())).GetNormalize();
			Vector3 x = arg_upAxis.GetCross(z).GetNormalize();
			Vector3 y = z.GetCross(x).GetNormalize();

			auto out = Matrix4x4();
			out._11 = x.x; out._12 = x.y; out._13 = x.z;
			out._21 = y.x; out._22 = y.y; out._23 = y.z;
			out._31 = z.x; out._32 = z.y; out._33 = z.z;


			return out;
		}

		inline Matrix4x4& SetLocalRotation()
		{
			if (localMatrix) {
				localMatrix = nullptr;
			}
			return rotation;
		}
		inline const Vector3& GetLocalPosition()const
		{
			return localPosition;
		}
		inline const Vector3& SetLocalPosition(const Vector3& arg_position) {
			if (localMatrix) {
				localMatrix->_41 = arg_position.x;
				localMatrix->_42 = arg_position.y;
				localMatrix->_43 = arg_position.z;
			}
			return  localPosition = arg_position;
		}
		inline const Vector3& SetWorldPosition(const Vector3& arg_position) {
			if (localMatrix) {
				localMatrix = nullptr;
			}
			if (!baseTransform)
				return  localPosition = arg_position;

			return  localPosition = arg_position - baseTransform->GetWorldPosition();
		}


		inline Vector3& SetLocalPosition() {
			if (localMatrix) {
				localMatrix = nullptr;

			}
			return  localPosition;
		}

		inline const Vector3& GetLocalScale() const
		{
			return scale;
		}
		inline  Matrix4x4 GetLocalScaleMatrix() const {
			return DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		}
		inline Vector3 GetWorldScale() const
		{
			auto out = scale;
			if (baseTransform) {
				out*= baseTransform->GetWorldScale();
			}
			return out;
		}

		inline const Vector3& SetLocalScale(const Vector3& arg_scale) {
			if (localMatrix) {
				localMatrix = nullptr;
			}
			return  scale = arg_scale;
		}
		inline Vector3& SetLocalScale() {
			if (localMatrix) {
				localMatrix = nullptr;
			}
			return  scale;
		}

		inline Vector3 GetFront() {
			return Vector3::ZAxis* GetWorldRotation();
		}

		inline Vector3 GetRight() {
			return  Vector3::XAxis * GetWorldRotation();

		}

		inline Vector3 GetUp() {
			return  Vector3::YAxis  * GetWorldRotation();
		}

		inline void GetRotatedVector(Vector3& arg_vector3) {
			arg_vector3 *= GetWorldRotation();
		}
		inline void SetBaseTransform(std::shared_ptr<Transform> argParent, const bool arg_isKeepLocalPosition = false)
		{
			baseTransform = argParent;
			if (argParent == nullptr)
				return;

			if (!arg_isKeepLocalPosition) {
				localPosition = localPosition - baseTransform->GetWorldPosition();
				if (localMatrix) {
					localMatrix->_41 = localPosition.x;
					localMatrix->_42 = localPosition.y;
					localMatrix->_43 = localPosition.z;
				}
			}
		}
		inline std::shared_ptr<Transform> GetBaseTransform()
		{
			return baseTransform;
		}


		virtual bool ShowUI();

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(localPosition);
			archive(scale);
			archive(rotation);
			archive(baseTransform);

		}

	protected:
		Vector3 localPosition = Vector3(0.0f, 0.0f, 0.0f);
		Matrix4x4 rotation;
		Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);
		std::shared_ptr< Matrix4x4> localMatrix = nullptr;
		std::shared_ptr<Transform> baseTransform = nullptr;
	};

	class BoneTransform :public Transform {
	public:
		inline BoneTransform() :Transform() {
		}
		inline BoneTransform(const Vector3& arg_position, const Vector3& arg_rotate, const Vector3& arg_scale)
			: Transform(arg_position, arg_rotate, arg_scale) {

		}
		inline BoneTransform(const Vector3& arg_pos)
			: Transform(arg_pos) {
		}

		inline Matrix4x4 GetBoneMatrix()
		{
			Matrix4x4 output = GetLocalMatrix();
			if (parentBoneTransform != nullptr) {
				auto baseMatrix = parentBoneTransform->GetBoneMatrix();
				output = (XMMATRIX)output * (XMMATRIX)baseMatrix;
			}
			return output;
		}

		inline Vector3 GetBonePosition()
		{
			Matrix4x4 output = GetLocalMatrix();
			if (parentBoneTransform != nullptr) {
				auto baseMatrix = parentBoneTransform->GetBoneMatrix();
				output = (XMMATRIX)output * (XMMATRIX)baseMatrix;
			}
			return Vector3( output._41, output._42, output._43);
		}
		inline void SetParentTransform(std::shared_ptr<BoneTransform> arg_Parent, const bool arg_isKeepLocalPosition = false)
		{
			parentBoneTransform = arg_Parent;
			if (!arg_isKeepLocalPosition) {

				localPosition = localPosition - parentBoneTransform->GetWorldPosition();
				if (localMatrix) {
					localMatrix->_41 = localPosition.x;
					localMatrix->_42 = localPosition.y;
					localMatrix->_43 = localPosition.z;
				}
			}
		}
		bool ShowUI()override;
	private:
		std::shared_ptr<BoneTransform> parentBoneTransform = nullptr;
	};


	void OutputCereal(std::shared_ptr<Transform>& v, const std::string& path);

	void InputCereal(std::shared_ptr<Transform>& v, const std::string& path);
}

