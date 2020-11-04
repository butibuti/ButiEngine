#pragma once
#include"stdafx.h"
#include"Header/GameParts/ResourceFactory_Dx12.h"
#include"Header/Resources/Resource_Sound_XAudio2.h"
#include "..\..\Header\GameParts\ResourceContainer.h"


ButiEngine::ResourceContainer::ResourceContainer()
{

}

void ButiEngine::ResourceContainer::SetGraphicDevice(std::weak_ptr<GraphicDevice> arg_shp_graphicDevice)
{

	wkp_graphicDevice = arg_shp_graphicDevice;

	unq_resourceFactory = ObjectFactory::Create <ResourceFactory_Dx12>(wkp_graphicDevice);

	container_geometryShaders.AddValue(nullptr, "none");
}

void ButiEngine::ResourceContainer::Initialize()
{

}

void ButiEngine::ResourceContainer::PreInitialize()
{
}




ButiEngine::MaterialTag ButiEngine::ResourceContainer::LoadMaterial(const MaterialVariable& arg_resourceMaterial, const TextureTag& arg_textureTag, const std::string& arg_filePath, const std::string& arg_fileDirectory)
{

	if (container_materials.ContainValue(arg_fileDirectory + arg_filePath)) {
		return container_materials.GetTag(arg_fileDirectory + arg_filePath);
	}

	return container_materials.AddValue(unq_resourceFactory->CreateMaterial(arg_resourceMaterial, arg_textureTag), arg_fileDirectory + arg_filePath);
}

ButiEngine::MaterialTag ButiEngine::ResourceContainer::LoadMaterial(const std::string& arg_filePath, const std::string& arg_fileDirectory)
{
	if (container_materials.ContainValue(arg_fileDirectory + arg_filePath)) {
		return container_materials.GetTag(arg_fileDirectory + arg_filePath);
	}
	vec_filePathAndDirectory_mat.push_back(arg_fileDirectory + arg_filePath);
	return container_materials.AddValue(unq_resourceFactory->CreateMaterial(arg_filePath, arg_fileDirectory), arg_fileDirectory + arg_filePath);
}

std::vector < ButiEngine::MaterialTag> ButiEngine::ResourceContainer::LoadMaterial(const std::vector<std::pair<std::string, std::string>>& arg_vec_filePathAndDirectory)
{
	std::vector<MaterialTag> out;
	out.reserve(arg_vec_filePathAndDirectory.size());

	for (auto itr = arg_vec_filePathAndDirectory.begin(); itr != arg_vec_filePathAndDirectory.end(); itr++) {
		out.push_back(LoadMaterial(itr->first, itr->second));
	}

	return out;
}

ButiEngine::MaterialTag ButiEngine::ResourceContainer::LoadMaterial(const std::wstring& arg_filePath, const std::string& arg_fileDirectory)
{
	std::string filePath;
	filePath = Util::WStringToString(arg_filePath);
	return container_materials.AddValue(unq_resourceFactory->CreateMaterial(filePath, arg_fileDirectory), arg_fileDirectory + filePath);
}

ButiEngine::TextureTag ButiEngine::ResourceContainer::LoadTexture(const std::string& arg_filePath, const std::string& arg_fileDirectory)
{
	if (container_textures.ContainValue(arg_fileDirectory + arg_filePath)) {
		return container_textures.GetTag(arg_fileDirectory + arg_filePath);
	}

	vec_filePathAndDirectory_tex.push_back(arg_fileDirectory + arg_filePath);
	auto tex= unq_resourceFactory->CreateTextureFromFile(GlobalSettings::GetResourceDirectory() + arg_fileDirectory + arg_filePath);
	tex->SetFilePath(arg_fileDirectory + arg_filePath);
	return container_textures.AddValue(tex, arg_fileDirectory + arg_filePath);
}

std::vector < ButiEngine::TextureTag>  ButiEngine::ResourceContainer::LoadTexture(const std::vector<std::pair<std::string, std::string>>& arg_vec_filePathAndDirectory)
{
	std::vector<TextureTag> out;
	out.reserve(arg_vec_filePathAndDirectory.size());

	for (auto itr = arg_vec_filePathAndDirectory.begin(); itr != arg_vec_filePathAndDirectory.end(); itr++) {
		out.push_back(LoadTexture(itr->first, itr->second));
	}

	return out;
}

ButiEngine::PixelShaderTag ButiEngine::ResourceContainer::LoadPixelShader(const std::string& arg_filePath, const std::string& arg_fileDirectory)
{
	if (container_pixelShaders.ContainValue(arg_fileDirectory + arg_filePath)) {
		return container_pixelShaders.GetTag(arg_fileDirectory + arg_filePath);
	}

	vec_filePathAndDirectory_ps.push_back(arg_fileDirectory + arg_filePath);
	return container_pixelShaders.AddValue(unq_resourceFactory->CreatePixelShaderFromFile(GlobalSettings::GetResourceDirectory() + arg_fileDirectory + arg_filePath),arg_fileDirectory+arg_filePath);
}

std::vector<ButiEngine::PixelShaderTag> ButiEngine::ResourceContainer::LoadPixelShader(const std::vector<std::pair<std::string, std::string>>& arg_vec_filePathAndDirectory)
{
	std::vector<PixelShaderTag> out;
	out.reserve(arg_vec_filePathAndDirectory.size());

	for (auto itr = arg_vec_filePathAndDirectory.begin(); itr != arg_vec_filePathAndDirectory.end(); itr++) {
		out.push_back(LoadPixelShader(itr->first, itr->second));
	}

	return out;
}

ButiEngine::VertexShaderTag ButiEngine::ResourceContainer::LoadVertexShader(const std::string& arg_filePath, const std::string& arg_fileDirectory)
{
	if (container_vertexShaders.ContainValue(arg_fileDirectory + arg_filePath)) {
		return container_vertexShaders.GetTag(arg_fileDirectory + arg_filePath);
	}

	vec_filePathAndDirectory_vs.push_back(arg_fileDirectory + arg_filePath);
	return container_vertexShaders.AddValue(unq_resourceFactory->CreateVertexShaderFromFile(GlobalSettings::GetResourceDirectory() + arg_fileDirectory + arg_filePath), arg_fileDirectory + arg_filePath);
}

std::vector<ButiEngine::VertexShaderTag> ButiEngine::ResourceContainer::LoadVertexShader(const std::vector<std::pair<std::string, std::string>>& arg_vec_filePathAndDirectory)
{
	std::vector<VertexShaderTag> out;
	out.reserve(arg_vec_filePathAndDirectory.size());

	for (auto itr = arg_vec_filePathAndDirectory.begin(); itr != arg_vec_filePathAndDirectory.end(); itr++) {
		out.push_back(LoadVertexShader(itr->first, itr->second));
	}

	return out;
}

ButiEngine::GeometryShaderTag ButiEngine::ResourceContainer::LoadGeometryShader(const std::string& arg_filePath, const std::string& arg_fileDirectory)
{
	if (container_geometryShaders.ContainValue(arg_fileDirectory + arg_filePath)) {
		return container_geometryShaders.GetTag(arg_fileDirectory + arg_filePath);
	}

	vec_filePathAndDirectory_gs.push_back(arg_fileDirectory + arg_filePath);
	return container_geometryShaders.AddValue(unq_resourceFactory->CreateGeometryShaderFromFile(GlobalSettings::GetResourceDirectory() + arg_fileDirectory + arg_filePath), arg_fileDirectory + arg_filePath);;
}

std::vector< ButiEngine::GeometryShaderTag> ButiEngine::ResourceContainer::LoadGeometryShader(const std::vector<std::pair<std::string, std::string>>& arg_vec_filePathAndDirectory)
{
	std::vector<GeometryShaderTag> out;
	out.reserve(arg_vec_filePathAndDirectory.size());

	for (auto itr = arg_vec_filePathAndDirectory.begin(); itr != arg_vec_filePathAndDirectory.end(); itr++) {
		out.push_back(LoadGeometryShader(itr->first, itr->second));
	}

	return out;
}

ButiEngine::ShaderTag ButiEngine::ResourceContainer::LoadShader(const  ShaderName& arg_shaderNames)
{

	vec_shaderNames.push_back(arg_shaderNames);
	return container_shaders.AddValue(ObjectFactory::Create<Resource_Shader>(container_vertexShaders.GetValue(arg_shaderNames.vertexShaderDirectory+arg_shaderNames.vertexShaderName), container_pixelShaders.GetValue(arg_shaderNames.pixelShaderDirectory + arg_shaderNames.pixelShaderName), container_geometryShaders.GetValue(arg_shaderNames.geometryShaderDirectory + arg_shaderNames.geometryShaderName)
		, arg_shaderNames.vertexShaderDirectory + arg_shaderNames.vertexShaderName + arg_shaderNames.geometryShaderDirectory + arg_shaderNames.pixelShaderName + arg_shaderNames.geometryShaderDirectory + arg_shaderNames.geometryShaderName
		), arg_shaderNames.shaderName);
}


std::vector < ButiEngine::ShaderTag> ButiEngine::ResourceContainer::LoadShader(const std::vector<ShaderName>& arg_vec_shaderNames)
{
	std::vector<ShaderTag> out;
	out.reserve(arg_vec_shaderNames.size());

	for (auto itr = arg_vec_shaderNames.begin(); itr != arg_vec_shaderNames.end(); itr++) {
		out.push_back(LoadShader(*itr));
	}

	return out;
}

ButiEngine::SoundTag ButiEngine::ResourceContainer::LoadSound(const std::string& arg_filePath, const std::string& arg_fileDirectory)
{
	if (container_sounds.ContainValue(arg_fileDirectory + arg_filePath)) {
		return container_sounds.GetTag(arg_fileDirectory + arg_filePath);
	}

	vec_filePathAndDirectory_sound.push_back(arg_fileDirectory + arg_filePath);
	return container_sounds.AddValue(ObjectFactory::Create<Resource_Sound_XAudio2>(GlobalSettings::GetResourceDirectory() + arg_fileDirectory + arg_filePath),arg_fileDirectory+arg_filePath);
}

std::vector<ButiEngine::SoundTag> ButiEngine::ResourceContainer::LoadSound(const std::vector<std::pair<std::string, std::string>>& arg_vec_filePathAndDirectory)
{
	std::vector<SoundTag> out;
	out.reserve(arg_vec_filePathAndDirectory.size());

	for (auto itr = arg_vec_filePathAndDirectory.begin(); itr != arg_vec_filePathAndDirectory.end(); itr++) {
		out.push_back(LoadSound(itr->first, itr->second));
	}

	return out;
}

ButiEngine::ModelTag ButiEngine::ResourceContainer::LoadModel(const std::string& arg_filePath, const std::string& arg_fileDirectory)
{
	if (container_models.ContainValue(arg_fileDirectory + arg_filePath)) {
		return container_models.GetTag(arg_fileDirectory + arg_filePath);
	}

	vec_filePathAndDirectory_model.push_back(arg_fileDirectory + arg_filePath);
	return container_models.AddValue(unq_resourceFactory->CreateModel(arg_filePath, arg_fileDirectory), arg_fileDirectory + arg_filePath);
}

std::vector < ButiEngine::ModelTag>  ButiEngine::ResourceContainer::LoadModel(const std::vector<std::pair<std::string, std::string>>& arg_vec_filePathAndDirectory)
{
	std::vector<ModelTag> out;
	out.reserve(arg_vec_filePathAndDirectory.size());

	for (auto itr = arg_vec_filePathAndDirectory.begin(); itr != arg_vec_filePathAndDirectory.end(); itr++) {
		out.push_back(LoadModel(itr->first, itr->second));
	}

	return out;
}

ButiEngine::ModelTag ButiEngine::ResourceContainer::LoadModel(std::shared_ptr<Resource_Model> arg_model, const std::string& arg_filePath, const std::string& arg_fileDirectory)
{
	if (container_models.ContainValue(arg_fileDirectory + arg_filePath)) {
		return container_models.GetTag(arg_fileDirectory + arg_filePath);
	}
	return container_models.AddValue(arg_model, arg_fileDirectory + arg_filePath);
}

ButiEngine::MotionTag ButiEngine::ResourceContainer::LoadMotion(const std::string& arg_filePath, const std::string& arg_fileDirectory)
{
	if (container_motions.ContainValue(arg_fileDirectory + arg_filePath)) {
		return container_motions.GetTag(arg_fileDirectory + arg_filePath);
	}
	vec_filePathAndDirectory_motion.push_back(arg_fileDirectory + arg_filePath);
	return container_motions.AddValue(unq_resourceFactory->CreateMotion(arg_filePath, arg_fileDirectory), arg_fileDirectory + arg_filePath);
}

std::vector<ButiEngine::MotionTag> ButiEngine::ResourceContainer::LoadMotion(const std::vector<std::pair<std::string, std::string>>& arg_vec_filePathAndDirectory)
{
	std::vector<MotionTag> out;
	out.reserve(arg_vec_filePathAndDirectory.size());

	for (auto itr = arg_vec_filePathAndDirectory.begin(); itr != arg_vec_filePathAndDirectory.end(); itr++) {
		out.push_back(LoadMotion(itr->first, itr->second));
	}

	return out;
}

ButiEngine::MotionTag ButiEngine::ResourceContainer::LoadMotion(std::shared_ptr<Resource_Motion> arg_motion, const std::string& arg_filePath, const std::string& arg_fileDirectory)
{
	
	return container_motions.AddValue(arg_motion, arg_fileDirectory + arg_filePath);
}

void ButiEngine::ResourceContainer::UnLoadMesh(MeshTag arg_meshTag)
{
	container_meshes.Remove(arg_meshTag);
}

void ButiEngine::ResourceContainer::UnLoadTexture(TextureTag arg_textureTag)
{
	container_textures.Remove(arg_textureTag);
}

void ButiEngine::ResourceContainer::UnLoadShader(ShaderTag arg_shaderTag)
{
	container_shaders.Remove(arg_shaderTag);
}

void ButiEngine::ResourceContainer::UnLoadSound(SoundTag arg_soundTag)
{
	container_sounds.Remove(arg_soundTag);
}

void ButiEngine::ResourceContainer::Release()
{
	container_geometryShaders.Clear();
	container_shaders.Clear();
	container_vertexShaders.Clear();
	container_pixelShaders.Clear();
	container_meshes.Clear();
	container_models.Clear();
	container_sounds.Clear();
	container_textures.Clear();
	container_materials.Clear();
	container_motions.Clear();
}

ButiEngine::MeshTag ButiEngine::ResourceContainer::GetMeshTag(const std::string& arg_key, const std::string& arg_fileDirectory)
{
	return container_meshes.GetTag(arg_fileDirectory + arg_key);
}

ButiEngine::TextureTag ButiEngine::ResourceContainer::GetTextureTag(const std::string& arg_key, const std::string& arg_fileDirectory)
{
	return container_textures.GetTag(arg_fileDirectory + arg_key);
}

ButiEngine::ShaderTag ButiEngine::ResourceContainer::GetShaderTag(const std::string& arg_key)
{
	return container_shaders.GetTag( arg_key);
}

ButiEngine::SoundTag ButiEngine::ResourceContainer::GetSoundTag(const std::string& arg_key, const std::string& arg_fileDirectory)
{
	return container_sounds.GetTag(arg_fileDirectory + arg_key);
}

ButiEngine::MaterialTag ButiEngine::ResourceContainer::GetMaterialTag(const std::string& arg_key, const std::string& arg_fileDirectory)
{
	return container_materials.GetTag(arg_fileDirectory + arg_key);
}

ButiEngine::ModelTag ButiEngine::ResourceContainer::GetModelTag(const std::string& arg_key, const std::string& arg_fileDirectory)
{
	return container_models.GetTag(arg_fileDirectory + arg_key);
}

ButiEngine::MotionTag ButiEngine::ResourceContainer::GetMotionTag(const std::string& arg_key, const std::string& arg_fileDirectory)
{
	return container_motions.GetTag(arg_fileDirectory + arg_key);
}

void ButiEngine::ResourceContainer::Reload()
{

	for (auto itr = vec_filePathAndDirectory_mat.begin(); itr != vec_filePathAndDirectory_mat.end(); itr++) {
		(LoadMaterial(*itr));
	}

	for (auto itr = vec_filePathAndDirectory_tex.begin(); itr != vec_filePathAndDirectory_tex.end(); itr++) {
		(LoadTexture(*itr));
	}

	for (auto itr = vec_filePathAndDirectory_ps.begin(); itr != vec_filePathAndDirectory_ps.end(); itr++) {
		(LoadPixelShader(*itr));
	}

	for (auto itr = vec_filePathAndDirectory_vs.begin(); itr != vec_filePathAndDirectory_vs.end(); itr++) {
		(LoadVertexShader(*itr));
	}

	for (auto itr = vec_filePathAndDirectory_gs.begin(); itr != vec_filePathAndDirectory_gs.end(); itr++) {
		(LoadGeometryShader(*itr));
	}

	for (auto itr = vec_shaderNames.begin(); itr != vec_shaderNames.end(); itr++) {
		(LoadShader(*itr));
	}

	for (auto itr = vec_filePathAndDirectory_sound.begin(); itr != vec_filePathAndDirectory_sound.end(); itr++) {
		(LoadSound(*itr));
	}


	for (auto itr = vec_filePathAndDirectory_model.begin(); itr != vec_filePathAndDirectory_model.end(); itr++) {
		(LoadModel(*itr));
	}

	for (auto itr = vec_filePathAndDirectory_motion.begin(); itr != vec_filePathAndDirectory_motion.end(); itr++) {
		(LoadMotion(*itr));
	}


}



void ButiEngine::OutputCereal(const std::shared_ptr<ResourceContainer>& v)
{
	std::stringstream stream;


	cereal::BinaryOutputArchive binOutArchive(stream);
	binOutArchive(v);

	std::ofstream outputFile(GlobalSettings::GetResourceDirectory() + "Application/resourceLoadData.resource", std::ios::out);

	outputFile << stream.str();

	outputFile.close();
	stream.clear();
}

void ButiEngine::InputCereal(std::shared_ptr<ResourceContainer>& v, const std::string& path, std::weak_ptr<GraphicDevice> arg_shp_graphicDevice)
{
	std::stringstream stream;

	std::ifstream inputFile(path, std::ios::in);

	stream << inputFile.rdbuf();

	cereal::BinaryInputArchive binInputArchive(stream);


	binInputArchive(v);

	v->SetGraphicDevice(arg_shp_graphicDevice);

	v->Reload();
}
