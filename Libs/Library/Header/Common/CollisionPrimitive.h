#pragma once
#include"stdafx.h"
namespace ButiEngine {
	namespace Collision {
		class CollisionPrimitive_Point;
		class CollisionPrimitive_Sphere;
		class CollisionPrimitive_Box_AABB;
		class CollisionPrimitive_Box_OBB;
		class CollisionPrimitive :public IObject
		{
		public:
			void Initialize() override {}
			void PreInitialize()override {}
			virtual void Update() = 0;
			virtual bool IsHit(std::weak_ptr< CollisionPrimitive> other) = 0;

			virtual bool IsHitPoint(CollisionPrimitive_Point* other) = 0;
			virtual bool IsHitSphere(CollisionPrimitive_Sphere* other) = 0;
			virtual bool IsHitBox_AABB(CollisionPrimitive_Box_AABB* other) = 0;
			virtual bool IsHitBox_OBB(CollisionPrimitive_Box_OBB* other) = 0;
			virtual void GetMaxPointAndMinPoint(Vector3& arg_outputMax, Vector3& arg_outputMin) const = 0;
			std::weak_ptr<Transform> wkp_transform;
		};
		class CollisionPrimitive_Point :public CollisionPrimitive
		{
		public:
			inline CollisionPrimitive_Point(std::weak_ptr<Transform> arg_weak_transform)
			{
				wkp_transform = arg_weak_transform;
			}
			inline void Update()override {
			}
			inline Vector3 GetPosition() {
				return wkp_transform.lock()->GetWorldPosition();
			}
			inline bool IsHit(std::weak_ptr< CollisionPrimitive> other)override
			{
				return other.lock()->IsHitPoint(this);


			}
			inline void GetMaxPointAndMinPoint(Vector3& arg_outputMax, Vector3& arg_outputMin) const override {
				auto point = wkp_transform.lock()->GetWorldPosition();
				arg_outputMax = point;
				arg_outputMin = point;
			}
			bool IsHitPoint(CollisionPrimitive_Point* other)override;
			bool IsHitSphere(CollisionPrimitive_Sphere* other)override;
			bool IsHitBox_AABB(CollisionPrimitive_Box_AABB* other)override;
			bool IsHitBox_OBB(CollisionPrimitive_Box_OBB* other)override;
		private:
		};
		class CollisionPrimitive_Sphere :public CollisionPrimitive, public Geometry::Sphere
		{
		public:
			inline CollisionPrimitive_Sphere(const float arg_radius, std::weak_ptr<Transform> arg_weak_transform)
				:Geometry::Sphere(arg_radius) {
				wkp_transform = arg_weak_transform;
			}
			inline void Update()override {
				position = wkp_transform.lock()->GetWorldPosition();
			}
			inline bool IsHit(std::weak_ptr< CollisionPrimitive> other)override
			{
				return other.lock()->IsHitSphere(this);


			}
			inline void GetMaxPointAndMinPoint(Vector3& arg_outputMax, Vector3& arg_outputMin) const {
				arg_outputMax = position + Vector3(radius, radius, radius);
				arg_outputMin = position - Vector3(radius, radius, radius);
			}
			bool IsHitPoint(CollisionPrimitive_Point* other)override;
			bool IsHitSphere(CollisionPrimitive_Sphere* other)override;
			bool IsHitBox_AABB(CollisionPrimitive_Box_AABB* other)override;
			bool IsHitBox_OBB(CollisionPrimitive_Box_OBB* other)override;
		private:
		};
		class CollisionPrimitive_Box_AABB : public CollisionPrimitive, public Geometry::Box_AABB {
		public:
			inline CollisionPrimitive_Box_AABB(const Vector3& arg_length, std::weak_ptr<Transform> arg_weak_transform)
				:Geometry::Box_AABB(arg_length) {
				wkp_transform = arg_weak_transform;
				initLengthes = arg_length / 2;
			}

			inline void Update()override {
				position = wkp_transform.lock()->GetWorldPosition();
				halfLengthes = initLengthes * wkp_transform.lock()->GetWorldScale();
			}
			inline bool IsHit(std::weak_ptr< CollisionPrimitive> other)override {
				return other.lock()->IsHitBox_AABB(this);
			}

			inline void GetMaxPointAndMinPoint(Vector3& arg_outputMax, Vector3& arg_outputMin) const {
				arg_outputMax = position + halfLengthes;
				arg_outputMin = position - halfLengthes;
			}
			bool IsHitPoint(CollisionPrimitive_Point* other)override;
			bool IsHitSphere(CollisionPrimitive_Sphere* other)override;
			bool IsHitBox_AABB(CollisionPrimitive_Box_AABB* other)override;
			bool IsHitBox_OBB(CollisionPrimitive_Box_OBB* other)override;
		private:
			Vector3 initLengthes;
		};
		class CollisionPrimitive_Box_OBB : public CollisionPrimitive, public Geometry::Box_OBB_Static {
		public:
			inline CollisionPrimitive_Box_OBB(const Vector3& arg_length, std::weak_ptr<Transform> arg_weak_transform)
				:Geometry::Box_OBB_Static(arg_length) {
				wkp_transform = arg_weak_transform;
				initLengthes = arg_length / 2;
			}

			inline void Update()override {
				position = wkp_transform.lock()->GetWorldPosition();

				directs[0] = wkp_transform.lock()->GetRight();
				directs[1] = wkp_transform.lock()->GetUp();
				directs[2] = wkp_transform.lock()->GetFront();
				halfLengthes = initLengthes * wkp_transform.lock()->GetWorldScale();
			}
			inline bool IsHit(std::weak_ptr< CollisionPrimitive> other)override {
				return other.lock()->IsHitBox_OBB(this);
			}

			inline void GetMaxPointAndMinPoint(Vector3& arg_outputMax, Vector3& arg_outputMin) const {
				auto aabb_legthes = Geometry::BoxHit::GetBox_OBBContainAABBLength(*this);
				arg_outputMax = position + aabb_legthes;
				arg_outputMin = position - aabb_legthes;
			}
			bool IsHitPoint(CollisionPrimitive_Point* other)override;
			bool IsHitSphere(CollisionPrimitive_Sphere* other)override;
			bool IsHitBox_AABB(CollisionPrimitive_Box_AABB* other)override;
			bool IsHitBox_OBB(CollisionPrimitive_Box_OBB* other)override;
		private:
			Vector3 initLengthes;
		};
	}
}
