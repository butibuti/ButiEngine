#include"stdafx_u.h"
#include "TestBehavior.h"
#include"Header/GameObjects/DefaultGameComponent/TransformAnimation.h"
#include"Header/Common/CollisionPrimitive.h"

void ButiEngine::TestBehavior::OnUpdate()
{
    if (GameDevice::GetInput()->CheckKey(Keys::A)) {
        gameObject.lock()->transform->TranslateX(-1*0.1);
    }
    if (GameDevice::GetInput()->CheckKey(Keys::D)) {

        gameObject.lock()->transform->TranslateX(1*0.1);
    }
    if (GameDevice::GetInput()->CheckKey(Keys::W)) {
        gameObject.lock()->transform->TranslateY(1*0.1);

    }
    if (GameDevice::GetInput()->CheckKey(Keys::S)) {

        gameObject.lock()->transform->TranslateY(-1*0.1);
    }
    if (GameDevice::GetInput()->CheckKey(Keys::E)) {
        gameObject.lock()->transform->TranslateZ(1*0.1);
    }
    if (GameDevice::GetInput()->CheckKey(Keys::R)) {

        gameObject.lock()->transform->TranslateZ(-1*0.1);
    }

    if (GameDevice::GetInput()->TriggerKey(Keys::Space)) {
        auto AnimPlane = gameObject.lock()->GetGameObjectManager().lock()->GetGameObject("Plane_Target");
        auto anim = AnimPlane.lock()->GetGameComponent<TransformAnimation>();

        if (!anim) {
            anim= AnimPlane.lock()->AddGameComponent<TransformAnimation>();
            anim->SetSpeed(1.0 / 120);
            anim->SetTargetTransform(gameObject.lock()->transform->Clone());
            anim->SetReverse(false);
        }
    } 
GUI::Begin("In Visibility!!");
    shp_AABB->Update();
    auto mainCamera = GetCamera("main");
    if (mainCamera.lock()->IsContaineVisibility(shp_AABB)==1) {
        GUI::Text(u8"Œ©‚¦‚Ä‚é‰J‚¤”„‰J");

    }/*
    if ( GetCollisionManager().lock()->IsWillHit(shp_AABB,0)) {
        GUI::Text(u8"“–‚½‚Á‚Ä‰J‚¤”„‰J");
    }*/
    GUI::End();
}

void ButiEngine::TestBehavior::OnSet()
{
}

void ButiEngine::TestBehavior::Start()
{
    auto gameObject = GetManager().lock()->AddObjectFromCereal("test", ObjectFactory::Create<Transform>());
    shp_AABB = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(1,1,1), gameObject.lock()->transform);

    

}

void ButiEngine::TestBehavior::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::Behavior> ButiEngine::TestBehavior::Clone()
{
    return ObjectFactory::Create<TestBehavior>();
}
