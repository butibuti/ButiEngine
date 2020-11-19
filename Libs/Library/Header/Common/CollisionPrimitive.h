#pragma once
#include"stdafx.h"
namespace ButiEngine {
	namespace Collision {
		class CollisionPrimitive_Point;
		class CollisionPrimitive_Sphere;
		class CollisionPrimitive_Box_AABB;
		class CollisionPrimitive_Box_OBB;
		class CollisionPrimitive_Polygon;
		class CollisionPrimitive_Surface;
		class CollisionPrimitive_PLane;
		class CollisionPrimitive :public IObject
		{
		public:
			void Initialize() override {}
			void PreInitialize()override {}
			virtual void Update() = 0;
			virtual bool IsHit(std::weak_ptr< CollisionPrimitive> other) = 0;

			virtual bool IsHitPoint(CollisionPrimitive_Point* other) = 0;
			virtual bool IsHitPolygon(CollisionPrimitive_Polygon* other) = 0;
			virtual bool IsHitSurface(CollisionPrimitive_Surface* other) = 0;
			virtual bool IsHitSphere(CollisionPrimitive_Sphere* other) = 0;
			virtual bool IsHitBox_AABB(CollisionPrimitive_Box_AABB* other) = 0;
			virtual bool IsHitBox_OBB(CollisionPrimitive_Box_OBB* other) = 0;
			virtual void GetMaxPointAndMinPoint(Vector3& arg_outputMax, Vector3& arg_outputMin) const = 0;
			virtual std::shared_ptr<CollisionPrimitive> Clone() = 0;
			virtual void ShowUI() = 0;
			std::weak_ptr<Transform> wkp_transform;
		};
		class CollisionPrimitive_Point :public CollisionPrimitive
		{
		public:
			inline CollisionPrimitive_Point(std::weak_ptr<Transform> arg_weak_transform)
			{
				wkp_transform = arg_weak_transform;
			}
			CollisionPrimitive_Point() {}
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
			bool IsHitPolygon(CollisionPrimitive_Polygon* other)override;
			bool IsHitSurface(CollisionPrimitive_Surface* other)override;

			std::shared_ptr<CollisionPrimitive> Clone()override {
				return ObjectFactory::Create<CollisionPrimitive_Point>();
			}

			void ShowUI() override {
				ImGui::BulletText("Point");
			}

			template<class Archive>
			void serialize(Archive& archive)
			{
				archive(wkp_transform);
			}

		private:
		};
		class CollisionPrimitive_Sphere :public CollisionPrimitive, public Geometry::Sphere
		{
		public:
			inline CollisionPrimitive_Sphere(const float arg_radius, std::weak_ptr<Transform> arg_weak_transform)
				:Geometry::Sphere(arg_radius) {
				wkp_transform = arg_weak_transform;
			}
			CollisionPrimitive_Sphere() {}
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
			bool IsHitPolygon(CollisionPrimitive_Polygon* other)override;
			bool IsHitSurface(CollisionPrimitive_Surface* other)override;

			std::shared_ptr<CollisionPrimitive> Clone()override {
				auto ret = ObjectFactory::Create<CollisionPrimitive_Sphere>();
				ret->radius = radius;
				return ret;
			}
			void ShowUI() override {

				if (ImGui::TreeNode("Sphere")) {
					ImGui::BulletText("radius");
					ImGui::DragFloat("##radius", &radius, 0.01, 0, 500);
					ImGui::TreePop();
				}

			}

			template<class Archive>
			void serialize(Archive& archive)
			{
				archive(wkp_transform);
				archive(radius);
			}
		private:
		};


		class CollisionPrimitive_Polygon :public CollisionPrimitive
		{
		public:
			inline CollisionPrimitive_Polygon(const Vector3& arg_vertex1, const Vector3& arg_vertex2, const Vector3& arg_vertex3, std::weak_ptr<Transform> arg_weak_transform)
			{
				wkp_transform = arg_weak_transform;
				points.push_back(arg_vertex1);
				points.push_back(arg_vertex2);
				points.push_back(arg_vertex3);

				initPoints = points;
				Update();
			}
			CollisionPrimitive_Polygon() {}
			inline void Update()override {
				for (int i = 0; i < 3; i++) {
					points[i] = initPoints[i] + wkp_transform.lock()->GetWorldPosition();
				}
			}
			inline Vector3 GetPosition() {
				return wkp_transform.lock()->GetWorldPosition();
			}
			inline bool IsHit(std::weak_ptr< CollisionPrimitive> other)override
			{
				return other.lock()->IsHitPolygon(this);


			}
			inline void GetMaxPointAndMinPoint(Vector3& arg_outputMax, Vector3& arg_outputMin) const override {
				auto point = wkp_transform.lock()->GetWorldPosition();

				float maxX = max(points[2].x, max(points[0].x, points[1].x));
				float maxY = max(points[2].y, max(points[0].y, points[1].y));
				float maxZ = max(points[2].z, max(points[0].z, points[1].z));

				float minX = min(points[2].x, min(points[0].x, points[1].x));
				float minY = min(points[2].y, min(points[0].y, points[1].y));
				float minZ = min(points[2].z, min(points[0].z, points[1].z));

				arg_outputMax = point + Vector3(maxX, maxY, maxZ);
				arg_outputMin = point + Vector3(minX, minY, minZ);
			}
			bool IsHitPoint(CollisionPrimitive_Point* other)override;
			bool IsHitSphere(CollisionPrimitive_Sphere* other)override;
			bool IsHitBox_AABB(CollisionPrimitive_Box_AABB* other)override;
			bool IsHitBox_OBB(CollisionPrimitive_Box_OBB* other)override;
			bool IsHitPolygon(CollisionPrimitive_Polygon* other)override;
			bool IsHitSurface(CollisionPrimitive_Surface* other)override;
			std::shared_ptr<CollisionPrimitive> Clone()override {
				return ObjectFactory::Create<CollisionPrimitive_Polygon>();
			}

			void ShowUI() override {
				ImGui::BulletText("Polygon");
				for (int i = 0; i < 3; i++) {
					ImGui::DragFloat3(("point##" + std::to_string(i)).c_str(), &initPoints[i].x, 0.01f, -100, 100);
				}
			}

			template<class Archive>
			void serialize(Archive& archive)
			{
				archive(wkp_transform);
				archive(initPoints);
			}

			std::vector<Vector3 >initPoints;
			std::vector<Vector3 >points;
		private:
		};
		class CollisionPrimitive_Surface :public CollisionPrimitive
		{
		public:
			inline CollisionPrimitive_Surface(const Vector3& arg_normal, std::weak_ptr<Transform> arg_weak_transform)
				{
				wkp_transform = arg_weak_transform;
				normal = arg_normal;
			}
			CollisionPrimitive_Surface() {}
			inline void Update()override {
			}
			inline bool IsHit(std::weak_ptr< CollisionPrimitive> other)override
			{
				return other.lock()->IsHitSurface(this);


			}
			inline void GetMaxPointAndMinPoint(Vector3& arg_outputMax, Vector3& arg_outputMin) const {
				arg_outputMax = Vector3(100, 100, 100);
				arg_outputMin = Vector3(-100, -100, -100);
			}
			bool IsHitPoint(CollisionPrimitive_Point* other)override;
			bool IsHitSphere(CollisionPrimitive_Sphere* other)override;
			bool IsHitBox_AABB(CollisionPrimitive_Box_AABB* other)override;
			bool IsHitBox_OBB(CollisionPrimitive_Box_OBB* other)override;
			bool IsHitPolygon(CollisionPrimitive_Polygon* other)override;
			bool IsHitSurface(CollisionPrimitive_Surface* other)override;
			std::shared_ptr<CollisionPrimitive> Clone()override {
				auto ret = ObjectFactory::Create<CollisionPrimitive_Surface>();
				ret->normal = normal;
				return ret;
			}
			void ShowUI() override {

				if (ImGui::TreeNode("Surface")) {
					ImGui::BulletText("Normal");
					if (ImGui::DragFloat3("##normal", &normal.x, 0.01, 0, 500)) {
						normal.Normalize();
					}
					ImGui::TreePop();
				}

			}

			template<class Archive>
			void serialize(Archive& archive)
			{
				archive(wkp_transform);
				archive(normal);

			}
			Vector3 normal;
		private:
		};

		class CollisionPrimitive_Box_AABB : public CollisionPrimitive, public Geometry::Box_AABB {
		public:
			inline CollisionPrimitive_Box_AABB(const Vector3& arg_length, std::weak_ptr<Transform> arg_weak_transform)
				:Geometry::Box_AABB(arg_length) {
				wkp_transform = arg_weak_transform;
				initLengthes = arg_length / 2;
			}
			CollisionPrimitive_Box_AABB() {}
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
			bool IsHitPolygon(CollisionPrimitive_Polygon* other)override;
			bool IsHitSurface(CollisionPrimitive_Surface* other)override;

			void ShowUI() override {

				if (ImGui::TreeNode("Box_AABB")) {
					ImGui::BulletText("Length");
					ImGui::DragFloat3("##length", &initLengthes.x, 0.01, 0, 500);
					ImGui::TreePop();
				}
			}

			std::shared_ptr<CollisionPrimitive> Clone()override {
				auto ret = ObjectFactory::Create<CollisionPrimitive_Box_AABB>();
				ret->halfLengthes = halfLengthes;
				ret->initLengthes = initLengthes;
				return ret;
			}
			template<class Archive>
			void serialize(Archive& archive)
			{
				archive(wkp_transform);
				archive(initLengthes);
				archive(halfLengthes);
			}
		private:
			Vector3 initLengthes;
		};
		class CollisionPrimitive_Box_OBB : public CollisionPrimitive, public Geometry::Box_OBB_Static {
		public:
			inline CollisionPrimitive_Box_OBB(const Vector3& arg_length, std::weak_ptr<Transform> arg_weak_transform)
				:Geometry::Box_OBB_Static(arg_length) {
				wkp_transform = arg_weak_transform;
				initLengthes = arg_length * 0.5f;
			}
			CollisionPrimitive_Box_OBB() {}
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
			bool IsHitPolygon(CollisionPrimitive_Polygon* other)override;
			bool IsHitSurface(CollisionPrimitive_Surface* other)override;

			void ShowUI() override {

				if (ImGui::TreeNode("Box_OBB")) {
					ImGui::BulletText("Length");
					ImGui::DragFloat3("##length", &initLengthes.x, 0.01, 0, 500);
					ImGui::TreePop();
				}
			}

			std::shared_ptr<CollisionPrimitive> Clone()override {
				auto ret = ObjectFactory::Create<CollisionPrimitive_Box_OBB>();
				ret->halfLengthes = halfLengthes;
				ret->initLengthes = initLengthes;
				return ret;
			}
			template<class Archive>
			void serialize(Archive& archive)
			{
				archive(wkp_transform);
				archive(initLengthes);
				archive(halfLengthes);
			}
		private:
			Vector3 initLengthes;
		};
	}
}
CEREAL_REGISTER_TYPE(ButiEngine::Collision::CollisionPrimitive_Point);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::Collision::CollisionPrimitive, ButiEngine::Collision::CollisionPrimitive_Point);
CEREAL_REGISTER_TYPE(ButiEngine::Collision::CollisionPrimitive_Sphere);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::Collision::CollisionPrimitive, ButiEngine::Collision::CollisionPrimitive_Sphere);
CEREAL_REGISTER_TYPE(ButiEngine::Collision::CollisionPrimitive_Box_AABB);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::Collision::CollisionPrimitive, ButiEngine::Collision::CollisionPrimitive_Box_AABB);
CEREAL_REGISTER_TYPE(ButiEngine::Collision::CollisionPrimitive_Box_OBB);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::Collision::CollisionPrimitive, ButiEngine::Collision::CollisionPrimitive_Box_OBB);
CEREAL_REGISTER_TYPE(ButiEngine::Collision::CollisionPrimitive_Polygon);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::Collision::CollisionPrimitive, ButiEngine::Collision::CollisionPrimitive_Polygon);
CEREAL_REGISTER_TYPE(ButiEngine::Collision::CollisionPrimitive_Surface);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::Collision::CollisionPrimitive, ButiEngine::Collision::CollisionPrimitive_Surface);