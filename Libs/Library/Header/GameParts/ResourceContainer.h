#pragma once
#include"stdafx.h"
#include"../Resources/Vertex.h"
#include"ResourceFactory.h"
#include"ResourceFactory_Dx12.h"
namespace ButiEngine {

	class ResourceContainer :public IObject
	{
	public:
		struct ShaderName {
			std::string shaderName;
			std::string vertexShaderName;
			std::string pixelShaderName;
			std::string vertexShaderDirectory = "";
			std::string pixelShaderDirectory = "";
			std::string geometryShaderName = "none";
			std::string geometryShaderDirectory = "";
		};
		ResourceContainer(std::weak_ptr<GraphicDevice> shp_graphicDevice);
		void Initialize()override;
		void PreInitialize()override;
		template<typename T>
		MeshTag LoadMesh(const std::string& filePath, const BackupData<T>& inputMeshData);


		MaterialTag LoadMaterial(const MaterialVariable& arg_resourceMaterial, const TextureTag& arg_textureTag, const std::string& arg_filePath, const std::string& arg_fileDirectory = "");
		MaterialTag LoadMaterial(const std::string& arg_filePath, const std::string& arg_fileDirectory = "");

		std::vector < MaterialTag> LoadMaterial(const std::vector<std::pair< std::string, std::string>>& arg_vec_filePathAndDirectory);
		MaterialTag LoadMaterial(const std::wstring& arg_filePath, const std::string& arg_fileDirectory = "");

		TextureTag LoadTexture(const std::string& arg_filePath, const std::string& arg_fileDirectory = "");
		std::vector < TextureTag> LoadTexture(const std::vector<std::pair< std::string, std::string>>& arg_vec_filePathAndDirectory);

		PixelShaderTag LoadPixelShader(const std::string& arg_filePath, const std::string& arg_fileDirectory = "");
		std::vector < PixelShaderTag> LoadPixelShader(const std::vector<std::pair< std::string, std::string>>& arg_vec_filePathAndDirectory);

		VertexShaderTag LoadVertexShader(const std::string& arg_filePath, const std::string& arg_fileDirectory = "");
		std::vector < VertexShaderTag> LoadVertexShader(const std::vector<std::pair< std::string, std::string>>& arg_vec_filePathAndDirectory);

		GeometryShaderTag LoadGeometryShader(const std::string& arg_filePath, const std::string& arg_fileDirectory = "");
		std::vector < GeometryShaderTag> LoadGeometryShader(const std::vector<std::pair< std::string, std::string>>& arg_vec_filePathAndDirectory);

		ShaderTag LoadShader(const  ShaderName& arg_shaderNames);
		std::vector < ShaderTag> LoadShader(const std::vector<ShaderName>& arg_vec_shaderNames);

		SoundTag LoadSound(const std::string& arg_filePath, const std::string& arg_fileDirectory = "");
		std::vector < SoundTag> LoadSound(const std::vector<std::pair< std::string, std::string>>& arg_vec_filePathAndDirectory);

		ModelTag LoadModel(const std::string& arg_filePath, const std::string& arg_fileDirectory = "");
		std::vector<ModelTag> LoadModel(const std::vector<std::pair< std::string, std::string>>& arg_vec_filePathAndDirectory);
		ModelTag LoadModel(std::shared_ptr<Resource_Model> arg_model, const std::string& arg_filePath, const std::string& arg_fileDirectory = "");

		MotionTag LoadMotion(const std::string& arg_filePath, const std::string& arg_fileDirectory = "");
		std::vector<MotionTag> LoadMotion(const std::vector<std::pair< std::string, std::string>>& arg_vec_filePathAndDirectory);
		MotionTag LoadMotion(std::shared_ptr<Resource_Motion> arg_motion, const std::string& arg_filePath, const std::string& arg_fileDirectory = "");

		void UnLoadMesh(MeshTag arg_meshTag);
		void UnLoadTexture(TextureTag arg_textureTag);
		void UnLoadShader(ShaderTag arg_shaderTag);
		void UnLoadSound(SoundTag arg_soundTag);
		void Release();

		MeshTag GetMeshTag(const std::string& arg_key, const std::string& arg_fileDirectory = "");
		TextureTag GetTextureTag(const std::string& arg_key, const std::string& arg_fileDirectory = "");
		ShaderTag GetShaderTag(const std::string& arg_key);
		SoundTag GetSoundTag(const std::string& arg_key, const std::string& arg_fileDirectory = "");
		MaterialTag GetMaterialTag(const std::string& arg_key, const std::string& arg_fileDirectory = "");
		ModelTag GetModelTag(const std::string& arg_key, const std::string& arg_fileDirectory = "");
		MotionTag GetMotionTag(const std::string& arg_key, const std::string& arg_fileDirectory = "");

		inline std::weak_ptr<IResource_Mesh> GetMesh(const  MeshTag& arg_key) {
			return container_meshes.GetValue(arg_key);
		}
		inline std::weak_ptr<IResource_Texture> GetTexture(const TextureTag & arg_key) {
			return container_textures.GetValue(arg_key);
		}
		inline std::weak_ptr<IResource_Shader> GetShader(const ShaderTag & arg_key) {
			return container_shaders.GetValue(arg_key);
		}
		inline std::weak_ptr<IResource_Sound> GetSound(const SoundTag & arg_key) {
			return container_sounds.GetValue(arg_key);
		}
		inline std::weak_ptr<IResource_Material>GetMaterial(const MaterialTag& arg_key) {
			return container_materials.GetValue(arg_key);
		}
		inline std::weak_ptr<IResource_Model>GetModel(const ModelTag &arg_key) {
			return container_models.GetValue(arg_key);
		}
		inline std::shared_ptr<ModelAnimation>GetMotion(const MotionTag &arg_key) {
			
			auto motion= container_motions.GetValue(arg_key);

			if (motion) {
				return motion->GetAnimation();
			}
			else {
				return nullptr;
			}
		}
	private:

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
}