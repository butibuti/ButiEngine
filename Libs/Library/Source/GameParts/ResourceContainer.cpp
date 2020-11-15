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

void ButiEngine::ResourceContainer::ShowGUI()
{
	static bool isShowShader = false;

	ImGui::Begin("ResourceContainer");

	auto app = wkp_graphicDevice.lock()->GetApplication().lock();
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_TabListPopupButton;

	if (ImGui::BeginTabBar("ResourceContainerTabBar", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("MeshTags", nullptr, ImGuiTabItemFlags_None)) {
			ImGui::BeginChild("##MeshTag", ImVec2(0, 0), true);
			{
				app->GetGUIController()->SetResourceTag(
					container_meshes.ShowGUI(app->GetGUIController()->GetGUIIO())
				);

			}
			ImGui::EndChild();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("ModelTags", nullptr, ImGuiTabItemFlags_None)) {

			if (ImGui::Button("Add")) {

			}



			ImGui::BeginChild("##ModelTag", ImVec2(0, 0), true);
			{
				app->GetGUIController()->SetResourceTag(
					container_models.ShowGUI(app->GetGUIController()->GetGUIIO())
				);

			}

			ImGui::EndChild();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("MaterialTags", nullptr, ImGuiTabItemFlags_None)) {

			if (ImGui::Button("Add")) {

			}

			ImGui::SameLine();
			ImGui::BeginChild("MaterialTagRemove", ImVec2(6 * ImGui::GetFontSize(), ImGui::GetFontSize() * 2), true);
			ImGui::Text("Remove");

			if (ImGui::IsWindowHovered()) {
				auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetMaterialTag();
				if (!tag.IsEmpty()) {
					UnLoadMaterial(tag);
				}
			}


			ImGui::EndChild();

			ImGui::BeginChild("##MaterialTag", ImVec2(0, 0), true);
			{
				app->GetGUIController()->SetResourceTag(
					container_materials.ShowGUI(app->GetGUIController()->GetGUIIO())
				);

			}
			ImGui::EndChild();

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("ShaderTags", nullptr, ImGuiTabItemFlags_None)) {
			if (ImGui::Button("Add Shader")) {
				isShowShader = !isShowShader;
			}

			ImGui::SameLine();

			ImGui::BeginChild("ShaderTagRemove", ImVec2(6 * ImGui::GetFontSize(), ImGui::GetFontSize() * 2), true);
			ImGui::Text("Remove");

			if (ImGui::IsWindowHovered()) {
				auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetShaderTag();
				if (!tag.IsEmpty()) {
					UnLoadShader(tag);
				}
			}


			ImGui::EndChild();


			ImGui::BeginChild("##ShaderTag", ImVec2(0, 0), true);
			{
				app->GetGUIController()->SetResourceTag(
					container_shaders.ShowGUI(app->GetGUIController()->GetGUIIO())
				);

			}
			ImGui::EndChild();

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("VertexShaderTags", nullptr, ImGuiTabItemFlags_None)) {
			ImGui::Button("Add VertexShader");
			auto flags = ImGuiPopupFlags_MouseButtonLeft;
			if (ImGui::BeginPopupContextItem("Add VertexShader", flags))
			{
				ImGui::Text("File name:");
				ImGui::InputText("##edit", CallBacks::objectName, IM_ARRAYSIZE(CallBacks::objectName));
				if (ImGui::Button("OK!!")) {
					LoadVertexShader(CallBacks::objectName, "Shader/Compiled/");
					CallBacks::ObjectNameReset();
					ImGui::CloseCurrentPopup();
				}ImGui::SameLine();
				if (ImGui::Button("Cancel")) {
					CallBacks::ObjectNameReset();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::SameLine();


			ImGui::BeginChild("VertexTagRemove", ImVec2(6 * ImGui::GetFontSize(), ImGui::GetFontSize() * 2), true);
			ImGui::Text("Remove");

			if (ImGui::IsWindowHovered()) {
				auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetVertexShaderTag();
				if (!tag.IsEmpty()) {
					UnLoadVertexShader(tag);
				}
			}


			ImGui::EndChild();


			ImGui::BeginChild("##VertexShaderTag", ImVec2(0, 0), true);
			{
				app->GetGUIController()->SetResourceTag(
					container_vertexShaders.ShowGUI(app->GetGUIController()->GetGUIIO())
				);

			}
			ImGui::EndChild();

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("PixelShaderTags", nullptr, ImGuiTabItemFlags_None)) {

			ImGui::Button("Add PixelShader");
			auto flags = ImGuiPopupFlags_MouseButtonLeft;
			if (ImGui::BeginPopupContextItem("Add PixelShader", flags))
			{
				ImGui::Text("File name:");
				ImGui::InputText("##edit", CallBacks::objectName, IM_ARRAYSIZE(CallBacks::objectName));
				if (ImGui::Button("OK!!")) {
					LoadPixelShader(CallBacks::objectName, "Shader/Compiled/");
					CallBacks::ObjectNameReset();
					ImGui::CloseCurrentPopup();
				}ImGui::SameLine();
				if (ImGui::Button("Cancel")) {
					CallBacks::ObjectNameReset();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			ImGui::SameLine();

			ImGui::BeginChild("PixelTagRemove", ImVec2(6 * ImGui::GetFontSize(), ImGui::GetFontSize() * 2), true);
			ImGui::Text("Remove");

			if (ImGui::IsWindowHovered()) {
				auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetPixelShaderTag();
				if (!tag.IsEmpty()) {
					UnLoadPixelShader(tag);
				}
			}


			ImGui::EndChild();


			ImGui::BeginChild("##PixelShaderTag", ImVec2(0, 0), true);
			{
				app->GetGUIController()->SetResourceTag(
					container_pixelShaders.ShowGUI(app->GetGUIController()->GetGUIIO())
				);

			}
			ImGui::EndChild();

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("GeometryShaderTags", nullptr, ImGuiTabItemFlags_None)) {

			ImGui::Button("Add GeometryShader");
			auto flags = ImGuiPopupFlags_MouseButtonLeft;
			if (ImGui::BeginPopupContextItem("Add GeometryShader", flags))
			{
				ImGui::Text("File name:");
				ImGui::InputText("##edit", CallBacks::objectName, IM_ARRAYSIZE(CallBacks::objectName));
				if (ImGui::Button("OK!!")) {
					LoadGeometryShader(CallBacks::objectName, "Shader/Compiled/");
					CallBacks::ObjectNameReset();
					ImGui::CloseCurrentPopup();
				}ImGui::SameLine();
				if (ImGui::Button("Cancel")) {
					CallBacks::ObjectNameReset();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::SameLine();


			ImGui::BeginChild("GeometryTagRemove", ImVec2(6 * ImGui::GetFontSize(), ImGui::GetFontSize() * 2), true);
			ImGui::Text("Remove");

			if (ImGui::IsWindowHovered()) {
				auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetGeometryShaderTag();
				if (!tag.IsEmpty()) {
					UnLoadGeometryShader(tag);
				}
			}


			ImGui::EndChild();


			ImGui::BeginChild("##GeometryShaderTag", ImVec2(0, 0), true);
			{
				app->GetGUIController()->SetResourceTag(
					container_geometryShaders.ShowGUI(app->GetGUIController()->GetGUIIO())
				);

			}
			ImGui::EndChild();

			ImGui::EndTabItem();
		}


		ImGui::EndTabBar();
	}




	ImGui::End();

	if (isShowShader) {

		ImGui::Begin("AddShader");
		{
			static VertexShaderTag vstag;
			static PixelShaderTag pstag;
			static GeometryShaderTag gstag;

			{

				ImGui::BulletText("VertexShaderTag");
				auto tagName = GetTagNameVertexShader(vstag);
				(ImGui::BeginChild("VSTagWin", ImVec2(ImGui::GetFontSize()* (tagName.size() + 2), ImGui::GetFontSize() * 2), true));
				ImGui::Text(Util::ToUTF8(tagName).c_str());

				if (ImGui::IsWindowHovered()) {
					auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetVertexShaderTag();
					if (!tag.IsEmpty()) {
						vstag = tag;
					}
				}


				ImGui::EndChild();

			}
			{

				ImGui::BulletText("PixelShaderTag");
				auto tagName = GetTagNamePixelShader(pstag);
				(ImGui::BeginChild("PSTagWin", ImVec2(ImGui::GetFontSize() * (tagName.size() + 2), ImGui::GetFontSize() * 2), true));
				ImGui::Text(Util::ToUTF8(tagName).c_str());

				if (ImGui::IsWindowHovered()) {
					auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetPixelShaderTag();
					if (!tag.IsEmpty()) {
						pstag = tag;
					}
				}


				ImGui::EndChild();

			}
			{

				ImGui::BulletText("GeometryShaderTag");
				auto tagName = GetTagNameGeometryShader(gstag);
				(ImGui::BeginChild("GSTagWin", ImVec2(ImGui::GetFontSize() * (tagName.size() + 2), ImGui::GetFontSize() * 2), true));
				ImGui::Text(Util::ToUTF8(tagName).c_str());

				if (ImGui::IsWindowHovered()) {
					auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetGeometryShaderTag();
					if (!tag.IsEmpty()) {
						gstag = tag;
					}
				}


				ImGui::EndChild();

			}

			ImGui::Text("ShaderName");
			ImGui::InputText("##edit", CallBacks::objectName, IM_ARRAYSIZE(CallBacks::objectName));
			if (ImGui::Button("OK!!")) {

				ShaderName sn;

				sn.shaderName = CallBacks::objectName;
				sn.geometryShaderName = GetTagNameGeometryShader(gstag);
				sn.vertexShaderName = GetTagNameVertexShader(vstag);
				sn.pixelShaderName= GetTagNamePixelShader(pstag);

				LoadShader(sn);

				CallBacks::ObjectNameReset();
				ImGui::SameLine();
			}
			ImGui::End();
		}

	}
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
	if (!Util::CheckFileExistence(GlobalSettings::GetResourceDirectory() + arg_fileDirectory + arg_filePath)) {
		return TextureTag();
	}
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
	if (!Util::CheckFileExistence(GlobalSettings::GetResourceDirectory() + arg_fileDirectory + arg_filePath+".dx12cps")) {
		return PixelShaderTag();
	}
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
	if (!Util::CheckFileExistence(GlobalSettings::GetResourceDirectory() + arg_fileDirectory + arg_filePath + ".dx12cps")) {
		return VertexShaderTag();
	}
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
	if (arg_textureTag.IsEmpty()) {
		return;
	}
	auto tagName = container_textures.GetIDName(arg_textureTag);

	for (auto itr = vec_filePathAndDirectory_tex.begin(); itr != vec_filePathAndDirectory_tex.end(); itr++) {
		if ((*itr)== tagName) {
			vec_filePathAndDirectory_tex.erase(itr);
			break;
		}
	}
	container_textures.Remove(arg_textureTag);
}

void ButiEngine::ResourceContainer::UnLoadShader(ShaderTag arg_shaderTag)
{
	if (arg_shaderTag.IsEmpty()) {
		return;
	}
	auto shaderName = container_shaders.GetIDName(arg_shaderTag);

	for (auto itr = vec_shaderNames.begin(); itr != vec_shaderNames.end(); itr++) {
		if ((*itr).shaderName == shaderName) {
			vec_shaderNames.erase(itr);
			break;
		}
	}

	container_shaders.Remove(arg_shaderTag);
}

void ButiEngine::ResourceContainer::UnLoadPixelShader(PixelShaderTag arg_shaderTag)
{
	if (arg_shaderTag.IsEmpty()) {
		return;
	}
	auto tagName = container_pixelShaders.GetIDName(arg_shaderTag);

	for (auto itr = vec_filePathAndDirectory_ps.begin(); itr != vec_filePathAndDirectory_ps.end(); itr++) {
		if ((*itr) == tagName) {
			vec_filePathAndDirectory_ps.erase(itr);
			break;
		}
	}
	container_pixelShaders.Remove(arg_shaderTag);
}

void ButiEngine::ResourceContainer::UnLoadVertexShader(VertexShaderTag arg_shaderTag)
{
	if (arg_shaderTag.IsEmpty()) {
		return;
	}
	auto tagName = container_vertexShaders.GetIDName(arg_shaderTag);

	for (auto itr = vec_filePathAndDirectory_vs.begin(); itr != vec_filePathAndDirectory_vs.end(); itr++) {
		if ((*itr) == tagName) {
			vec_filePathAndDirectory_vs.erase(itr);
			break;
		}
	}
	container_vertexShaders.Remove(arg_shaderTag);
}

void ButiEngine::ResourceContainer::UnLoadGeometryShader(GeometryShaderTag arg_shaderTag)
{

	if (arg_shaderTag.IsEmpty()) {
		return;
	}
	auto tagName = container_geometryShaders.GetIDName(arg_shaderTag);

	for (auto itr = vec_filePathAndDirectory_gs.begin(); itr != vec_filePathAndDirectory_gs.end(); itr++) {
		if ((*itr) == tagName) {
			vec_filePathAndDirectory_gs.erase(itr);
			break;
		}
	}
	container_geometryShaders.Remove(arg_shaderTag);
}

void ButiEngine::ResourceContainer::UnLoadSound(SoundTag arg_soundTag)
{
	if (arg_soundTag.IsEmpty()) {
		return;
	}
	auto tagName = container_sounds.GetIDName(arg_soundTag);

	for (auto itr = vec_filePathAndDirectory_sound.begin(); itr != vec_filePathAndDirectory_sound.end(); itr++) {
		if ((*itr)== tagName) {
			vec_filePathAndDirectory_sound.erase(itr);
			break;
		}
	}
	container_sounds.Remove(arg_soundTag);
}

void ButiEngine::ResourceContainer::UnLoadModel(ModelTag arg_modelTag)
{
	if (arg_modelTag.IsEmpty()) {
		return;
	}
	auto tagName = container_models.GetIDName(arg_modelTag);

	for (auto itr = vec_filePathAndDirectory_model.begin(); itr != vec_filePathAndDirectory_model.end(); itr++) {
		if ((*itr) == tagName) {
			vec_filePathAndDirectory_model.erase(itr);
			break;
		}
	}
	container_models.Remove(arg_modelTag);
}

void ButiEngine::ResourceContainer::UnLoadMaterial(MaterialTag arg_materialTag)
{
	if (arg_materialTag.IsEmpty()) {
		return;
	}
	auto tagName = container_materials.GetIDName(arg_materialTag);

	for (auto itr = vec_filePathAndDirectory_model.begin(); itr != vec_filePathAndDirectory_model.end(); itr++) {
		if ((*itr) == tagName) {
			vec_filePathAndDirectory_model.erase(itr);
			break;
		}
	}
	container_materials.Remove(arg_materialTag);
}

std::string ButiEngine::ResourceContainer::GetTagNameMesh(MeshTag arg_meshTag)
{
	return container_meshes.GetIDName(arg_meshTag);
}

std::string ButiEngine::ResourceContainer::GetTagNameTexture(TextureTag arg_textureTag)
{
	return container_textures.GetIDName(arg_textureTag);
}

std::string ButiEngine::ResourceContainer::GetTagNameShader(ShaderTag arg_shaderTag)
{
	return container_shaders.GetIDName(arg_shaderTag);
}

std::string ButiEngine::ResourceContainer::GetTagNamePixelShader(PixelShaderTag arg_shaderTag)
{
	return container_pixelShaders.GetIDName(arg_shaderTag);
}

std::string ButiEngine::ResourceContainer::GetTagNameVertexShader(VertexShaderTag arg_shaderTag)
{
	return container_vertexShaders.GetIDName(arg_shaderTag);
}

std::string ButiEngine::ResourceContainer::GetTagNameGeometryShader(GeometryShaderTag arg_shaderTag)
{
	return container_geometryShaders.GetIDName(arg_shaderTag);
}

std::string ButiEngine::ResourceContainer::GetTagNameSound(SoundTag arg_soundTag)
{
	return container_sounds.GetIDName(arg_soundTag);
}

std::string ButiEngine::ResourceContainer::GetTagNameModel(ModelTag arg_modelTag)
{
	return container_models.GetIDName(arg_modelTag);
}
std::string ButiEngine::ResourceContainer::GetTagNameMaterial(MaterialTag arg_materiallTag)
{
	return container_materials.GetIDName(arg_materiallTag);
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

void ButiEngine::ResourceContainer::MaterialUpdate()
{
	auto ref_materials = container_materials.GetResources();
	auto endItr = ref_materials.end();
	for (auto itr = ref_materials.begin(); itr !=endItr; itr++) {
		(*itr)->Update();
	}
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

	auto copy = vec_filePathAndDirectory_ps;
	vec_filePathAndDirectory_ps.clear();

	for (auto itr = copy.begin(); itr !=copy.end(); itr++) {
		(LoadPixelShader(*itr));
	}
	copy = vec_filePathAndDirectory_vs;
	vec_filePathAndDirectory_vs.clear();
	for (auto itr = copy.begin(); itr != copy.end(); itr++) {
		(LoadVertexShader(*itr));
	}
	copy = vec_filePathAndDirectory_gs;
	vec_filePathAndDirectory_gs.clear();
	for (auto itr = copy.begin(); itr != copy.end(); itr++) {
		(LoadGeometryShader(*itr));
	}
	auto  shaderNameCopy = vec_shaderNames;
	vec_shaderNames.clear();
	for (auto itr = shaderNameCopy.begin(); itr != shaderNameCopy.end(); itr++) {
		(LoadShader(*itr));
	}
	copy = vec_filePathAndDirectory_sound;
	vec_filePathAndDirectory_sound.clear();
	for (auto itr = copy.begin(); itr != copy.end(); itr++) {
		(LoadSound(*itr));
	}

	copy = vec_filePathAndDirectory_model;
	vec_filePathAndDirectory_model.clear();
	for (auto itr = copy.begin(); itr != copy.end(); itr++) {
		(LoadModel(*itr));
	}
	copy = vec_filePathAndDirectory_mat;
	vec_filePathAndDirectory_mat.clear();
	for (auto itr = copy.begin(); itr != copy.end(); itr++) {
		auto fullPath = (*itr);
		LoadMaterial(StringHelper::GetFileName(fullPath,true),StringHelper::GetDirectory(fullPath));
	}
	copy = vec_filePathAndDirectory_tex;
	vec_filePathAndDirectory_tex.clear();
	for (auto itr = copy.begin(); itr != copy.end(); itr++) {
		(LoadTexture(*itr));
	}
	copy = vec_filePathAndDirectory_motion;
	vec_filePathAndDirectory_motion.clear();
	for (auto itr = copy.begin(); itr != copy.end(); itr++) {
		(LoadMotion(*itr));
	}


}



void ButiEngine::OutputCereal(const std::shared_ptr<ResourceContainer>& v)
{
	std::stringstream stream;


	cereal::PortableBinaryOutputArchive binOutArchive(stream);
	binOutArchive(v);

	std::ofstream outputFile(GlobalSettings::GetResourceDirectory() + "Application/resourceLoadData.resource", std::ios::binary);

	outputFile << stream.str();

	outputFile.close();
	stream.clear();
}

void ButiEngine::InputCereal(std::shared_ptr<ResourceContainer>& v, std::weak_ptr<GraphicDevice> arg_shp_graphicDevice)
{
	std::stringstream stream;

	std::ifstream inputFile(GlobalSettings::GetResourceDirectory() + "Application/resourceLoadData.resource", std::ios::binary);

	stream << inputFile.rdbuf();

	cereal::PortableBinaryInputArchive binInputArchive(stream);


	binInputArchive(v);

	v->SetGraphicDevice(arg_shp_graphicDevice);

	//
}
