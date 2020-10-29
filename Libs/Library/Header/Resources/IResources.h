#include"stdafx.h"

namespace ButiEngine {
	class IResource_Mesh;
	class IResource_Motion;
	class IResource_VertexShader;
	class IResource_PixelShader;
	class IResource_GeometryShader;
	class IResource_Shader;
	class IResource_Sound;
	class IResource_Texture;
	class IResource_Model;
	class IResource_Material;
	class IRenderer;


	using MeshTag = ID<IResource_Mesh>;
	using MotionTag = ID<IResource_Motion>;
	
	using VertexShaderTag = ID<IResource_VertexShader>;
	using PixelShaderTag = ID<IResource_PixelShader>;
	using GeometryShaderTag = ID<IResource_GeometryShader>;
	using ShaderTag = ID<IResource_Shader>;
	using SoundTag = ID<IResource_Sound>;
	using TextureTag = ID<IResource_Texture>;
	using ModelTag = ID<IResource_Model>;

	using MaterialTag = ID<IResource_Material>;

	class ModelAnimation;
	class MotionKeyFrameData;
	class BackupDataBase;
	struct Bone;
	struct WaveDatas;
	namespace Morph{
		class Morph;
	}

	class IResource_Motion :public IObject {
	public:
		virtual std::shared_ptr<ModelAnimation> GetAnimation() = 0;
		virtual void AddKeyFrame(const std::wstring& arg_boneName, const MotionKeyFrameData& arg_data) = 0;
		virtual void AddKeyFrameLane(const std::wstring& arg_boneName, const std::vector<MotionKeyFrameData>& arg_datas) = 0;
	};
	class IResource_VertexShader :public IObject {
	public:
		virtual void Attach()const = 0;
		virtual void SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice) = 0;
	};
	class IResource_PixelShader :public IObject {
	public:
		virtual void Attach()const = 0;
		virtual void SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice) = 0;
	};
	class IResource_GeometryShader :public IObject {
	public:
		virtual void Attach()const = 0;
		virtual void SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice) = 0;
	};

	class IResource_Shader :public IObject {
	public:
		virtual void Attach()const = 0;

		virtual bool GetIsContainGeometryShader()const = 0;

		virtual std::string GetShaderName()const = 0;

		virtual std::weak_ptr<IResource_VertexShader> GetVertexShader() = 0;
		virtual std::weak_ptr<IResource_PixelShader> GetPixelShader() = 0;
		virtual std::weak_ptr<IResource_GeometryShader> GetGeometryShader() = 0;
	};
	class IResource_Sound :public IObject {
	public:
		virtual std::shared_ptr<WaveDatas> GetWavDatas() = 0;
	};

	class IResource_Texture :public IObject {
	public:
		virtual void SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice) = 0;
		virtual void Attach(int slot) = 0;
		virtual UInt2 GetSize() const = 0;
		virtual void SetFilePath(const std::string& arg_textureFilePath) = 0;
		virtual const std::string& GetTexturePath() = 0;
	};
	class IResource_Material :public IObject {
	public:
		virtual void Attach(std::shared_ptr<IRenderer> arg_shp_renderer) = 0;
		virtual const std::string& GetMaterialName() = 0;
		virtual const std::string& GetMaterialNameEng() = 0;
		virtual const std::string& GetMaterialComment() = 0;
		virtual void SetMaterialName(const std::string& arg_name) = 0;
		virtual void SetMaterialNameEng(const std::string& arg_name) = 0;
		virtual void SetComment(const std::string& arg_comment) = 0;
		virtual MaterialVariable GetMaterialVariable() const = 0;
		virtual std::shared_ptr<IResource_Texture> GetTexture() = 0;
		virtual std::shared_ptr<IResource_Texture> GetSphereTexture() = 0;
		virtual TextureTag GetSphereTextureTag() = 0;
		virtual TextureTag GetTextureTag() = 0;
	};
	class IResource_Mesh :public IObject {
	public:
		virtual void SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice) = 0;
		virtual void SetBackupData(std::shared_ptr<BackupDataBase> arg_backupData) = 0;
		virtual void Draw() = 0;
		virtual UINT GetVertexCount() = 0;
		virtual UINT GetIndexCount() = 0;
		virtual const BackupDataBase& GetBackUpdata()const = 0;
	};
	class IResource_Model :public IObject {
	public:
		virtual void SetMeshTag(const MeshTag& arg_meshTag) = 0;
		virtual void SetMaterialVec(const std::vector<MaterialTag>& arg_materialTags) = 0;
		virtual void SetName(const std::wstring& arg_name) = 0;
		virtual void SetEngName(const std::wstring& arg_engName) = 0;
		virtual void SetModelName(const std::wstring& arg_modelName) = 0;
		virtual void SetEngModelName(const std::wstring& arg_engModelName) = 0;
		virtual void SetComment(const std::wstring& arg_comment) = 0;
		virtual void SetEngComment(const std::wstring& arg_engComment) = 0;
		virtual void AddBone(Bone& arg_bone) = 0;
		virtual void AddMorph(std::shared_ptr<Morph::Morph> arg_morph) = 0;
		virtual void AddMaterial(const MaterialTag& arg_materialTag) = 0;
		virtual void SetVersion(const float arg_version) = 0;
		virtual void SetSubset(const std::vector<UINT>& arg_subset) = 0;
		virtual const MeshTag GetMeshTag() = 0;
		virtual std::vector<MaterialTag>& GetMaterialTags() = 0;

		virtual std::vector<UINT> GetSubset() = 0;
		virtual const std::wstring& GetName() = 0;
		virtual const std::wstring& GetEngName() = 0;
		virtual const std::wstring& GetModelName() = 0;
		virtual const std::wstring& GetEngModelName() = 0;
		virtual const std::wstring& GetComment() = 0;
		virtual const std::wstring& GetEngComment() = 0;
		virtual std::vector<std::shared_ptr<Bone>> GetBone() = 0;
	};
}