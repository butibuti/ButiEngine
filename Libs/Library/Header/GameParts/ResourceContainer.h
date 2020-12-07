#pragma once
#include"stdafx.h"
#include"../Resources/Vertex.h"
#include"ResourceFactory.h"
#include"ResourceFactory_Dx12.h"
namespace ButiEngine {

	class ResourceContainer :public IResourceContainer
	{
	public:

		ResourceContainer();
		void SetGraphicDevice(std::weak_ptr<GraphicDevice> arg_shp_graphicDevice);
		void Initialize()override;
		void PreInitialize()override;

		void ShowGUI()override;

		template<typename T>
		MeshTag LoadMesh(const std::string& filePath, const BackupData<T>& inputMeshData);
		template<typename T>
		MeshTag LoadRealTimeMesh(const std::string& filePath, const BackupData<T>& inputMeshData);


		MaterialTag LoadMaterial(const MaterialVariable& arg_resourceMaterial, const TextureTag& arg_textureTag, const std::string& arg_filePath, const std::string& arg_fileDirectory = "")override;
		MaterialTag LoadMaterial(const std::string& arg_filePath, const std::string& arg_fileDirectory = "")override;
		std::vector < MaterialTag> LoadMaterial(const std::vector<MaterialLoadInfo>& arg_vec_loadInfo)override;
		std::vector < MaterialTag> LoadMaterial(const std::vector<std::string>& arg_vec_filePathAndDirectory)override;
		MaterialTag LoadMaterial(const std::wstring& arg_filePath, const std::string& arg_fileDirectory = "")override;

		TextureTag LoadTexture(const std::string& arg_filePath, const std::string& arg_fileDirectory = "")override;
		std::vector < TextureTag> LoadTexture(const std::vector<std::string>& arg_vec_filePathAndDirectory)override;

		PixelShaderTag LoadPixelShader(const std::string& arg_filePath, const std::string& arg_fileDirectory = "")override;
		std::vector < PixelShaderTag> LoadPixelShader(const std::vector<std::string>& arg_vec_filePathAndDirectory)override;

		VertexShaderTag LoadVertexShader(const std::string& arg_filePath, const std::string& arg_fileDirectory = "")override;
		std::vector < VertexShaderTag> LoadVertexShader(const std::vector<std::string>& arg_vec_filePathAndDirectory)override;

		GeometryShaderTag LoadGeometryShader(const std::string& arg_filePath, const std::string& arg_fileDirectory = "")override;
		std::vector < GeometryShaderTag> LoadGeometryShader(const std::vector<std::string>& arg_vec_filePathAndDirectory)override;

		ShaderTag LoadShader(const  ShaderName& arg_shaderNames)override;
		std::vector < ShaderTag> LoadShader(const std::vector<ShaderName>& arg_vec_shaderNames)override;

		SoundTag LoadSound(const std::string& arg_filePath, const std::string& arg_fileDirectory = "")override;
		std::vector < SoundTag> LoadSound(const std::vector<std::string>& arg_vec_filePathAndDirectory)override;

		ModelTag LoadModel(const std::string& arg_filePath, const std::string& arg_fileDirectory = "")override;
		std::vector<ModelTag> LoadModel(const std::vector<std::string>& arg_vec_filePathAndDirectory)override;
		ModelTag LoadModel(std::shared_ptr<IResource_Model> arg_model, const std::string& arg_filePath, const std::string& arg_fileDirectory = "")override;

		MotionTag LoadMotion(const std::string& arg_filePath, const std::string& arg_fileDirectory = "")override;
		std::vector<MotionTag> LoadMotion(const std::vector<std::string>& arg_vec_filePathAndDirectory)override;
		MotionTag LoadMotion(std::shared_ptr<IResource_Motion> arg_motion, const std::string& arg_filePath, const std::string& arg_fileDirectory = "")override;

		void UnLoadMesh(MeshTag arg_meshTag)override;
		void UnLoadTexture(TextureTag arg_textureTag)override;
		void UnLoadShader(ShaderTag arg_shaderTag)override;
		void UnLoadPixelShader(PixelShaderTag arg_shaderTag)override;
		void UnLoadVertexShader(VertexShaderTag arg_shaderTag)override;
		void UnLoadGeometryShader(GeometryShaderTag arg_shaderTag)override;
		void UnLoadSound(SoundTag arg_soundTag)override;
		void UnLoadModel(ModelTag arg_modelTag)override;
		void UnLoadMaterial(MaterialTag arg_materialTag)override;
		
		std::string GetTagNameMesh(MeshTag arg_meshTag)override;
		std::string GetTagNameTexture(TextureTag arg_textureTag)override;
		std::string GetTagNameShader(ShaderTag arg_shaderTag)override;
		std::string GetTagNamePixelShader(PixelShaderTag arg_shaderTag)override;
		std::string GetTagNameVertexShader(VertexShaderTag arg_shaderTag)override;
		std::string GetTagNameGeometryShader(GeometryShaderTag arg_shaderTag)override;
		std::string GetTagNameSound(SoundTag arg_soundTag)override;
		std::string GetTagNameModel(ModelTag arg_modelTag)override;
		std::string GetTagNameMaterial(MaterialTag arg_materiallTag)override;
		void Release()override;

		void MaterialUpdate()override;

		MeshTag GetMeshTag(const std::string& arg_key, const std::string& arg_fileDirectory = "")override;
		TextureTag GetTextureTag(const std::string& arg_key, const std::string& arg_fileDirectory = "")override;
		ShaderTag GetShaderTag(const std::string& arg_key)override;
		SoundTag GetSoundTag(const std::string& arg_key, const std::string& arg_fileDirectory = "")override;
		MaterialTag GetMaterialTag(const std::string& arg_key, const std::string& arg_fileDirectory = "")override;
		ModelTag GetModelTag(const std::string& arg_key, const std::string& arg_fileDirectory = "")override;
		MotionTag GetMotionTag(const std::string& arg_key, const std::string& arg_fileDirectory = "")override;

		std::weak_ptr<IResource_Mesh> GetMesh(const  MeshTag& arg_key)override {
			return container_meshes.GetValue(arg_key);
		}
		std::weak_ptr<IResource_Texture> GetTexture(const TextureTag & arg_key) override {
			return container_textures.GetValue(arg_key);
		}
		std::weak_ptr<IResource_Shader> GetShader(const ShaderTag & arg_key)override {
			return container_shaders.GetValue(arg_key);
		}
		std::weak_ptr<IResource_Sound> GetSound(const SoundTag & arg_key) override {
			return container_sounds.GetValue(arg_key);
		}
		std::weak_ptr<IResource_Material>GetMaterial(const MaterialTag& arg_key) override {
			return container_materials.GetValue(arg_key);
		}
		std::weak_ptr<IResource_Model>GetModel(const ModelTag &arg_key) {
			return container_models.GetValue(arg_key);
		}
		std::shared_ptr<ModelAnimation>GetMotion(const MotionTag &arg_key) override {
			
			auto motion= container_motions.GetValue(arg_key);

			if (motion) {
				return motion->GetAnimation();
			}
			else {
				return nullptr;
			}
		}

		void Reload()override;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(container_meshes);
			archive(container_textures);
			archive(container_materials);
			archive(container_shaders);
			archive(container_vertexShaders);
			archive(container_geometryShaders);
			archive(container_pixelShaders);
			archive(container_motions);
			archive(container_sounds);
			archive(container_models);

			archive(vec_filePathAndDirectory_tex);
			archive(vec_filePathAndDirectory_ps);
			archive(vec_filePathAndDirectory_vs);
			archive(vec_filePathAndDirectory_gs);
			archive(vec_filePathAndDirectory_sound);
			archive(vec_filePathAndDirectory_model);
			archive(vec_filePathAndDirectory_motion);
			archive(vec_shaderNames);
			archive(vec_materialLoadInfos);
		}

	private:

		//backupdata
		std::vector<std::string> vec_filePathAndDirectory_tex;
		std::vector<std::string> vec_filePathAndDirectory_ps;
		std::vector<std::string> vec_filePathAndDirectory_vs;
		std::vector<std::string> vec_filePathAndDirectory_gs;
		std::vector<std::string> vec_filePathAndDirectory_sound;
		std::vector<std::string> vec_filePathAndDirectory_model;
		std::vector<std::string> vec_filePathAndDirectory_motion;
		std::vector<ShaderName> vec_shaderNames;
		std::vector<MaterialLoadInfo> vec_materialLoadInfos;


		std::weak_ptr<GraphicDevice> wkp_graphicDevice;

		IDContainer<IResource_Mesh> container_meshes;

		IDContainer<IResource_Texture> container_textures;

		IDContainer<IResource_Shader> container_shaders;
		IDContainer<IResource_PixelShader> container_pixelShaders;
		IDContainer<IResource_VertexShader> container_vertexShaders;
		IDContainer<IResource_GeometryShader> container_geometryShaders;

		IDContainer<IResource_Motion> container_motions;

		IDContainer<IResource_Sound> container_sounds;

		IDContainer<IResource_Material> container_materials;

		IDContainer<IResource_Model> container_models;


		std::shared_ptr<ResourceFactory> unq_resourceFactory = nullptr;

	};

	template<typename T>
	inline MeshTag ButiEngine::ResourceContainer::LoadMesh(const std::string& filePath, const BackupData<T>& inputMeshData)
	{
		return container_meshes.AddValue(unq_resourceFactory->GetThis<ResourceFactory_Dx12>()->CreateMesh(inputMeshData), filePath);
	}
	template<typename T>
	inline MeshTag ButiEngine::ResourceContainer::LoadRealTimeMesh(const std::string& filePath, const BackupData<T>& inputMeshData)
	{
		return container_meshes.AddValue(unq_resourceFactory->GetThis<ResourceFactory_Dx12>()->CreateRealTimeMesh(inputMeshData), filePath);
	}

	void OutputCereal(const std::shared_ptr<ResourceContainer>& v);

	void InputCereal(std::shared_ptr<ResourceContainer>& v,  std::weak_ptr<GraphicDevice> arg_shp_graphicDevice);
}

