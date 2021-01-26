#pragma once

#include"ButiMath.h"
namespace ButiEngine {
    namespace Geometry
    {
        

        struct Sphere {
            Vector3 position;
            float radius = 1.0f; // 半径
            Sphere() {}
            Sphere(const Vector3& p, const float r) : position(p), radius(r) {}
            Sphere(const float r) :radius(r) {}
            ~Sphere() {}
        };
        struct Capsule {
            Segment s;
            float r; // 半径
            Capsule() : r(0.5f) {}
            Capsule(const Segment& s, const float r) : s(s), r(r) {}
            Capsule(const Vector3& p1, const Vector3& p2, const float r) : s(p1, p2), r(r) {}
            ~Capsule() {}
        };
        struct Cylinder {
            Segment s;
            float r; // 半径
            Cylinder() : r(0.5f) {}
            Cylinder(const Segment& s, const float r) : s(s), r(r) {}
            Cylinder(const Vector3& p1, const Vector3& p2, const float r) : s(p1, p2), r(r) {}
            ~Cylinder() {}
        };
        struct Box_AABB {
            Vector3 position;
            Vector3 halfLengthes = Vector3(0.5f, 0.5f, 0.5f);
            Box_AABB() {}
            Box_AABB(const Vector3& arg_centerPos, const Vector3& arg_lengthes) :position(arg_centerPos), halfLengthes(arg_lengthes / 2) {}
            Box_AABB(const Vector3& arg_lengthes) : halfLengthes(arg_lengthes / 2) {}

            inline float GetLength(const UINT index)const { return *(&halfLengthes.x + index); }
            inline float GetMin(const UINT index)const { return position.Get(index) - *(&halfLengthes.x + index); }
            inline float GetMax(const UINT index)const { return position.Get(index) + *(&halfLengthes.x + index); }
            inline Vector3 GetMinPoint()const { return position -halfLengthes ; }
            inline Vector3 GetMaxPoint()const { return position + halfLengthes; }
        };
        class Box_OBB_Static
        {
        public:
            Vector3 position;
            Vector3 directs[3] = { Vector3(1,0,0),Vector3(0,1,0) ,Vector3(0,0,1) };
            Vector3 halfLengthes = Vector3(0.5f, 0.5f, 0.5f);
        public:
            Box_OBB_Static(Vector3 arg_length) {
                halfLengthes = arg_length / 2;
            }
            Box_OBB_Static(){}
            inline const Vector3& GetDirect(const UINT index)const {
                return directs[index];
            }
            inline const float GetLength(const UINT index) const {
                return (*(&halfLengthes.x + index));
            }
            inline Vector3 GetAABBMax()const {
                return position + halfLengthes;
            }
            inline Vector3 GetAABBMin()const {
                return position - halfLengthes;
            }
            inline Vector3 GetMax()const {
                return halfLengthes;
            }
            inline Vector3 GetMin()const {
                return - halfLengthes;
            }
            Vector3 GetPos() const {
                return  position;
            }
        };

        namespace GeometryUtill {
            static inline float GetDistance(const Vector3& arg_point, const Vector3& arg_surfacePoint, const Vector3& arg_surfaceNormal) {
                return abs(arg_surfaceNormal.Dot(arg_point - arg_surfacePoint)) / arg_surfaceNormal.GetLength();
            }
            static inline bool IsHitSphere(const Sphere& arg_sphere, const Vector3& arg_surfacePoint, const Vector3& arg_surfaceNormal) {
                return (GetDistance(arg_sphere.position, arg_surfacePoint, arg_surfaceNormal)) <= arg_sphere.radius;
            }
            static inline float GetDistancePointToPolygon(const Vector3& arg_point,  const std::vector<Vector3>& arg_vertices) {
                Vector3 closest;

                Vector3 p0_p1 = arg_vertices[1] - arg_vertices[0];
                Vector3 p0_p2 = arg_vertices[2] - arg_vertices[0];
                Vector3 p0_pt = arg_point - arg_vertices[0];

                float d1 = p0_p1.Dot( p0_pt);
                float d2 = p0_p2.Dot(p0_pt);

                if (d1 <= 0.0f && d2<= 0.0f)
                {
                    closest = arg_vertices[0];
                    return  closest.Distance(arg_point);
                }

                Vector3 p1_pt = arg_point - arg_vertices[1];

                float d3 = p0_p1.Dot( p1_pt);
                float d4 =p0_p2.Dot( p1_pt);

                if (d3>= 0.0f && d4 <= d3)
                {
                    closest = arg_vertices[1];
                    return closest.Distance(arg_point);
                }


                float vc = d1 * d4 - d3 * d2;
                if (vc <= 0.0f && d1>= 0.0f && d3 <= 0.0f)
                {
                    float v = d1 / (d1 - d3);
                    closest = arg_vertices[0]+ v * p0_p1;
                    return closest.Distance( arg_point);
                }


                Vector3 p2_pt = arg_point - arg_vertices[2];

                float d5 =p0_p1.Dot( p2_pt);
                float d6 = p0_p2.Dot(p2_pt);
                if (d6 >= 0.0f && d5 <= d6)
                {
                    closest = arg_vertices[2];
                    return  closest.Distance(arg_point);
                }


                float vb = d5* d2 - d1 * d6;
                if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
                {
                    float w = d2 / (d2 - d6);
                    closest = arg_vertices[0]+ w * p0_p2;
                    return  closest.Distance(arg_point);
                }


                float va = d3 * d6- d5 * d4;
                if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
                {
                    float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
                    closest = arg_vertices[1] + w * (arg_vertices[2] - arg_vertices[1]);
                    return  closest.Distance(arg_point);
                }

                float denom = 1.0f / (va + vb + vc);
                float v = vb * denom;
                float w = vc * denom;
                closest = arg_vertices[0] + p0_p1 * v + p0_p2 * w;

                return  closest.Distance(arg_point);
            }

            static inline float IsHitSpherePolygon(const Sphere& arg_sphere, const std::vector<Vector3>& arg_vertices) {
                if (GetDistancePointToPolygon(arg_sphere.position, arg_vertices) <= arg_sphere.radius) {
                    return true;
                }
                else {
                    return false;
                }
            }

            static inline bool IsContainPointInPolygon(const Vector3& arg_point, const Vector3& arg_normal, const std::vector<Vector3>& arg_vertices) {
                auto itr = arg_vertices.begin();
                bool isBreak = false;
                for (; itr != arg_vertices.end(); itr++) {
                    auto next = itr + 1;
                    if (next == arg_vertices.end()) {
                        next = arg_vertices.begin();
                        isBreak = true;
                    }
                    if (arg_normal != Vector3(*next - *itr).Cross(Vector3(arg_point - *itr)).GetNormalize()) {

                        return false;
                    }
                    if (isBreak) {
                        break;
                    }
                    else {
                        itr++;
                    }
                }
                return true;
            }
            static inline bool IsContainPointInPolygon(const Vector3& arg_point, const std::vector<Vector3>& arg_vertices) {
                Vector3 normal = Vector3(arg_vertices.at(1) - arg_vertices.at(0)).Cross(arg_vertices.at(arg_vertices.size() - 1) - arg_vertices.at(0));
                normal.Normalize();
                return IsContainPointInPolygon(arg_point, normal, arg_vertices);
            }
            static inline float GetDistanceLineLine(const Line& arg_line, const Line& arg_otherLine) {
                Vector3 normal = arg_line.velocity.GetCross(arg_otherLine.velocity).GetNormalize();
                return normal.Dot(arg_otherLine.point - arg_line.point);
            }
        }

        namespace LineHit {
            static inline bool IsHitPointLine(const Vector3& arg_point, const Line& arg_line) {
                if (Vector3(arg_point - arg_line.point).Cross(arg_line.velocity).GetLength())
                {
                    return true;
                }
                else {
                    return false;
                }
            }
            static inline bool IsHitPointSegment(const Vector3& arg_point, const Segment& arg_segment) {
                auto v = Vector3(arg_point - arg_segment.point);
                if (v.Cross(arg_segment.velocity).GetLength() && v.GetLengthSqr() <= arg_segment.velocity.GetLengthSqr())
                {
                    return true;
                }
                else {
                    return false;
                }
            }
            static inline Vector3 GetDistancePointSegment(const Vector3& arg_point, const Segment& arg_segment) {
                Vector3 v = arg_segment.GetEndPoint() - arg_segment.point;
                Vector3 vp = arg_point - arg_segment.point;
                auto t = v.GetNormalize().Dot(vp) / v.GetLength();

                if (t > 1) {

                    return arg_segment.GetEndPoint() - arg_point;
                }
                else if (t < 0) {
                    return arg_segment.point - arg_point;
                }

                return v * t - vp;
            }

            static inline float GetPolygonY(const Vector3& arg_pointA, const Vector3& arg_pointB, const Vector3& arg_pointC, const float objX, const float objZ) {
                auto normal = Vector3(arg_pointA - arg_pointB).Cross(arg_pointA - arg_pointC);
                if (normal.y < 0) {
                    normal *= -1;
                }
                return arg_pointA.y - (normal.x * (objX - arg_pointA.x) + normal.z * (objZ - arg_pointA.z)) / normal.y;
            }
        }

        static inline bool IsHitLineLine(const Line& arg_linev1, const Line& arg_linev2) {
            Vector3 v3 = arg_linev2.point - arg_linev1.point;
            Vector3 normal1 = arg_linev1.velocity.GetCross(arg_linev2.velocity);
            Vector3 normal2 = arg_linev1.velocity.GetCross(v3);

            if (!normal2.GetLengthSqr()) {
                return true;
            }
            if (normal1.GetLengthSqr() != 0 && normal1.Cross(normal2).GetLengthSqr() == 0) {
                return true;
            }
            return false;
        }

        namespace SurfaceHit {
            static inline bool IsHitLineSurface(const Line& arg_line, const Vector3& arg_surfacePoint, const Vector3& arg_surfaceNormal) {
                Vector3 v1 = arg_line.point - arg_surfacePoint;
                if (v1.Dot(arg_surfaceNormal) == 0) {
                    return true;
                }
                if (arg_line.velocity.Dot(arg_surfaceNormal)) {
                    return true;
                }
                return false;
            }
            static inline bool IsHitLinePolygon(const Line& arg_line, const std::vector<Vector3>& arg_vertices) {
                if (arg_vertices.size() < 3) {
                    return false;
                }
                Vector3 normal = Vector3(arg_vertices.at(1) - arg_vertices.at(0)).Cross(arg_vertices.at(arg_vertices.size() - 1) - arg_vertices.at(0));
                normal.Normalize();
                if (!IsHitLineSurface(arg_line, arg_vertices.at(0), normal)) {
                    return false;
                }
                auto startPointDis = GeometryUtill::GetDistance(arg_line.point, arg_vertices.at(0), normal);



                auto intDivRaio = startPointDis/-(normal.Dot(arg_line.velocity));


                Vector3 penetrationPoint = arg_line.point + arg_line.velocity*intDivRaio;



                return GeometryUtill::IsContainPointInPolygon(penetrationPoint, normal, arg_vertices);
            }
            static inline bool IsHitSegmentSurface(const Segment& arg_segment, const Vector3& arg_surfacePoint, const Vector3& arg_surfaceNormal) {
                Vector3 v1 = arg_segment.point - arg_surfacePoint;
                Vector3 v2 = arg_segment.GetEndPoint() - arg_surfacePoint;

                if (arg_surfaceNormal.Dot(v1) * arg_surfaceNormal.Dot(v2) <= 0) {
                    return true;
                }
                return false;
            }
            static inline bool IsHitSegmentPolygon(const Segment& arg_segment, const std::vector<Vector3>& arg_vertices) {
                if (arg_vertices.size() < 3) {
                    return false;
                }
                Vector3 normal = Vector3(arg_vertices.at(1) - arg_vertices.at(0)).Cross(arg_vertices.at(arg_vertices.size() - 1) - arg_vertices.at(0));
                normal.Normalize();
                if (!IsHitSegmentSurface(arg_segment, arg_vertices.at(0), normal)) {
                    return false;
                }
                auto startPointDis = GeometryUtill::GetDistance(arg_segment.point, arg_vertices.at(0), normal);
                auto intDivRaio = startPointDis / (startPointDis + GeometryUtill::GetDistance(arg_segment.GetEndPoint(), arg_vertices.at(0), normal));

                Vector3 penetrationPoint = arg_vertices.at(0) + (1 - intDivRaio) * (arg_segment.GetEndPoint() - arg_vertices.at(0)) + intDivRaio * (arg_segment.point - arg_vertices.at(0));



                return GeometryUtill::IsContainPointInPolygon(penetrationPoint, normal, arg_vertices);
            }

            static inline bool IsFrontPoint(const Vector3& point, const Vector3& surfacePoint, const Vector3& surfaceNormal) {
                if (((Vector3)(point - surfacePoint)).Dot(surfaceNormal) > 0) {
                    return true;
                }
                else {
                    return false;
                }
            }

            static inline bool IsHitAABBSurfase(const Box_AABB& arg_AABB, const Vector3& surfacePoint, const Vector3& surfaceNormal) {

                Vector3 maxPoint =
                    arg_AABB.GetMinPoint();
                if (surfaceNormal.x > 0)
                {
                    maxPoint.x += arg_AABB.halfLengthes.x*2;
                }
                if (surfaceNormal.y > 0)
                {
                    maxPoint.y += arg_AABB.halfLengthes.y*2;
                }
                if (surfaceNormal.z > 0)
                {
                    maxPoint.z += arg_AABB.halfLengthes.z*2;
                } 

                return IsFrontPoint(maxPoint, surfacePoint, surfaceNormal);
                
                /*Vector3 minPoint =
                    arg_AABB.GetMinPoint();
                if (surfaceNormal.x < 0)
                {
                    minPoint.x += arg_AABB.halfLengthes.x*2;
                }
                if (surfaceNormal.y < 0)
                {
                    minPoint.y += arg_AABB.halfLengthes.y*2;
                }
                if (surfaceNormal.z < 0)
                {
                    minPoint.z += arg_AABB.halfLengthes.z*2;
                }*/



            }

            static inline bool IsHitAABBCameraFrustum(const Box_AABB& arg_AABB, std::shared_ptr<Transform> arg_transform, const Matrix4x4& arg_projectionMatrix ,float nearClip, float farClip) {
                
                Vector3 surfacePoints[6] = {};
                Vector3 surfaceNormals[6] = {};

                for (int i = 0; i < 4; i++)
                {
                    float a, b, c, d;
                    int r = i / 2;
                    if (i % 2 == 0)
                    {
                        // 平面の方程式
                        // ax + by + cz + d = 0
                        a = arg_projectionMatrix.m[3][ 0] - arg_projectionMatrix.m[r][ 0];
                        b = arg_projectionMatrix.m[3][ 1] - arg_projectionMatrix.m[r][ 1];
                        c = arg_projectionMatrix.m[3][ 2] - arg_projectionMatrix.m[r][ 2];
                        d = arg_projectionMatrix.m[3][ 3] - arg_projectionMatrix.m[r][ 3];
                    }
                    else
                    {
                        a = arg_projectionMatrix.m[3][ 0] + arg_projectionMatrix.m[r][ 0];
                        b = arg_projectionMatrix.m[3][ 1] + arg_projectionMatrix.m[r][ 1];
                        c = arg_projectionMatrix.m[3][ 2] + arg_projectionMatrix.m[r][ 2];
                        d = arg_projectionMatrix.m[3][ 3] + arg_projectionMatrix.m[r][ 3];
                    }

                    Vector3 normal =  Vector3(a, b, c).GetNormalize();
                   
                    normal = normal*arg_transform->GetWorldRotation() ;
                    surfaceNormals[i] = normal;
                    surfacePoints[i] = arg_transform->GetWorldPosition();
                }

                // for the near plane
                {
                    float a = arg_projectionMatrix.m[3][ 0] + arg_projectionMatrix.m[2][ 0];
                    float b = arg_projectionMatrix.m[3][ 1] + arg_projectionMatrix.m[2][ 1];
                    float c = arg_projectionMatrix.m[3][ 2] + arg_projectionMatrix.m[2][ 2];
                    float d = arg_projectionMatrix.m[3][ 3] + arg_projectionMatrix.m[2][ 3];

                    Vector3 normal = Vector3(a, b, c).GetNormalize();
                    normal = normal*arg_transform->GetWorldRotation();

                    Vector3 pos = arg_transform->GetWorldPosition ()+ (arg_transform->GetFront() * nearClip);
                    surfacePoints[4] =  pos;
                    surfaceNormals[4] = normal;
                }

                // for the far plane
                {
                    float a = arg_projectionMatrix.m[3][ 0] - arg_projectionMatrix.m[2][ 0];
                    float b = arg_projectionMatrix.m[3][ 1] - arg_projectionMatrix.m[2][ 1];
                    float c = arg_projectionMatrix.m[3][ 2] - arg_projectionMatrix.m[2][ 2];
                    float d = arg_projectionMatrix.m[3][ 3] - arg_projectionMatrix.m[2][ 3];

                    Vector3 normal = Vector3(a, b, c).GetNormalize();
                    normal = normal*arg_transform->GetWorldRotation ();

                    Vector3 pos = arg_transform->GetWorldPosition() + (arg_transform->GetFront() * nearClip) + (arg_transform->GetFront() * farClip);
                    surfaceNormals[5] = normal;
                    surfacePoints[5] = pos;
                }


                for (int i = 0; i < 6; i++) {
                    if (!IsHitAABBSurfase(arg_AABB, surfacePoints[i], surfaceNormals[i])) {
                        return false;
                    }
                }
                return true;
            }
        }
        namespace RayHit {

            static inline bool HitRaySphere(const Line& arg_ray, const Sphere& arg_sphere) {
                Vector3 spherePos = arg_sphere.position;
                spherePos.x = spherePos.x - arg_ray.point.x;
                spherePos.y = spherePos.y - arg_ray.point.y;
                spherePos.z = spherePos.z - arg_ray.point.z;

                float A = arg_ray.velocity.x * arg_ray.velocity.x + arg_ray.velocity.y * arg_ray.velocity.y + arg_ray.velocity.z * arg_ray.velocity.z;
                float B = arg_ray.velocity.x * spherePos.x + arg_ray.velocity.y * spherePos.y + arg_ray.velocity.z * spherePos.z;
                float C = spherePos.x * spherePos.x + spherePos.y * spherePos.y + spherePos.z * spherePos.z - arg_sphere.radius * arg_sphere.radius;

                if (A == 0.0f)
                    return false; 

                float s = B * B - A * C;
                if (s < 0.0f)
                    return false; 

                s = sqrtf(s);
                float a1 = (B - s) / A;
                float a2 = (B + s) / A;

                if (a1 < 0.0f || a2 < 0.0f)
                    return false; 


                return true;
            }
            static inline bool IsHitRayAABB(const Line& arg_ray, const Box_AABB& arg_box,Vector3& colPos) {


                float t;

                Vector3 min = arg_box.GetMinPoint();
                Vector3 max = arg_box.GetMaxPoint();


                t = -FLT_MAX;
                float t_max = FLT_MAX;

                for (int i = 0; i < 3; ++i) {
                    if (abs(arg_ray.velocity.GetData_const()[i]) < FLT_EPSILON) {
                        if (arg_ray.point.GetData_const()[i] < min.GetData()[i] || arg_ray.point.GetData_const()[i]> max.GetData()[i])
                            return false; 
                    }
                    else {

                        float odd = 1.0f / arg_ray.velocity.GetData_const()[i];
                        float t1 = (min.GetData()[i] - arg_ray.point.GetData_const()[i]) * odd;
                        float t2 = (max.GetData()[i]- arg_ray.point.GetData_const()[i]) * odd;
                        if (( t1) > (t2)) {
                            float tmp = t1; t1 = t2; t2 = tmp;
                        }

                        if (t1 > t) 
                            t = t1;
                        if (t2 < t_max) 
                            t_max = t2;


                        if (t >= t_max)
                            return false;
                    }
                }

                if (t < 0) {
                    return false;
                }
                colPos = arg_ray .point+ t * (arg_ray.velocity);
                
                return true;
            }
            static inline bool IsHitRayOBB(const Line& arg_ray, const Box_OBB_Static& arg_box, const Matrix4x4& arg_transform, Vector3& colPos) {
                Matrix4x4 invRotate = arg_transform.GetInverse();
                Vector3 p_l  = arg_ray.point * invRotate;

                invRotate._41 = 0;
                invRotate._42 = 0;
                invRotate._43 = 0;

                Vector3 dir_l = arg_ray.velocity * invRotate;

                float t;
                Vector3 min = arg_box.GetMin();
                Vector3 max = arg_box.GetMax();


                t = -FLT_MAX;
                float t_max = FLT_MAX;

                for (int i = 0; i < 3; ++i) {
                    if (abs(dir_l.GetData_const()[i]) < FLT_EPSILON) {
                        if (p_l.GetData_const()[i] < min.GetData()[i] || p_l.GetData_const()[i]> max.GetData()[i])
                            return false;
                    }
                    else {

                        float odd = 1.0f / dir_l.GetData_const()[i];
                        float t1 = (min.GetData()[i] - p_l.GetData_const()[i]) * odd;
                        float t2 = (max.GetData()[i] - p_l.GetData_const()[i]) * odd;
                        if ((t1) > (t2)) {
                            float tmp = t1; t1 = t2; t2 = tmp;
                        }

                        if (t1 > t) t = t1;
                        if (t2 < t_max) t_max = t2;


                        if (t >= t_max)
                            return false;
                    }
                }

                if (t < 0) {
                    return false;
                }
                colPos = arg_ray.point + t * (arg_ray.velocity);


                return true;
            }

            
            static inline bool IsHitSegmentAABB(const Segment& arg_ray, const Box_AABB& arg_box,Vector3& colPos) {


                float t;

                Vector3 min = arg_box.GetMinPoint();
                Vector3 max = arg_box.GetMaxPoint();


                t = -FLT_MAX;
                float t_max = FLT_MAX;

                for (int i = 0; i < 3; ++i) {
                    if (abs(arg_ray.velocity.GetData_const()[i]) < FLT_EPSILON) {
                        if (arg_ray.point.GetData_const()[i] < min.GetData()[i] || arg_ray.point.GetData_const()[i]> max.GetData()[i])
                            return false; 
                    }
                    else {

                        float odd = 1.0f / arg_ray.velocity.GetData_const()[i];
                        float t1 = (min.GetData()[i] - arg_ray.point.GetData_const()[i]) * odd;
                        float t2 = (max.GetData()[i]- arg_ray.point.GetData_const()[i]) * odd;
                        if (( t1) > (t2)) {
                            float tmp = t1; t1 = t2; t2 = tmp;
                        }

                        if (t1 > t) 
                            t = t1;
                        if (t2 < t_max) 
                            t_max = t2;


                        if (t >= t_max)
                            return false;
                    }
                }


                if (t < 0 || t * t>arg_ray.LengthSqr()) {
                    return false;
                }
                colPos = arg_ray .point+ t * (arg_ray.velocity);
                
                return true;
            }

            static inline bool IsHitSegmentOBB(const Segment& arg_ray, const Box_OBB_Static& arg_box, const Matrix4x4& arg_transform, Vector3& colPos) {
                Matrix4x4 invRotate = arg_transform.GetInverse();
                Vector3 p_l = arg_ray.point * invRotate;

                invRotate._41 = 0;
                invRotate._42 = 0;
                invRotate._43 = 0;
                Vector3 dir_l = arg_ray.velocity * invRotate;

                float t;
                Vector3 min = arg_box.GetMin();
                Vector3 max = arg_box.GetMax();


                t = -FLT_MAX;
                float t_max = FLT_MAX;

                for (int i = 0; i < 3; ++i) {
                    if (abs(dir_l.GetData_const()[i]) < FLT_EPSILON) {
                        if (p_l.GetData_const()[i] < min.GetData()[i] || p_l.GetData_const()[i]> max.GetData()[i])
                            return false;
                    }
                    else {

                        float odd = 1.0f / dir_l.GetData_const()[i];
                        float t1 = (min.GetData()[i] - p_l.GetData_const()[i]) * odd;
                        float t2 = (max.GetData()[i] - p_l.GetData_const()[i]) * odd;
                        if ((t1) > (t2)) {
                            float tmp = t1; t1 = t2; t2 = tmp;
                        }

                        if (t1 > t) t = t1;
                        if (t2 < t_max) t_max = t2;


                        if (t >= t_max)
                            return false;
                    }
                }


                if (t < 0||t>arg_ray.Length()) {
                    return false;
                }
                colPos = arg_ray.point + t * (arg_ray.velocity);


                return true;
            }

        }

        namespace BoxHit {
            static inline float GetDitancePointBox_AABBSqrt(const Vector3& arg_point, const Box_AABB& arg_box) {

                float SqLen = 0;

                for (int i = 0; i < 3; i++)
                {
                    if (arg_point.Get(i) < arg_box.GetMin(i))  // i=0はX、1はY、2はZの意味です
                        SqLen += (arg_point.Get(i) - arg_box.GetMin(i)) * (arg_point.Get(i) - arg_box.GetMin(i));
                    if (arg_point.Get(i) > arg_box.GetMax(i))
                        SqLen += (arg_point.Get(i) - arg_box.GetMax(i)) * (arg_point.Get(i) - arg_box.GetMax(i));
                }
                return SqLen;
            }

            static inline float GetDitancePointBox_AABB(const Vector3& arg_point, const Box_AABB& arg_box) {
                return (GetDitancePointBox_AABBSqrt(arg_point, arg_box));
            }


            static inline float GetDitancePointBox_OBB_Static(const Vector3& arg_point, const Box_OBB_Static& arg_box) {

                Vector3 output;
                for (int i = 0; i < 3; i++)
                {
                    float L = arg_box.GetLength(i);
                    if (L <= 0) continue;
                    float s = Vector3(arg_point - arg_box.GetPos()).Dot(arg_box.GetDirect(i)) / L;

                    // sの値から、はみ出した部分があればそのベクトルを加算
                    s = fabs(s);
                    if (s > 1)
                        output += (1 - s) * L * arg_box.GetDirect(i);
                }
                auto outputLen = output.GetLength();
                return outputLen;
            }

            static inline bool IsHitPointBox_OBB(const Vector3& arg_point, const Box_OBB_Static& arg_box) {
                return ! GetDitancePointBox_OBB_Static(arg_point, arg_box);
            }
            static inline bool IsHitSphereBox_OBB(const Sphere& arg_sphere, const Box_OBB_Static& arg_box) {
                return (arg_sphere.radius) >= GetDitancePointBox_OBB_Static(arg_sphere.position, arg_box);
            }
            static inline bool IsHitPointBox_AABB(const Vector3& arg_point, const Box_AABB& arg_box) {
                return !GetDitancePointBox_AABB(arg_point, arg_box);
            }
            static inline bool IsHitSphereBox_AABB(const Sphere& arg_sphere, const Box_AABB& arg_box) {
                return (arg_sphere.radius) >= GetDitancePointBox_AABB(arg_sphere.position, arg_box);
            }

            static inline float GetLengthSeparatedAxis(const Vector3& arg_sep, const Vector3& arg_e1, const Vector3& arg_e2, Vector3* arg_e3 = nullptr) {
                float r1 = fabs(arg_sep.Dot(arg_e1));
                float r2 = fabs(arg_sep.Dot(arg_e2));
                float r3 = arg_e3 ? (fabs(arg_sep.Dot(*arg_e3))) : 0;
                return r1 + r2 + r3;
            }

            static inline bool IsHitBox_AABB(const Box_AABB& arg_box, const Box_AABB& arg_otherBox) {
                Vector3  Ae1 = Vector3::XAxis * arg_box.GetLength(0);
                Vector3  Ae2 = Vector3::YAxis * arg_box.GetLength(1);
                Vector3 Ae3 = Vector3:: ZAxis * arg_box.GetLength(2);
                Vector3 Be1 = Vector3::XAxis* arg_otherBox.GetLength(0);
                Vector3 Be2 = Vector3::YAxis * arg_otherBox.GetLength(1);
                Vector3 Be3 = Vector3::ZAxis * arg_otherBox.GetLength(2);
                Vector3 Interval = arg_box.position - arg_otherBox.position;

                // 分離軸 : Ae1
                float rA = Ae1.GetLength();
                float rB = GetLengthSeparatedAxis(Vector3::XAxis, Be1, Be2, &Be3);
                float L = fabs(Interval.Dot(Vector3::XAxis));
                if (L > rA + rB)
                    return false; // 衝突していない

                 // 分離軸 : Ae2
                rA = Ae2.GetLength();
                rB = GetLengthSeparatedAxis(Vector3::YAxis, Be1, Be2, &Be3);
                L = fabs(Interval.Dot(Vector3::YAxis));
                if (L > rA + rB)
                    return false;

                // 分離軸 : Ae3
                rA = Ae3.GetLength();
                rB = GetLengthSeparatedAxis(Vector3::ZAxis, Be1, Be2, &Be3);
                L = fabs(Interval.Dot(Vector3::ZAxis));
                if (L > rA + rB)
                    return false;

                // 分離軸 : Be1
                rA = GetLengthSeparatedAxis(Vector3::XAxis, Ae1, Ae2, &Ae3);
                rB = Be1.GetLength();
                L = fabs(Interval.Dot(Vector3::XAxis));
                if (L > rA + rB)
                    return false;

                // 分離軸 : Be2
                rA = GetLengthSeparatedAxis(Vector3::YAxis, Ae1, Ae2, &Ae3);
                rB = Be2.GetLength();
                L = fabs(Interval.Dot(Vector3::YAxis));
                if (L > rA + rB)
                    return false;

                // 分離軸 : Be3
                rA = GetLengthSeparatedAxis(Vector3::ZAxis, Ae1, Ae2, &Ae3);
                rB = Be3.GetLength();
                L = fabs(Interval.Dot(Vector3::ZAxis));
                if (L > rA + rB)
                    return false;

                return true;
            }

            static inline bool IsHitBox_OBBBox_AABB(const Box_AABB& arg_box, const Box_OBB_Static& arg_otherBox) {
                Vector3 Ae1 = Vector3::XAxis * arg_box.GetLength(0);
                Vector3 Ae2 = Vector3::YAxis * arg_box.GetLength(1);
                Vector3 Ae3 = Vector3::ZAxis * arg_box.GetLength(2);
                Vector3 NBe1 = arg_otherBox.GetDirect(0), Be1 = NBe1 * arg_otherBox.GetLength(0);
                Vector3 NBe2 = arg_otherBox.GetDirect(1), Be2 = NBe2 * arg_otherBox.GetLength(1);
                Vector3 NBe3 = arg_otherBox.GetDirect(2), Be3 = NBe3 * arg_otherBox.GetLength(2);
                Vector3 Interval = arg_box.position - arg_otherBox.GetPos();

                // 分離軸 : Ae1
                float rA = Ae1.GetLength();
                float rB = GetLengthSeparatedAxis(Vector3::XAxis, Be1, Be2, &Be3);
                float L = fabs(Interval.Dot(Vector3::XAxis));
                if (L > rA + rB)
                    return false; // 衝突していない

                 // 分離軸 : Ae2
                rA = Ae2.GetLength();
                rB = GetLengthSeparatedAxis(Vector3::YAxis, Be1, Be2, &Be3);
                L = fabs(Interval.Dot(Vector3::YAxis));
                if (L > rA + rB)
                    return false;

                // 分離軸 : Ae3
                rA = Ae3.GetLength();
                rB = GetLengthSeparatedAxis(Vector3::ZAxis, Be1, Be2, &Be3);
                L = fabs(Interval.Dot(Vector3::ZAxis));
                if (L > rA + rB)
                    return false;

                // 分離軸 : Be1
                rA = GetLengthSeparatedAxis(NBe1, Ae1, Ae2, &Ae3);
                rB = Be1.GetLength();
                L = fabs(Interval.Dot(NBe1));
                if (L > rA + rB)
                    return false;

                // 分離軸 : Be2
                rA = GetLengthSeparatedAxis(NBe2, Ae1, Ae2, &Ae3);
                rB = Be2.GetLength();
                L = fabs(Interval.Dot(NBe2));
                if (L > rA + rB)
                    return false;

                // 分離軸 : Be3
                rA = GetLengthSeparatedAxis(NBe3, Ae1, Ae2, &Ae3);
                rB = Be3.GetLength();
                L = fabs(Interval.Dot(NBe3));
                if (L > rA + rB)
                    return false;


                return true;
            }
            static inline Vector3 GetBox_OBBContainAABBLength(const Box_OBB_Static& arg_box) {
                Vector3 Be1 = arg_box.GetDirect(0) * arg_box.GetLength(0);
                Vector3 Be2 = arg_box.GetDirect(1) * arg_box.GetLength(1);
                Vector3 Be3 = arg_box.GetDirect(2) * arg_box.GetLength(2);

                return Vector3(GetLengthSeparatedAxis(Vector3::XAxis, Be1, Be2, &Be3), GetLengthSeparatedAxis(Vector3::YAxis, Be1, Be2, &Be3), GetLengthSeparatedAxis(Vector3::ZAxis, Be1, Be2, &Be3));
            }

            static inline bool IsHitBox_OBB(const Box_OBB_Static& arg_box, const Box_OBB_Static& arg_otherBox) {
                // 各方向ベクトルの確保
                  // （N***:標準化方向ベクトル）
                Vector3 NAe1 = arg_box.GetDirect(0), Ae1 = NAe1 * arg_box.GetLength(0);
                Vector3 NAe2 = arg_box.GetDirect(1), Ae2 = NAe2 * arg_box.GetLength(1);
                Vector3 NAe3 = arg_box.GetDirect(2), Ae3 = NAe3 * arg_box.GetLength(2);
                Vector3 NBe1 = arg_otherBox.GetDirect(0), Be1 = NBe1 * arg_otherBox.GetLength(0);
                Vector3 NBe2 = arg_otherBox.GetDirect(1), Be2 = NBe2 * arg_otherBox.GetLength(1);
                Vector3 NBe3 = arg_otherBox.GetDirect(2), Be3 = NBe3 * arg_otherBox.GetLength(2);
                Vector3 Interval = arg_box.GetPos() - arg_otherBox.GetPos();

                // 分離軸 : Ae1
                float rA = Ae1.GetLength();
                float rB = GetLengthSeparatedAxis(NAe1, Be1, Be2, &Be3);
                float L = fabs(Interval.Dot(NAe1));
                if (L > rA + rB)
                    return false; // 衝突していない

                 // 分離軸 : Ae2
                rA = Ae2.GetLength();
                rB = GetLengthSeparatedAxis(NAe2, Be1, Be2, &Be3);
                L = fabs(Interval.Dot(NAe2));
                if (L > rA + rB)
                    return false;

                // 分離軸 : Ae3
                rA = Ae3.GetLength();
                rB = GetLengthSeparatedAxis(NAe3, Be1, Be2, &Be3);
                L = fabs(Interval.Dot(NAe3));
                if (L > rA + rB)
                    return false;

                // 分離軸 : Be1
                rA = GetLengthSeparatedAxis(NBe1, Ae1, Ae2, &Ae3);
                rB = Be1.GetLength();
                L = fabs(Interval.Dot(NBe1));
                if (L > rA + rB)
                    return false;

                // 分離軸 : Be2
                rA = GetLengthSeparatedAxis(NBe2, Ae1, Ae2, &Ae3);
                rB = Be2.GetLength();
                L = fabs(Interval.Dot(NBe2));
                if (L > rA + rB)
                    return false;

                // 分離軸 : Be3
                rA = GetLengthSeparatedAxis(NBe3, Ae1, Ae2, &Ae3);
                rB = Be3.GetLength();
                L = fabs(Interval.Dot(NBe3));
                if (L > rA + rB)
                    return false;

                // 分離軸 : C11
                Vector3 Cross = NAe1.GetCross(NBe1);
                rA = GetLengthSeparatedAxis(Cross, Ae2, Ae3);
                rB = GetLengthSeparatedAxis(Cross, Be2, Be3);
                L = fabs(Interval.Dot(Cross));
                if (L > rA + rB)
                    return false;

                // 分離軸 : C12
                Cross = NAe1.GetCross(NBe2);
                rA = GetLengthSeparatedAxis(Cross, Ae2, Ae3);
                rB = GetLengthSeparatedAxis(Cross, Be1, Be3);
                L = fabs(Interval.Dot(Cross));
                if (L > rA + rB)
                    return false;

                // 分離軸 : C13
                Cross = NAe1.GetCross(NBe3);
                rA = GetLengthSeparatedAxis(Cross, Ae2, Ae3);
                rB = GetLengthSeparatedAxis(Cross, Be1, Be2);
                L = fabs(Interval.Dot(Cross));
                if (L > rA + rB)
                    return false;

                // 分離軸 : C21
                Cross = NAe2.GetCross(NBe1);
                rA = GetLengthSeparatedAxis(Cross, Ae1, Ae3);
                rB = GetLengthSeparatedAxis(Cross, Be2, Be3);
                L = fabs(Interval.Dot(Cross));
                if (L > rA + rB)
                    return false;

                // 分離軸 : C22
                Cross = NAe2.GetCross(NBe2);
                rA = GetLengthSeparatedAxis(Cross, Ae1, Ae3);
                rB = GetLengthSeparatedAxis(Cross, Be1, Be3);
                L = fabs(Interval.Dot(Cross));
                if (L > rA + rB)
                    return false;

                // 分離軸 : C23
                Cross = NAe2.GetCross(NBe3);
                rA = GetLengthSeparatedAxis(Cross, Ae1, Ae3);
                rB = GetLengthSeparatedAxis(Cross, Be1, Be2);
                L = fabs(Interval.Dot(Cross));
                if (L > rA + rB)
                    return false;

                // 分離軸 : C31
                Cross = NAe3.GetCross(NBe1);
                rA = GetLengthSeparatedAxis(Cross, Ae1, Ae2);
                rB = GetLengthSeparatedAxis(Cross, Be2, Be3);
                L = fabs(Interval.Dot(Cross));
                if (L > rA + rB)
                    return false;

                // 分離軸 : C32
                Cross = NAe3.GetCross(NBe2);
                rA = GetLengthSeparatedAxis(Cross, Ae1, Ae2);
                rB = GetLengthSeparatedAxis(Cross, Be1, Be3);
                L = fabs(Interval.Dot(Cross));
                if (L > rA + rB)
                    return false;

                // 分離軸 : C33
                Cross = NAe3.GetCross(NBe3);
                rA = GetLengthSeparatedAxis(Cross, Ae1, Ae2);
                rB = GetLengthSeparatedAxis(Cross, Be1, Be2);
                L = fabs(Interval.Dot(Cross));
                if (L > rA + rB)
                    return false;


                return true;
            }
        }

        namespace SphereHit {
            inline bool IsHitSphereSphere(const Sphere& arg_sphere, const Sphere& arg_otherSphere) {
                float distance = Vector3(arg_sphere.position - arg_otherSphere.position).GetLength();
                auto border = arg_sphere.radius + arg_otherSphere.radius;
                return distance <= border;
            }
            inline bool IsHitPointSphere(const Vector3& arg_point, const Sphere& arg_otherSphere) {
                float distance = Vector3(arg_point - arg_otherSphere.position).GetLength();
                return distance <=arg_otherSphere.radius;
            }
        }

        namespace CapsuleHit {
            inline bool IsHitCapsuleSphere(const Sphere& arg_sphere,const Capsule& arg_capsule) {
                float distance = LineHit::GetDistancePointSegment(arg_sphere.position, arg_capsule.s).GetLength();
                return distance <= arg_sphere.radius + arg_capsule.r;

            }
        }
    }
}