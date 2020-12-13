#pragma once
#include "stdafx.h"
#include "..\..\Header\Common\CollisionPrimitive.h"

#include "..\..\Header\Common\CerealUtill.h"

CEREAL_REGISTER_TYPE(ButiEngine::Collision::CollisionPrimitive_Point);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::Collision::CollisionPrimitive, ButiEngine::Collision::CollisionPrimitive_Point);
CEREAL_REGISTER_TYPE(ButiEngine::Collision::CollisionPrimitive_Ray);
CEREAL_REGISTER_POLYMORPHIC_RELATION(ButiEngine::Collision::CollisionPrimitive, ButiEngine::Collision::CollisionPrimitive_Ray);
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

bool ButiEngine::Collision::CollisionPrimitive_Sphere::IsHitPoint(CollisionPrimitive_Point* other)
{
    return Geometry::SphereHit::IsHitPointSphere(other->GetPosition(), *this);
}

bool ButiEngine::Collision::CollisionPrimitive_Sphere::IsHitSphere(CollisionPrimitive_Sphere* other)
{
    return  Geometry::SphereHit::IsHitSphereSphere(*this, *other);
}

bool ButiEngine::Collision::CollisionPrimitive_Sphere::IsHitBox_AABB(CollisionPrimitive_Box_AABB* other)
{
    return Geometry::BoxHit::IsHitSphereBox_AABB(*this ,*other);
}

bool ButiEngine::Collision::CollisionPrimitive_Sphere::IsHitBox_OBB(CollisionPrimitive_Box_OBB* other)
{
    return Geometry::BoxHit::IsHitSphereBox_OBB(*this, *other);
}

bool ButiEngine::Collision::CollisionPrimitive_Sphere::IsHitPolygon(CollisionPrimitive_Polygon* other)
{
    return Geometry::GeometryUtill::IsHitSpherePolygon(*this, other->points);
}

bool ButiEngine::Collision::CollisionPrimitive_Sphere::IsHitSurface(CollisionPrimitive_Surface* other)
{
    return Geometry::GeometryUtill::IsHitSphere(*this, other->wkp_transform.lock()->GetWorldPosition(), other->normal);
}

bool ButiEngine::Collision::CollisionPrimitive_Sphere::IsHitRay(CollisionPrimitive_Ray* other)
{
    return Geometry::RayHit::HitRaySphere(*other ,*this);
}

bool ButiEngine::Collision::CollisionPrimitive_Box_AABB::IsHitPoint(CollisionPrimitive_Point* other)
{
    return Geometry::BoxHit::IsHitPointBox_AABB(other->GetPosition(), *this);
}

bool ButiEngine::Collision::CollisionPrimitive_Box_AABB::IsHitSphere(CollisionPrimitive_Sphere* other)
{
    return Geometry::BoxHit::IsHitSphereBox_AABB(*other,*this);
}

bool ButiEngine::Collision::CollisionPrimitive_Box_AABB::IsHitBox_AABB(CollisionPrimitive_Box_AABB* other)
{
    return Geometry::BoxHit::IsHitBox_AABB(*this,*other);
}

bool ButiEngine::Collision::CollisionPrimitive_Box_AABB::IsHitBox_OBB(CollisionPrimitive_Box_OBB* other)
{
    return Geometry::BoxHit::IsHitBox_OBBBox_AABB(*this, *other);
}

bool ButiEngine::Collision::CollisionPrimitive_Box_AABB::IsHitPolygon(CollisionPrimitive_Polygon* other)
{
    return false;
}

bool ButiEngine::Collision::CollisionPrimitive_Box_AABB::IsHitSurface(CollisionPrimitive_Surface* other)
{
    return false;
}

bool ButiEngine::Collision::CollisionPrimitive_Box_AABB::IsHitRay(CollisionPrimitive_Ray* other)
{
    Vector3 colPos;
    return Geometry::RayHit::IsHitRayAABB(*other, *this, colPos);
}

bool ButiEngine::Collision::CollisionPrimitive_Box_AABB::IsHitSegment(CollisionPrimitive_Segment* other)
{
    Vector3 colPos;
    return Geometry::RayHit::IsHitSegmentAABB(*other, *this, colPos);
}


bool ButiEngine::Collision::CollisionPrimitive_Box_OBB::IsHitPoint(CollisionPrimitive_Point* other)
{
    return Geometry::BoxHit::IsHitPointBox_OBB(other->GetPosition(), *this);
}

bool ButiEngine::Collision::CollisionPrimitive_Box_OBB::IsHitSphere(CollisionPrimitive_Sphere* other)
{
    return Geometry::BoxHit::IsHitSphereBox_OBB(*other ,*this);
}

bool ButiEngine::Collision::CollisionPrimitive_Box_OBB::IsHitBox_AABB(CollisionPrimitive_Box_AABB* other)
{
    return Geometry::BoxHit::IsHitBox_OBBBox_AABB(*other, *this);
}

bool ButiEngine::Collision::CollisionPrimitive_Box_OBB::IsHitBox_OBB(CollisionPrimitive_Box_OBB* other)
{
    return Geometry::BoxHit::IsHitBox_OBB(*this, *other);
}

bool ButiEngine::Collision::CollisionPrimitive_Box_OBB::IsHitPolygon(CollisionPrimitive_Polygon* other)
{
    return false;
}

bool ButiEngine::Collision::CollisionPrimitive_Box_OBB::IsHitSurface(CollisionPrimitive_Surface* other)
{
    return false;
}

bool ButiEngine::Collision::CollisionPrimitive_Box_OBB::IsHitRay(CollisionPrimitive_Ray* other)
{
    Vector3 colPos;
    return Geometry::RayHit::IsHitRayOBB(*other, *this, wkp_transform.lock()->GetMatrix_WithoutScale(), colPos);
}

bool ButiEngine::Collision::CollisionPrimitive_Box_OBB::IsHitSegment(CollisionPrimitive_Segment* other)
{
    Vector3 colPos;
    return Geometry::RayHit::IsHitSegmentOBB(*other, *this, wkp_transform.lock()->GetMatrix_WithoutScale(), colPos);
}

ButiEngine::Geometry::Box_AABB ButiEngine::Collision::CollisionPrimitive_Box_OBB::ToAABB()
{
    auto length = Geometry::BoxHit::GetBox_OBBContainAABBLength(*this);
    return Geometry::Box_AABB(wkp_transform.lock()->GetWorldPosition(),length );
}

bool ButiEngine::Collision::CollisionPrimitive_Point::IsHitPoint(CollisionPrimitive_Point* other)
{
    return other->GetPosition()==GetPosition();
}

bool ButiEngine::Collision::CollisionPrimitive_Point::IsHitSphere(CollisionPrimitive_Sphere* other)
{
    return Geometry::SphereHit::IsHitPointSphere(GetPosition(), *other);
}

bool ButiEngine::Collision::CollisionPrimitive_Point::IsHitBox_AABB(CollisionPrimitive_Box_AABB* other)
{
    return Geometry::BoxHit::IsHitPointBox_AABB(GetPosition(), *other);
}

bool ButiEngine::Collision::CollisionPrimitive_Point::IsHitBox_OBB(CollisionPrimitive_Box_OBB* other)
{
    return Geometry::BoxHit::IsHitPointBox_OBB(GetPosition(),*other);
}

bool ButiEngine::Collision::CollisionPrimitive_Point::IsHitPolygon(CollisionPrimitive_Polygon* other)
{
    return false;
}

bool ButiEngine::Collision::CollisionPrimitive_Point::IsHitSurface(CollisionPrimitive_Surface* other)
{
    return false;
}

bool ButiEngine::Collision::CollisionPrimitive_Polygon::IsHitPoint(CollisionPrimitive_Point* other)
{
    return false;
}

bool ButiEngine::Collision::CollisionPrimitive_Polygon::IsHitSphere(CollisionPrimitive_Sphere* other)
{
    return Geometry::GeometryUtill::IsHitSpherePolygon(*other, points);
}

bool ButiEngine::Collision::CollisionPrimitive_Polygon::IsHitBox_AABB(CollisionPrimitive_Box_AABB* other)
{
    return false;
}

bool ButiEngine::Collision::CollisionPrimitive_Polygon::IsHitBox_OBB(CollisionPrimitive_Box_OBB* other)
{
    return false;
}

bool ButiEngine::Collision::CollisionPrimitive_Polygon::IsHitPolygon(CollisionPrimitive_Polygon* other)
{
    return false;
}

bool ButiEngine::Collision::CollisionPrimitive_Polygon::IsHitSurface(CollisionPrimitive_Surface* other)
{
    return false;
}

bool ButiEngine::Collision::CollisionPrimitive_Polygon::IsHitRay(CollisionPrimitive_Ray* other)
{
    return Geometry::SurfaceHit::IsHitLinePolygon(*other,points);
}

bool ButiEngine::Collision::CollisionPrimitive_Surface::IsHitPoint(CollisionPrimitive_Point* other)
{
    return false;
}

bool ButiEngine::Collision::CollisionPrimitive_Surface::IsHitSphere(CollisionPrimitive_Sphere* other)
{

    return Geometry::GeometryUtill::IsHitSphere(*other, wkp_transform.lock()->GetWorldPosition(),normal);
}

bool ButiEngine::Collision::CollisionPrimitive_Surface::IsHitBox_AABB(CollisionPrimitive_Box_AABB* other)
{
    return false;
}

bool ButiEngine::Collision::CollisionPrimitive_Surface::IsHitBox_OBB(CollisionPrimitive_Box_OBB* other)
{
    return false;
}

bool ButiEngine::Collision::CollisionPrimitive_Surface::IsHitPolygon(CollisionPrimitive_Polygon* other)
{
    return false;
}

bool ButiEngine::Collision::CollisionPrimitive_Surface::IsHitSurface(CollisionPrimitive_Surface* other)
{
    return false;
}

bool ButiEngine::Collision::CollisionPrimitive_Surface::IsHitRay(CollisionPrimitive_Ray* other)
{
    return Geometry::SurfaceHit::IsHitLineSurface(*other,wkp_transform.lock()->GetWorldPosition(), normal);
}


bool ButiEngine::Collision::CollisionPrimitive_Ray::IsHitSphere(CollisionPrimitive_Sphere* other)
{
    return Geometry::RayHit::HitRaySphere(*this, *other);
}

bool ButiEngine::Collision::CollisionPrimitive_Ray::IsHitPolygon(CollisionPrimitive_Polygon* other)
{
    return Geometry::SurfaceHit::IsHitLinePolygon(*this,other->points);
}

bool ButiEngine::Collision::CollisionPrimitive_Ray::IsHitSurface(CollisionPrimitive_Surface* other)
{
    return Geometry::SurfaceHit::IsHitLineSurface(*this, other->wkp_transform.lock()->GetWorldPosition(), other->normal);
}

bool ButiEngine::Collision::CollisionPrimitive_Ray::IsHitBox_AABB(CollisionPrimitive_Box_AABB* other)
{
    Vector3 colPos;
    return Geometry::RayHit::IsHitRayAABB(*this, *other,colPos);
}

bool ButiEngine::Collision::CollisionPrimitive_Ray::IsHitBox_OBB(CollisionPrimitive_Box_OBB* other)
{
    Vector3 colPos;
    return Geometry::RayHit::IsHitRayOBB(*this, *other, other->wkp_transform.lock()->GetMatrix_WithoutScale(),colPos);
}

bool ButiEngine::Collision::CollisionPrimitive_Segment::IsHitBox_AABB(CollisionPrimitive_Box_AABB* other)
{
    Vector3 colPos;
    return Geometry::RayHit::IsHitSegmentAABB(*this, *other, colPos);
}

bool ButiEngine::Collision::CollisionPrimitive_Segment::IsHitBox_OBB(CollisionPrimitive_Box_OBB* other)
{
    Vector3 colPos;
    return Geometry::RayHit::IsHitSegmentOBB(*this, *other, other->wkp_transform.lock()->GetMatrix_WithoutScale(), colPos);
}
