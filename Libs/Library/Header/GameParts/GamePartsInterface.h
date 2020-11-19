#include"stdafx.h"

namespace ButiEngine 
{
	class IDrawObject;
	class IScene;
	class Application;
	template<typename T>
	class CBuffer_Dx12;
	class IRenderer :public IObject {
	public:
		void PreInitialize()override {}
		virtual void Update() = 0;
		virtual void RenderingStart() = 0;
		virtual virtual void BefRendering() = 0;
		virtual void AddLayer() = 0;
		virtual UINT GetLayerCount() = 0;
		virtual void Rendering(const UINT arg_layer) = 0;
		virtual void RenderingEnd() = 0;
		virtual void ClearDrawObjects() = 0;
		virtual void Draw(const MeshTag& arg_meshTag) = 0;
		virtual std::weak_ptr<IScene> GetScene() = 0;
		virtual void TextureAttach(const TextureTag& arg_textureTag, const UINT slot) = 0;
		virtual void ShaderAttach(const ShaderTag& arg_shaderTag) = 0;
		virtual void MaterialAttach(const MaterialTag& arg_materialTag) = 0;
		virtual UINT* RegistDrawObject(std::weak_ptr< IDrawObject> arg_wkp_drawObject,const bool arg_afterDraw, const UINT arg_layer = 0) = 0;

		virtual void UnRegistDrawObject(UINT* arg_index, const bool arg_afterDraw, const UINT arg_layer = 0) = 0;

		virtual void Release() = 0;
		virtual void UpdateFog(const Fog& arg_param) = 0;
		virtual void ReleaseFogBuffer() = 0;
		virtual std::shared_ptr<CBuffer_Dx12<Fog>> GetFogCBuffer() = 0;
	};
	class ISoundManager :public IObject {

	public:
		void PreInitialize()override {}
		virtual void StopCheck() = 0;
		virtual void Update() = 0;
		virtual void ClearCheck() = 0;
		virtual void Play(SoundTag tag) = 0;
		virtual void PlayBGM(SoundTag tag) = 0;
		virtual void Release() = 0;
	};

	class ISceneManager :public IObject {

	public:

		virtual ~ISceneManager() {}
		virtual inline void Update() = 0;
		virtual void LoadScene(const std::string& arg_sceneName, std::shared_ptr<IScene> shp_scene) = 0;
		virtual void RemoveScene(const std::string& arg_sceneName) = 0;
		virtual void ChangeScene(std::string arg_sceneName, float sceneChangeDalay = 0) = 0;
		virtual void SetScene_Init(const std::string& arg_sceneName, std::shared_ptr<IScene> shp_scene) = 0;
		virtual void RenewalScene() = 0;
		virtual inline std::shared_ptr<IScene> GetCurrentScene() = 0;
		virtual std::weak_ptr<Application> GetApplication() = 0;
		virtual void Release() = 0;
		virtual inline void Draw() = 0;


	};
}