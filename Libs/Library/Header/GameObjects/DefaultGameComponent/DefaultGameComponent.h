#pragma once
#include "../../GameComponentHeader.h"
#include"Header/Common/CollisionPrimitive.h"
#include"Header/Resources/Vertex.h"
#include"../../Resources/DrawData/IDrawData.h"

namespace ButiEngine {
	class CameraMan :public  GameComponent {
	public:
		void Start() override;
		void OnUpdate()override;
		virtual std::string GetGameComponentName() {
			return "CameraMan";
		}
		void ShakeVartical(const float power);
		void ShakeHorizontal(const float power);
		template<class Archive>
		void serialize(Archive& archive)
		{
			//archive(isActive);
			//archive(cameraName);
		}
		std::shared_ptr<GameComponent> Clone()override {
			return ObjectFactory::Create<CameraMan>();
		}
		void OnShowUI()override;
	private:
		float vertQuake;
		static float vertQuakeMax;
		float horizonQuake;
		static float horizonQuakeMax;
		std::shared_ptr<Transform> child;
		std::string cameraName = "main";
	};
	namespace Collision {
		enum class CollisionPrimType {
			cube, sphere, capsule
		};
		class ColliderComponent :public GameComponent
		{
		public:
			ColliderComponent(std::shared_ptr<CollisionPrimitive> arg_shp_collisionPrim, const UINT arg_layerNum = 0);
			ColliderComponent() {}
			void Initialize()override;
			void OnSet()override;
			void OnUpdate()override;
			void OnRemove()override;
			void Start()override;
			void CollisionStart();
			void CollisionStop();
			std::string GetGameComponentName() {
				return "ColliderComponent";
			}
			std::shared_ptr<GameComponent> Clone()override;
			std::shared_ptr<CollisionPrimitive> GetCollisionPrimitive();
			void OnShowUI()override;

			template<class Archive>
			void serialize(Archive& archive)
			{
				archive(shp_collisionPrim);
				archive(layerNum);
				archive(isActive);
			}
		private:
			UINT* p_index = nullptr;
			UINT layerNum = 0;
			std::shared_ptr<CollisionPrimitive> shp_collisionPrim;
		};
	}


	class TransformAnimation :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "TransformAnimation";
		}
		void OnUpdate()override;
		std::shared_ptr<Transform> GetInitTransform();
		std::shared_ptr<Transform> GetTargetTransform();
		virtual void SetInitTransform(std::shared_ptr<Transform> arg_shp_InitTransform);
		void SetEaseType(const Easing::EasingType type);
		void SetSpeed(const float arg_speed);
		void SetTime(const float arg_time);
		void SetReverse(const bool isReverse);
		std::shared_ptr<GameComponent> Clone()override;

		virtual void _cdecl SetTargetTransform(std::shared_ptr<Transform> arg_shp_targetTransform);
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(speed);
			archive(t);
			archive(shp_targetTransform);
			archive(shp_initTransform);
			archive(easeType);
			archive(isReverse);
		}
		void OnShowUI()override;
		virtual void PositionSet();
		void OnSet()override;
	protected:
		int direction = 1;
		float t = 0;
		std::shared_ptr<Transform> shp_targetTransform;
		Quat initRotate;
		Quat targetRotate;
		std::shared_ptr<Transform> shp_initTransform;
		float speed = 0.002f;;
		Easing::EasingType easeType = Easing::EasingType::EaseIn;
		bool isReverse = false;
	}; class UIAnimation :public TransformAnimation
	{
	public:
		std::string GetGameComponentName()override {
			return "UIAnimation";
		}
		void SetInitTransform(std::shared_ptr<Transform> arg_shp_InitTransform)override;
		void SetTargetTransform(std::shared_ptr<Transform> arg_shp_targetTransform)override;
		std::shared_ptr<GameComponent> Clone()override;
		void PositionSet()override;
		void OnShowUI()override;
		void OnSet()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(speed);
			archive(t);
			archive(shp_targetTransform);
			archive(shp_initTransform);
			archive(easeType);
			archive(isReverse);
		}
	protected:
		Vector2 size;
	};
	class CubeTransformAnimation :public TransformAnimation
	{
	public:
		std::string GetGameComponentName()override {
			return "CubeTransformAnimation";
		}
		void SetXEaseType(const Easing::EasingType type);
		void SetYEaseType(const Easing::EasingType type);
		void SetZEaseType(const Easing::EasingType type);
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(speed);
			archive(t);
			archive(shp_targetTransform);
			archive(shp_initTransform);
			archive(easeType);
			archive(xEaseType);
			archive(yEaseType);
			archive(zEaseType);
			archive(isReverse);
		}
		void OnShowUI()override;
		void PositionSet()override;
	protected:
		Easing::EasingType xEaseType = Easing::EasingType::none;
		Easing::EasingType yEaseType = Easing::EasingType::none;
		Easing::EasingType zEaseType = Easing::EasingType::none;
	};

	class FPSViewBehavior :public GameComponent
	{
	public:
		void Start()override;
		void OnUpdate() override;
		void OnSet()override;
		std::string GetGameComponentName()override {
			return "FPSViewBehavior";
		};
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		std::weak_ptr<GameObject> head;
		bool isCenter = true;
	};
	class ChaseComponent :public GameComponent
	{
	public:
		ChaseComponent(std::shared_ptr<Transform> arg_shp_target, const float arg_speed = 0.1f);
		ChaseComponent() {}
		void OnUpdate() override;
		void OnSet()override;
		std::string GetGameComponentName()override {
			return "ChaseComponent";
		};
		void OnShowUI()override;

		std::shared_ptr<GameComponent> Clone()override;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(shp_target);
			archive(speed);
			archive(isActive);
		}

	private:
		std::shared_ptr<Transform> shp_target;
		float speed;
	};
	class IKComponent :public GameComponent
	{
	public:
		IKComponent() {}
		std::string GetGameComponentName()override {
			return "IKComponent";
		}
		void OnRemove()override;
		void OnSet()override;
		void Start()override;
		std::shared_ptr<GameComponent> Clone()override;

		void OnShowUI()override;

		void OnUpdate()override;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(vec_ikData);
		}


	protected:
		void RegistIK();
	private:
		std::shared_ptr<ModelDrawData> shp_modelData;
		std::vector<std::shared_ptr<Bone>> vec_endBones;
		std::vector<std::shared_ptr< IKData>> vec_ikData;
	};


	struct Particle2D {
		Vector3 position;
		Vector4 color = Vector4(1, 1, 1, 1);
		float size = 1.0f;
		Vector3 velocity;
		Vector3 force;
		float accelation;
		float life = 60;
		Vector4 colorPase;
		float sizePase = 0;
	};
	struct Particle3D {
		Vector3 position;
		Vector3 axis = Vector3(1, 0, 0);

		Vector4 color = Vector4(1, 1, 1, 1);
		float size = 5.0f;
		float angle = 0.0f;
		float anglePase = 0.0f;
		Vector3 velocity;
		Vector3 force;
		float accelation;
		float life = 60;
		Vector4 colorPase;
		float sizePase = 0;
	};


	class Resource_RealTimeMesh;


	class ImmediateParticleController :public GameComponent
	{
	public:

		void OnUpdate()override;
		void OnSet()override;
		void Start();
		std::string GetGameComponentName()override {
			return "ImmediateParticleController";
		}
		void AddParticle(const Particle3D& arg_particle);
		std::shared_ptr<GameComponent> Clone()override;
		void OnRemove()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(meshTag);
		}
	private:
		std::vector<Particle3D> vec_particleInfo;
		MeshTag meshTag;
		std::shared_ptr<BackupData<Vertex::Vertex_UV_Normal_Color>> shp_backUp;
		std::shared_ptr<Resource_RealTimeMesh> shp_mesh;
		int addParticleCount = 0;
	};
	class LookAtComponent :public GameComponent
	{
	public:
		LookAtComponent(std::shared_ptr<Transform> arg_shp_lookTarget);
		LookAtComponent() {}

		void OnUpdate()override;
		void OnSet()override;
		void Detach();
		std::string GetGameComponentName()override {
			return "LookAtComponent";
		}
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(shp_lookTarget);
			archive(isActive);
		}
		void OnShowUI();
	private:
		std::shared_ptr<Transform> shp_lookTarget;
	};
	class PostEffectParamUpdater :public GameComponent
	{
	public:
		PostEffectParamUpdater(const Vector4& pase);
		PostEffectParamUpdater();
		void OnUpdate()override;
		std::string GetGameComponentName()override {
			return "PostEffectParamUpdater";
		}
		void OnSet();
		void Start();
		std::shared_ptr<GameComponent> Clone()override;
		void OnShowUI()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(pase);
			archive(isActive);
		}
	private:
		Vector4 pase;
		std::shared_ptr<CBuffer<LightVariable>> shp_param;
	};
	class SplineCurveMover :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "SplineCurveMover";
		}
		void OnUpdate()override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(speed);
			archive(t);
			archive(splineCurve);
		}
		void OnShowUI()override;
		void PositionSet();
		void SetSplineCurve(const SplineCurve& arg_curve);
		void SetSpeed(const float spped);
	private:

		SplineCurve splineCurve;
		float t = 0;
		float speed = 0.0025f;;
	};
	class SucideComponent :public GameComponent
	{
	public:
		SucideComponent(const float arg_count);
		SucideComponent();
		void OnUpdate()override;
		std::string GetGameComponentName()override {
			return "SucideComponent";
		}
		std::shared_ptr<GameComponent> Clone()override;
		void OnShowUI()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(count);
			archive(shp_timer);
			archive(isActive);
		}
	private:
		std::shared_ptr<RelativeTimer> shp_timer;
		float count;
	};
	class UIComponent :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "UIComponent";
		}
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(relativePos);
			archive(relativeScale);
		}
		void OnShowUI()override;
		void OnUpdate()override {}
		void OnSet()override;
		void Start()override;
		void SetPosision(const Vector2& arg_relativePos);
		void SetScalse(const Vector2& arg_relativeScale);
	protected:
		void SetRelativeTransform();
		Vector2 relativePos;
		Vector2 relativeScale = Vector2(1, 1);
	};
	class ModelAnimation;

	class ModelDrawData;
	class SimpleBoneAnimatorComponent :public GameComponent {
	public:
		SimpleBoneAnimatorComponent(std::shared_ptr< ModelDrawData > arg_shp_modelData);
		SimpleBoneAnimatorComponent() {}
		void OnUpdate()override;
		void AddAnimation(std::shared_ptr<ModelAnimation> arg_shp_animation);
		void AddAnimation(MotionTag arg_motionTag);
		void SetLoop(const bool arg_isLoop);
		void OnShowUI()override;
		std::string GetGameComponentName()override {
			return "SimpleBoneAnimatorComponent";
		}
		std::shared_ptr<GameComponent> Clone()override;
	private:
		std::shared_ptr<ModelAnimation> shp_animation;
		std::shared_ptr< ModelDrawData > shp_modelData;
	};

	class MeshDrawComponent :public GameComponent
	{
	public:
		MeshDrawComponent(const MeshTag& arg_meshTag, const ShaderTag& arg_shaderTag, const MaterialTag& arg_materialTag, std::shared_ptr< DrawInformation >arg_shp_drawInfo = nullptr, const UINT arg_layer = 0, std::shared_ptr<Transform> arg_shp_transform = nullptr);
		MeshDrawComponent(const MeshTag& arg_meshTag, const ShaderTag& arg_shaderTag, const std::vector< MaterialTag>& arg_materialTag, std::shared_ptr< DrawInformation >arg_shp_drawInfo = nullptr, const UINT arg_layer = 0, std::shared_ptr<Transform> arg_shp_transform = nullptr);
		MeshDrawComponent(const ModelTag& arg_modelTag, const ShaderTag& arg_shaderTag, std::shared_ptr< DrawInformation >arg_shp_drawInfo = nullptr, const UINT arg_layer = 0, std::shared_ptr<Transform> arg_shp_transform = nullptr);
		MeshDrawComponent() {}
		std::string GetGameComponentName() {
			return "MeshDraw";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnRemove() override;
		void SetBlendMode(const BlendMode& arg_blendMode);
		void SetMaterialTag(MaterialTag  arg_materialTag);
		void SetMeshTag(MeshTag  arg_meshTag);
		void SetModelTag(ModelTag  arg_modelTag);
		void SetShaderTag(ShaderTag  arg_shaderTag);
		std::shared_ptr<GameComponent> Clone()override;
		void Regist();
		void ReRegist();
		void UnRegist();
		std::shared_ptr< DrawInformation > GetDrawInformation();
		void OnShowUI()override;

		template <class T>
		std::shared_ptr<CBuffer< T>> CreateCBuffer(const std::string& arg_cBufferName, const UINT arg_slot, std::weak_ptr<GraphicDevice> arg_wkp_graphicDevice);
		template <class T>
		std::shared_ptr<CBuffer<T>> GetCBuffer(const std::string& arg_cBufferName) {
			return data->GetThis<DrawData>()->GetCBuffer<T>(arg_cBufferName);
		}

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(layer);
			archive(meshTag);
			archive(shaderTag);
			archive(modelTag);
			archive(materialTag);
			archive(isActive);
			archive(shp_transform);
			archive(shp_drawInfo);
		}
	protected:
		std::shared_ptr< MeshDrawData > data;
		virtual void CreateData();
		UINT* index;
		UINT layer;
		MeshTag meshTag;
		ShaderTag shaderTag;
		ModelTag modelTag;
		std::shared_ptr<Transform> shp_transform;
		std::shared_ptr< DrawInformation >shp_drawInfo = nullptr;
		std::vector<MaterialTag> materialTag;

		bool isCereal = true;
	};
	class MeshDrawComponent_Static :public MeshDrawComponent
	{
	public:
		MeshDrawComponent_Static(const MeshTag& arg_meshTag, const ShaderTag& arg_shaderTag, const MaterialTag& arg_materialTag, std::shared_ptr< DrawInformation >arg_shp_drawInfo = nullptr, const UINT arg_layer = 0, std::shared_ptr<Transform> arg_shp_transform = nullptr);
		MeshDrawComponent_Static(const MeshTag& arg_meshTag, const ShaderTag& arg_shaderTag, const std::vector< MaterialTag>& arg_materialTag, std::shared_ptr< DrawInformation >arg_shp_drawInfo = nullptr, const UINT arg_layer = 0, std::shared_ptr<Transform> arg_shp_transform = nullptr);
		MeshDrawComponent_Static(const ModelTag& arg_modelTag, const ShaderTag& arg_shaderTag, std::shared_ptr< DrawInformation >arg_shp_drawInfo = nullptr, const UINT arg_layer = 0, std::shared_ptr<Transform> arg_shp_transform = nullptr);
		MeshDrawComponent_Static() {}
		std::string GetGameComponentName() {
			return "MeshDraw_Static";
		}
		void OnUpdate()override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(layer);
			archive(meshTag);
			archive(shaderTag);
			archive(modelTag);
			archive(materialTag);
			archive(isActive);
			archive(shp_transform);
			archive(shp_drawInfo);
		}
	protected:
	};
}


BUTI_REGIST_GAMECOMPONENT(ButiEngine::UIComponent)
BUTI_REGIST_GAMECOMPONENT(ButiEngine::SimpleBoneAnimatorComponent)

BUTI_REGIST_GAMECOMPONENT(ButiEngine::MeshDrawComponent)
BUTI_REGIST_GAMECOMPONENT(ButiEngine::MeshDrawComponent_Static)
BUTI_REGIST_GAMECOMPONENT(ButiEngine::Collision::ColliderComponent)
BUTI_REGIST_GAMECOMPONENT(ButiEngine::SucideComponent)
BUTI_REGIST_GAMECOMPONENT(ButiEngine::TransformAnimation)


//å„Ç≈å¬ï Ç….hÇçÏÇÈÇ‚Ç¬ÇÁ

BUTI_REGIST_GAMECOMPONENT(ButiEngine::CameraMan)
BUTI_REGIST_GAMECOMPONENT(ButiEngine::CubeTransformAnimation)

BUTI_REGIST_GAMECOMPONENT(ButiEngine::FPSViewBehavior);

BUTI_REGIST_GAMECOMPONENT(ButiEngine::IKComponent)

BUTI_REGIST_GAMECOMPONENT(ButiEngine::ImmediateParticleController)
BUTI_REGIST_GAMECOMPONENT(ButiEngine::LookAtComponent)
BUTI_REGIST_GAMECOMPONENT(ButiEngine::PostEffectParamUpdater)
BUTI_REGIST_GAMECOMPONENT(ButiEngine::SplineCurveMover)
BUTI_REGIST_GAMECOMPONENT(ButiEngine::UIAnimation)
BUTI_REGIST_GAMECOMPONENT(ButiEngine::ChaseComponent)

