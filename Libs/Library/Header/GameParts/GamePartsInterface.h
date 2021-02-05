#ifndef GamePartInterface
#define GamePartInterface

#include"../Device/ObjectFactory.h"

namespace ButiEngine 
{
	namespace Collision {
		class CollisionPrimitive;
	}

	class GlobalSettings {
	public:

		static std::string GetResourceDirectory() {
			return resourceDirectory;
		}

	private:
		static std::string resourceDirectory;
	};
	enum class WindowPopType {
		normal = SW_SHOWNORMAL, max = SW_SHOWMAXIMIZED
	};
	class IDrawObject;
	class IScene;
	class IApplication;
	class ImguiController;
	template<typename T>
	class CBuffer_Dx12;
	class SceneInformation;

	class ICollisionManager :public IObject
	{
	public:
		void Initialize()override {}
		void PreInitialize()override {}
		virtual void Update()= 0;
		virtual UINT* RegistCollisionObject(const int layerNum, std::shared_ptr< Collision::CollisionPrimitive>arg_prim, std::shared_ptr<GameObject> arg_registObj)= 0;
		virtual void UnRegistCollisionObject(const int layerNum, UINT* registNum)= 0;
		virtual UINT GetLayerCount()= 0;
		virtual void AddLayer(const Vector3& size, const int level)= 0;
		virtual void RemoveLayer(const int arg_layer)= 0;
		virtual bool IsWillHit(std::shared_ptr< Collision::CollisionPrimitive>arg_prim, int arg_layer)= 0;
		virtual void ShowGUI()= 0;
	};

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
		virtual void MaterialAttach(const UINT slotOffset, const MaterialTag& arg_materialTag) = 0;
		virtual UINT* RegistDrawObject(std::weak_ptr< IDrawObject> arg_wkp_drawObject,const bool arg_afterDraw, const UINT arg_layer = 0) = 0;

		virtual void UnRegistDrawObject(UINT* arg_index, const bool arg_afterDraw, const UINT arg_layer = 0) = 0;

		virtual void Release() = 0;
		virtual void UpdateFog(const Fog& arg_param) = 0;
		virtual void ReleaseFogBuffer() = 0;
		virtual std::shared_ptr<CBuffer_Dx12<Fog>> GetFogCBuffer() = 0;

		virtual std::vector< std::shared_ptr<IDrawObject>> GetHitDrawObjects(std::shared_ptr<Collision::CollisionPrimitive> arg_prim, const int arg_layer)=0;
	};


	class IWindow
	{
	public:
		virtual void Initialize(const std::string arg_windowName, const WindowPopType arg_popType, bool isFullScreen, UINT width = 0, UINT height = 0)=0;
		virtual HWND GetHandle()=0;
		virtual DirectX::XMUINT2 GetSize()=0;
		virtual Vector2 GetWindowCenterPosition()=0;
		virtual void SetSize(int width, int height)=0;
		virtual bool Update()=0;
		virtual Vector2 GetWindowPosition()=0;
		virtual void Release()=0;
	private:

	};

	class ISoundManager :public IObject {

	public:
		void PreInitialize()override {}
		virtual void StopCheck() = 0;
		virtual void Update() = 0;
		virtual void ClearCheck() = 0;
		virtual void Play(SoundTag tag, float valume) = 0;
		virtual void PlayBGM(SoundTag tag, float valume) = 0;
		virtual void Release() = 0;
	};

	class ISceneManager :public IObject {

	public:

		virtual ~ISceneManager() {}
		virtual inline void Update() = 0;
		virtual void SetScene(const std::string& arg_sceneName, std::shared_ptr<IScene> shp_scene) = 0;
		virtual void SetScene_Init(const std::string& arg_sceneName, std::shared_ptr<IScene> shp_scene ) = 0;
		virtual void LoadScene(const std::string& arg_sceneName, std::shared_ptr<SceneInformation> shp_scene = nullptr) = 0;
		virtual void LoadScene_Init(const std::string& arg_sceneName, std::shared_ptr<SceneInformation> shp_scene = nullptr) = 0;
		virtual void RemoveScene(const std::string& arg_sceneName) = 0;
		virtual void ChangeScene(const std::string& arg_sceneName, float sceneChangeDalay = 0) = 0;
		virtual void RenewalScene() = 0;
		virtual void ReloadScene() = 0;
		virtual void ReloadScene(const std::string& arg_sceneName)=0;
		virtual inline std::shared_ptr<IScene> GetCurrentScene() = 0;
		virtual std::weak_ptr<IApplication> GetApplication() = 0;
		virtual void Release() = 0;


	};

	class IResourceContainer :public IObject
	{
	public:
		struct ShaderName {
			std::string shaderName;
			std::string vertexShaderName;
			std::string pixelShaderName;
			std::string geometryShaderName = "none";
			template<class Archive>
			void serialize(Archive& archive)
			{
				archive(shaderName);
				archive(vertexShaderName);
				archive(pixelShaderName);
				archive(geometryShaderName);
			}
		};
		struct MaterialLoadInfo {
			MaterialLoadInfo(std::string arg_fileName) {
				fileName = arg_fileName;
			}
			MaterialLoadInfo(){}
			std::string materialName;
			std::string fileName="none";
			MaterialVariable var;
			std::vector<TextureTag> vec_texture;
			template<class Archive>
			void serialize(Archive& archive)
			{
				archive(materialName);
				archive(fileName);
				archive(var);
				archive(vec_texture);
			}

		};

		virtual void SetGraphicDevice(std::weak_ptr<GraphicDevice> arg_shp_graphicDevice)=0;

		virtual void ShowGUI()=0;



		virtual MaterialTag LoadMaterial(const MaterialVariable& arg_resourceMaterial, const std::vector< TextureTag>& arg_textureTag, const std::string& arg_filePath, const std::string& arg_fileDirectory = "") =0;
		virtual MaterialTag LoadMaterial(const std::string& arg_filePath, const std::string& arg_fileDirectory = "")=0;

		virtual std::vector < MaterialTag> LoadMaterial(const std::vector<std::string>& arg_vec_filePathAndDirectory) = 0;
		virtual std::vector < MaterialTag> LoadMaterial(const std::vector<MaterialLoadInfo>& arg_vec_loadInfo)=0;
		virtual MaterialTag LoadMaterial(const std::wstring& arg_filePath, const std::string& arg_fileDirectory = "")=0;

		virtual TextureTag LoadTexture(const std::string& arg_filePath, const std::string& arg_fileDirectory = "")=0;
		virtual std::vector < TextureTag> LoadTexture(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;

		virtual PixelShaderTag LoadPixelShader(const std::string& arg_filePath, const std::string& arg_fileDirectory = "")=0;
		virtual std::vector < PixelShaderTag> LoadPixelShader(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;

		virtual VertexShaderTag LoadVertexShader(const std::string& arg_filePath, const std::string& arg_fileDirectory = "")=0;
		virtual std::vector < VertexShaderTag> LoadVertexShader(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;

		virtual GeometryShaderTag LoadGeometryShader(const std::string& arg_filePath, const std::string& arg_fileDirectory = "")=0;
		virtual std::vector < GeometryShaderTag> LoadGeometryShader(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;

		virtual ShaderTag LoadShader(const  ShaderName& arg_shaderNames)=0;
		virtual std::vector < ShaderTag> LoadShader(const std::vector<ShaderName>& arg_vec_shaderNames)=0;

		virtual SoundTag LoadSound(const std::string& arg_filePath, const std::string& arg_fileDirectory = "")=0;
		virtual std::vector < SoundTag> LoadSound(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;

		virtual ModelTag LoadModel(const std::string& arg_filePath, const std::string& arg_fileDirectory = "")=0;
		virtual std::vector<ModelTag> LoadModel(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;
		virtual ModelTag LoadModel(std::shared_ptr<IResource_Model> arg_model, const std::string& arg_filePath, const std::string& arg_fileDirectory = "")=0;

		virtual MotionTag LoadMotion(const std::string& arg_filePath, const std::string& arg_fileDirectory = "")=0;
		virtual std::vector<MotionTag> LoadMotion(const std::vector<std::string>& arg_vec_filePathAndDirectory)=0;
		virtual MotionTag LoadMotion(std::shared_ptr<IResource_Motion> arg_motion, const std::string& arg_filePath, const std::string& arg_fileDirectory = "")=0;

		virtual void UnLoadMesh(MeshTag arg_meshTag)=0;
		virtual void UnLoadTexture(TextureTag arg_textureTag)=0;
		virtual void UnLoadShader(ShaderTag arg_shaderTag)=0;
		virtual void UnLoadPixelShader(PixelShaderTag arg_shaderTag)=0;
		virtual void UnLoadVertexShader(VertexShaderTag arg_shaderTag)=0;
		virtual void UnLoadGeometryShader(GeometryShaderTag arg_shaderTag)=0;
		virtual void UnLoadSound(SoundTag arg_soundTag)=0;
		virtual void UnLoadModel(ModelTag arg_modelTag)=0;
		virtual void UnLoadMaterial(MaterialTag arg_materialTag)=0;

		virtual std::string GetTagNameMesh(MeshTag arg_meshTag)=0;
		virtual std::string GetTagNameTexture(TextureTag arg_textureTag)=0;
		virtual std::string GetTagNameShader(ShaderTag arg_shaderTag)=0;
		virtual std::string GetTagNamePixelShader(PixelShaderTag arg_shaderTag)=0;
		virtual std::string GetTagNameVertexShader(VertexShaderTag arg_shaderTag)=0;
		virtual std::string GetTagNameGeometryShader(GeometryShaderTag arg_shaderTag)=0;
		virtual std::string GetTagNameSound(SoundTag arg_soundTag)=0;
		virtual std::string GetTagNameModel(ModelTag arg_modelTag)=0;
		virtual std::string GetTagNameMaterial(MaterialTag arg_materiallTag)=0;
		virtual void Release()=0;

		virtual void MaterialUpdate()=0;

		virtual MeshTag GetMeshTag(const std::string& arg_key, const std::string& arg_fileDirectory = "") = 0;
		virtual TextureTag GetTextureTag(const std::string& arg_key, const std::string& arg_fileDirectory = "") = 0;
		virtual ShaderTag GetShaderTag(const std::string& arg_key) = 0;
		virtual SoundTag GetSoundTag(const std::string& arg_key, const std::string& arg_fileDirectory = "") = 0;
		virtual MaterialTag GetMaterialTag(const std::string& arg_key, const std::string& arg_fileDirectory = "") = 0;
		virtual ModelTag GetModelTag(const std::string& arg_key, const std::string& arg_fileDirectory = "") = 0;
		virtual MotionTag GetMotionTag(const std::string& arg_key, const std::string& arg_fileDirectory = "") = 0;

		virtual MeshTag GetMeshTag(const MeshTag& arg_tag) = 0;
		virtual TextureTag GetTextureTag(const TextureTag& arg_tag) = 0;
		virtual ShaderTag GetShaderTag(const ShaderTag& arg_tag) = 0;
		virtual SoundTag GetSoundTag(const SoundTag& arg_tag) = 0;
		virtual MaterialTag GetMaterialTag(const MaterialTag& arg_tag) = 0;
		virtual ModelTag GetModelTag(const ModelTag& arg_tag) = 0;
		virtual MotionTag GetMotionTag(const MotionTag& arg_tag) = 0;

		virtual std::weak_ptr<IResource_Mesh> GetMesh(const  MeshTag& arg_key)=0;
		virtual std::weak_ptr<IResource_Texture> GetTexture(const TextureTag& arg_key)=0;
		virtual std::weak_ptr<IResource_Shader> GetShader(const ShaderTag& arg_key)=0;
		virtual std::weak_ptr<IResource_Sound> GetSound(const SoundTag& arg_key)=0;
		virtual std::weak_ptr<IResource_Material>GetMaterial(const MaterialTag& arg_key)=0;
		virtual std::weak_ptr<IResource_Model>GetModel(const ModelTag& arg_key)=0;
		virtual std::shared_ptr<ModelAnimation>GetMotion(const MotionTag& arg_key)=0;

		virtual void Reload()=0;

	};


	class IApplication :public IObject
	{
	public:
		virtual std::unique_ptr<IWindow>& GetWindow()=0;
		virtual std::shared_ptr<ISceneManager> GetSceneManager()=0;
		virtual std::shared_ptr<GraphicDevice> GetGraphicDevice()=0;
		virtual std::shared_ptr<IResourceContainer> GetResourceContainer()=0;
		virtual std::unique_ptr<ImguiController>& GetGUIController()=0;
		virtual std::shared_ptr<GameObjectTagManager> GetGameObjectTagManager()=0;
		virtual bool Update()=0;
		virtual int Run()=0;
		virtual void InitLoadResources()=0;
		virtual void Exit()=0;
	};
}

#endif // !GamePartInterface