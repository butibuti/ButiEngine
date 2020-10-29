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
