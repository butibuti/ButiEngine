#pragma once
#include "stdafx.h"
#include "..\..\Header\Common\CollisionPrimitive.h"


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
