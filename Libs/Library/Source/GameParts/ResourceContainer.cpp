#pragma once
#include"stdafx.h"
#include"Header/GameParts/ResourceFactory_Dx12.h"
#include"Header/Resources/Resource_Sound_XAudio2.h"
#include "..\..\Header\GameParts\ResourceContainer.h"

#include "..\..\Header\Common\CerealUtill.h"

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
	static bool isShowAddMaterial = false;
	static MaterialTag editMaterial = MaterialTag();
	static MaterialLoadInfo* EditMaterialLoadInfo;

	GUI::Begin("ResourceContainer");

	auto app = wkp_graphicDevice.lock()->GetApplication().lock();
	GUI::GuiTabBarFlags tab_bar_flags = GUI::GuiTabBarFlags_TabListPopupButton;

	if (GUI::BeginTabBar("ResourceContainerTabBar", tab_bar_flags))
	{
		if (GUI::BeginTabItem("MeshTags", nullptr, GUI::GuiTabItemFlags_None)) {
			GUI::BeginChild("##MeshTag", Vector2(0, 0), true);
			{
				app->GetGUIController()->SetResourceTag(
					container_meshes.ShowGUI(app->GetGUIController()->GetGUIIO())
				);

			}
			GUI::EndChild();
			GUI::EndTabItem();
		}

		if (GUI::BeginTabItem("ModelTags", nullptr, GUI::GuiTabItemFlags_None)) {

			if (GUI::Button("Add Model")) {

			}

			auto flags = GUI::GuiPopupFlags_MouseButtonLeft;
			if (GUI::BeginPopupContextItem("Add Model", flags))
			{
				GUI::Text("File name:");
				GUI::InputText("##edit", GUI::objectName, 128);
				if (GUI::Button("OK!!")) {
					LoadModel(Util::UTF8ToMultiByte( GUI::objectName), "Model/");
					GUI::ObjectNameReset();
					GUI::CloseCurrentPopup();
				}GUI::SameLine();
				if (GUI::Button("Cancel")) {
					GUI::ObjectNameReset();
					GUI::CloseCurrentPopup();
				}
				GUI::EndPopup();
			}

			GUI::SameLine();
			GUI::BeginChild("ModelTagRemove", Vector2(6 * GUI::GetFontSize(), GUI::GetFontSize() * 2), true);
			GUI::Text("Remove");

			if (GUI::IsWindowHovered()) {
				auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetModelTag();
				if (!tag.IsEmpty()) {
					UnLoadModel(tag);
				}
			}


			GUI::EndChild();



			GUI::BeginChild("##ModelTag", Vector2(0, 0), true);
			{
				app->GetGUIController()->SetResourceTag(
					container_models.ShowGUI(app->GetGUIController()->GetGUIIO())
				);

			}

			GUI::EndChild();
			GUI::EndTabItem();
		}
		if (GUI::BeginTabItem("MaterialTags", nullptr, GUI::GuiTabItemFlags_None)) {

			if (GUI::Button("Add Material")) {
				isShowAddMaterial = !isShowAddMaterial;
			}

			GUI::SameLine();
			GUI::BeginChild("MaterialTagEdit", Vector2(8 * GUI::GetFontSize(), GUI::GetFontSize() * 2), true);
			GUI::Text("Edit Tag");

			if (GUI::IsWindowHovered()) {
				auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetMaterialTag();
				if (!tag.IsEmpty()) {
					editMaterial = tag;

					for (int i = 0; i < vec_materialLoadInfos.size(); i++) {
						if (vec_materialLoadInfos.at(i).fileName == tag.GetID() || vec_materialLoadInfos.at(i).materialName == tag.GetID()) {
							EditMaterialLoadInfo = &vec_materialLoadInfos.at(i);
						}
					}

				}
			}

			GUI::EndChild();
			GUI::SameLine();
			GUI::BeginChild("MaterialTagRemove", Vector2(6 * GUI::GetFontSize(), GUI::GetFontSize() * 2), true);
			GUI::Text("Remove");

			if (GUI::IsWindowHovered()) {
				auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetMaterialTag();
				if (!tag.IsEmpty()) {
					UnLoadMaterial(tag);
				}
			}


			GUI::EndChild();

			GUI::BeginChild("##MaterialTag", Vector2(0, 0), true);
			{
				app->GetGUIController()->SetResourceTag(
					container_materials.ShowGUI(app->GetGUIController()->GetGUIIO())
				);

			}
			GUI::EndChild();

			GUI::EndTabItem();
		}
		if (GUI::BeginTabItem("ShaderTags", nullptr, GUI::GuiTabItemFlags_None)) {
			if (GUI::Button("Add Shader")) {
				isShowShader = !isShowShader;
			}

			GUI::SameLine();

			GUI::BeginChild("ShaderTagRemove", Vector2(6 * GUI::GetFontSize(), GUI::GetFontSize() * 2), true);
			GUI::Text("Remove");

			if (GUI::IsWindowHovered()) {
				auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetShaderTag();
				if (!tag.IsEmpty()) {
					UnLoadShader(tag);
				}
			}


			GUI::EndChild();


			GUI::BeginChild("##ShaderTag", Vector2(0, 0), true);
			{
				app->GetGUIController()->SetResourceTag(
					container_shaders.ShowGUI(app->GetGUIController()->GetGUIIO())
				);

			}
			GUI::EndChild();

			GUI::EndTabItem();
		}
		if (GUI::BeginTabItem("TextureTags", nullptr, GUI::GuiTabItemFlags_None)) {
			GUI::Button("Add Texture fromFile");
			auto flags = GUI::GuiPopupFlags_MouseButtonLeft;
			if (GUI::BeginPopupContextItem("Add Texture", flags))
			{
				GUI::Text("File name:");
				GUI::InputText("##edit", GUI::objectName, 128);
				if (GUI::Button("OK!!")) {
					LoadTexture(GUI::objectName, "Texture/");
					GUI::ObjectNameReset();
					GUI::CloseCurrentPopup();
				}GUI::SameLine();
				if (GUI::Button("Cancel")) {
					GUI::ObjectNameReset();
					GUI::CloseCurrentPopup();
				}
				GUI::EndPopup();
			}
			GUI::SameLine(); GUI::Button("Add RenderTargetTexture");
			if (GUI::BeginPopupContextItem("Add RenderTargetTexture", flags))
			{
				GUI::Text("Texture name:");
				GUI::InputText("##edit", GUI::objectName, 128);

				static int texWidth;
				static int texHeight;
				GUI::DragInt("texWidth", texWidth, 1.0, 0, 1000000);
				GUI::DragInt("texHeight", texHeight, 1.0, 0, 1000000);

				if (GUI::Button("OK!!")) {
					LoadTexture(":/" + std::string(GUI::objectName) + "/" + std::to_string(texWidth) + "/" + std::to_string(texHeight));
					GUI::ObjectNameReset();
					GUI::CloseCurrentPopup();
				}GUI::SameLine();
				if (GUI::Button("Cancel")) {
					GUI::ObjectNameReset();
					GUI::CloseCurrentPopup();
				}
				GUI::EndPopup();
			}
			GUI::SameLine(); GUI::Button("Add DepthStencilTexture");
			if (GUI::BeginPopupContextItem("Add DepthStencilTexture", flags))
			{
				GUI::Text("Texture name:");
				GUI::InputText("##edit", GUI::objectName, 128);

				static int texWidth;
				static int texHeight;
				GUI::DragInt("texWidth##_d", texWidth, 1.0, 0, 1000000);
				GUI::DragInt("texHeight##_d", texHeight, 1.0, 0, 1000000);

				if (GUI::Button("OK!!")) {
					LoadTexture(";/" + std::string(GUI::objectName) + "/" + std::to_string(texWidth) + "/" + std::to_string(texHeight));
					GUI::ObjectNameReset();
					GUI::CloseCurrentPopup();
				}GUI::SameLine();
				if (GUI::Button("Cancel")) {
					GUI::ObjectNameReset();
					GUI::CloseCurrentPopup();
				}
				GUI::EndPopup();
			}
			GUI::SameLine();


			GUI::BeginChild("TextureTagRemove", Vector2(6 * GUI::GetFontSize(), GUI::GetFontSize() * 2), true);
			GUI::Text("Remove");

			if (GUI::IsWindowHovered()) {
				auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetTextureTag();
				if (!tag.IsEmpty()) {
					UnLoadTexture(tag);
				}
			}


			GUI::EndChild();


			GUI::BeginChild("##TextureTag", Vector2(0, 0), true);
			{
				app->GetGUIController()->SetResourceTag(
					container_textures.ShowGUI(app->GetGUIController()->GetGUIIO())
				);

			}
			GUI::EndChild();

			GUI::EndTabItem();
		}
		if (GUI::BeginTabItem("SoundTags", nullptr, GUI::GuiTabItemFlags_None)) {
			GUI::Button("Add Sound fromFile");
			auto flags = GUI::GuiPopupFlags_MouseButtonLeft;
			if (GUI::BeginPopupContextItem("Add Sound", flags))
			{
				GUI::Text("File name:");
				GUI::InputText("##edit", GUI::objectName, 128);
				if (GUI::Button("OK!!")) {
					LoadSound(GUI::objectName, "Sound/");
					GUI::ObjectNameReset();
					GUI::CloseCurrentPopup();
				}GUI::SameLine();
				if (GUI::Button("Cancel")) {
					GUI::ObjectNameReset();
					GUI::CloseCurrentPopup();
				}
				GUI::EndPopup();
			}
			GUI::SameLine(); 


			GUI::BeginChild("SoundTagRemove", Vector2(6 * GUI::GetFontSize(), GUI::GetFontSize() * 2), true);
			GUI::Text("Remove");

			if (GUI::IsWindowHovered()) {
				auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetSoundTag();
				if (!tag.IsEmpty()) {
					UnLoadSound(tag);
				}
			}


			GUI::EndChild();


			GUI::BeginChild("##SoundTag", Vector2(0, 0), true);
			{
				app->GetGUIController()->SetResourceTag(
					container_sounds.ShowGUI(app->GetGUIController()->GetGUIIO())
				);

			}
			GUI::EndChild();

			GUI::EndTabItem();
		}
		if (GUI::BeginTabItem("VertexShaderTags", nullptr, GUI::GuiTabItemFlags_None)) {
			GUI::Button("Add VertexShader");
			auto flags = GUI::GuiPopupFlags_MouseButtonLeft;
			if (GUI::BeginPopupContextItem("Add VertexShader", flags))
			{
				GUI::Text("File name:");
				GUI::InputText("##edit", GUI::objectName, 128);
				if (GUI::Button("OK!!")) {
					LoadVertexShader(GUI::objectName, "Shader/Compiled/");
					GUI::ObjectNameReset();
					GUI::CloseCurrentPopup();
				}GUI::SameLine();
				if (GUI::Button("Cancel")) {
					GUI::ObjectNameReset();
					GUI::CloseCurrentPopup();
				}
				GUI::EndPopup();
			}
			GUI::SameLine();


			GUI::BeginChild("VertexTagRemove", Vector2(6 * GUI::GetFontSize(), GUI::GetFontSize() * 2), true);
			GUI::Text("Remove");

			if (GUI::IsWindowHovered()) {
				auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetVertexShaderTag();
				if (!tag.IsEmpty()) {
					UnLoadVertexShader(tag);
				}
			}


			GUI::EndChild();


			GUI::BeginChild("##VertexShaderTag", Vector2(0, 0), true);
			{
				app->GetGUIController()->SetResourceTag(
					container_vertexShaders.ShowGUI(app->GetGUIController()->GetGUIIO())
				);

			}
			GUI::EndChild();

			GUI::EndTabItem();
		}
		if (GUI::BeginTabItem("PixelShaderTags", nullptr, GUI::GuiTabItemFlags_None)) {

			GUI::Button("Add PixelShader");
			auto flags = GUI::GuiPopupFlags_MouseButtonLeft;
			if (GUI::BeginPopupContextItem("Add PixelShader", flags))
			{
				GUI::Text("File name:");
				GUI::InputText("##edit", GUI::objectName, 128);
				if (GUI::Button("OK!!")) {
					LoadPixelShader(GUI::objectName, "Shader/Compiled/");
					GUI::ObjectNameReset();
					GUI::CloseCurrentPopup();
				}GUI::SameLine();
				if (GUI::Button("Cancel")) {
					GUI::ObjectNameReset();
					GUI::CloseCurrentPopup();
				}
				GUI::EndPopup();
			}

			GUI::SameLine();

			GUI::BeginChild("PixelTagRemove", Vector2(6 * GUI::GetFontSize(), GUI::GetFontSize() * 2), true);
			GUI::Text("Remove");

			if (GUI::IsWindowHovered()) {
				auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetPixelShaderTag();
				if (!tag.IsEmpty()) {
					UnLoadPixelShader(tag);
				}
			}


			GUI::EndChild();


			GUI::BeginChild("##PixelShaderTag", Vector2(0, 0), true);
			{
				app->GetGUIController()->SetResourceTag(
					container_pixelShaders.ShowGUI(app->GetGUIController()->GetGUIIO())
				);

			}
			GUI::EndChild();

			GUI::EndTabItem();
		}
		if (GUI::BeginTabItem("GeometryShaderTags", nullptr, GUI::GuiTabItemFlags_None)) {

			GUI::Button("Add GeometryShader");
			auto flags = GUI::GuiPopupFlags_MouseButtonLeft;
			if (GUI::BeginPopupContextItem("Add GeometryShader", flags))
			{
				GUI::Text("File name:");
				GUI::InputText("##edit", GUI::objectName, 128);
				if (GUI::Button("OK!!")) {
					LoadGeometryShader(GUI::objectName, "Shader/Compiled/");
					GUI::ObjectNameReset();
					GUI::CloseCurrentPopup();
				}GUI::SameLine();
				if (GUI::Button("Cancel")) {
					GUI::ObjectNameReset();
					GUI::CloseCurrentPopup();
				}
				GUI::EndPopup();
			}
			GUI::SameLine();


			GUI::BeginChild("GeometryTagRemove", Vector2(6 * GUI::GetFontSize(), GUI::GetFontSize() * 2), true);
			GUI::Text("Remove");

			if (GUI::IsWindowHovered()) {
				auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetGeometryShaderTag();
				if (!tag.IsEmpty()) {
					UnLoadGeometryShader(tag);
				}
			}


			GUI::EndChild();


			GUI::BeginChild("##GeometryShaderTag", Vector2(0, 0), true);
			{
				app->GetGUIController()->SetResourceTag(
					container_geometryShaders.ShowGUI(app->GetGUIController()->GetGUIIO())
				);

			}
			GUI::EndChild();

			GUI::EndTabItem();
		}


		GUI::EndTabBar();
	}




	GUI::End();

	if (!editMaterial.IsEmpty()) {

		GUI::Begin("MaterialEditor");

		if (GUI::Button("Exit")) {
			editMaterial = MaterialTag();
		}
		auto material = GetMaterial(editMaterial).lock();
		bool isMaterialEdit=material->OnShowUI();

		GUI::End();
		if (isMaterialEdit) {
			EditMaterialLoadInfo->var = material->GetMaterialVariable();
			EditMaterialLoadInfo->vec_texture = material->GetTextureTags();
		}
	}


	if (isShowShader) {

		GUI::Begin("AddShader");
		{
			static VertexShaderTag vstag;
			static PixelShaderTag pstag;
			static GeometryShaderTag gstag;

			{
				if (GUI::Button("exit")) {
					isShowShader = false;
				}
				GUI::BulletText("VertexShaderTag");
				auto tagName = GetTagNameVertexShader(vstag);
				(GUI::BeginChild("VSTagWin", Vector2(GUI::GetFontSize() * (tagName.size() + 2), GUI::GetFontSize() * 2), true));
				GUI::Text(Util::ToUTF8(tagName).c_str());

				if (GUI::IsWindowHovered()) {
					auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetVertexShaderTag();
					if (!tag.IsEmpty()) {
						vstag = tag;
					}
				}


				GUI::EndChild();

			}
			{

				GUI::BulletText("PixelShaderTag");
				auto tagName = GetTagNamePixelShader(pstag);
				(GUI::BeginChild("PSTagWin", Vector2(GUI::GetFontSize() * (tagName.size() + 2), GUI::GetFontSize() * 2), true));
				GUI::Text(Util::ToUTF8(tagName).c_str());

				if (GUI::IsWindowHovered()) {
					auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetPixelShaderTag();
					if (!tag.IsEmpty()) {
						pstag = tag;
					}
				}


				GUI::EndChild();

			}
			{

				GUI::BulletText("GeometryShaderTag");
				auto tagName = GetTagNameGeometryShader(gstag);
				(GUI::BeginChild("GSTagWin", Vector2(GUI::GetFontSize() * (tagName.size() + 2), GUI::GetFontSize() * 2), true));
				GUI::Text(Util::ToUTF8(tagName).c_str());

				if (GUI::IsWindowHovered()) {
					auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetGeometryShaderTag();
					if (!tag.IsEmpty()) {
						gstag = tag;
					}
				}


				GUI::EndChild();

			}

			GUI::Text("ShaderName");
			GUI::InputText("##edit", GUI::objectName, 128);
			if (GUI::Button("OK!!") &&  !vstag.IsEmpty() && !pstag.IsEmpty()) {

				ShaderInfo sn;

				sn.shaderName = GUI::objectName;
				if (gstag.IsEmpty()) {
					sn.geometryShaderTag= GeometryShaderTag();
				}
				else {
					sn.geometryShaderTag = gstag;
				}
				sn.vertexShaderTag = (vstag);
				sn.pixelShaderTag = (pstag);

				LoadShader(sn);

				GUI::ObjectNameReset();
				GUI::SameLine();
			}
			GUI::End();
		}

	}
	if (isShowAddMaterial) {

		GUI::Begin("AddMaterial");
		{

			if (GUI::Button("exit")) {
				isShowAddMaterial= false;
			}
			static TextureTag textag;
			static MaterialVariable matVar;
			{

				GUI::BulletText("TextureTag");
				auto tagName = GetTagNameTexture(textag);
				(GUI::BeginChild("TexTagWin", Vector2(GUI::GetFontSize() * (tagName.size() + 2), GUI::GetFontSize() * 2), true));
				GUI::Text(Util::ToUTF8(tagName).c_str());

				if (GUI::IsWindowHovered()) {
					auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetTextureTag();
					if (!tag.IsEmpty()) {
						textag = tag;
					}
				}


				GUI::EndChild();

			}
			GUI::Text("MaterialName");
			GUI::InputText("##edit", GUI::objectName, 128);
			GUI::ColorEdit4("diffuse", matVar.diffuse);
			GUI::ColorEdit4("ambient", matVar.ambient);
			GUI::ColorEdit4("emissive", matVar.emissive);
			GUI::ColorEdit4("specular", matVar.specular);

			if (GUI::Button("OK!!")) {

				MaterialLoadInfo sn;
				sn.var = matVar;
				sn.materialName = GUI::objectName;
				sn.vec_texture.push_back(textag);
				LoadMaterial(sn.var, sn.vec_texture, sn.materialName);
				GUI::ObjectNameReset();
				GUI::SameLine();
			}
			GUI::End();
		}

	}
}



ButiEngine::MaterialTag ButiEngine::ResourceContainer::LoadMaterial(const MaterialVariable& arg_resourceMaterial, const std::vector< TextureTag>& arg_textureTag, const std::string& arg_filePath, const std::string& arg_fileDirectory)
{

	if (container_materials.ContainValue(arg_fileDirectory + arg_filePath)) {
		return container_materials.GetTag(arg_fileDirectory + arg_filePath);
	}
	MaterialLoadInfo info;
	info.materialName = arg_fileDirectory + arg_filePath;
	info.var = arg_resourceMaterial;
	info.vec_texture=(arg_textureTag);
	vec_materialLoadInfos.push_back(info);
	return container_materials.AddValue(unq_resourceFactory->CreateMaterial(arg_resourceMaterial, arg_textureTag), arg_fileDirectory + arg_filePath);
}

ButiEngine::MaterialTag ButiEngine::ResourceContainer::LoadMaterial(const std::string& arg_filePath, const std::string& arg_fileDirectory)
{
	if (container_materials.ContainValue(arg_fileDirectory + arg_filePath)) {
		return container_materials.GetTag(arg_fileDirectory + arg_filePath);
	}
	MaterialLoadInfo info;
	info.materialName = arg_fileDirectory + arg_filePath;
	info.fileName = arg_fileDirectory + arg_filePath;
	vec_materialLoadInfos.push_back(info);
	return container_materials.AddValue(unq_resourceFactory->CreateMaterial(arg_filePath, arg_fileDirectory), arg_fileDirectory + arg_filePath);

}

std::vector < ButiEngine::MaterialTag> ButiEngine::ResourceContainer::LoadMaterial(const std::vector<MaterialLoadInfo>& arg_vec_filePathAndDirectory)
{
	std::vector<MaterialTag> out;
	out.reserve(arg_vec_filePathAndDirectory.size());

	for (auto itr = arg_vec_filePathAndDirectory.begin(); itr != arg_vec_filePathAndDirectory.end(); itr++) {
		if(itr->fileName!="none")
		out.push_back(LoadMaterial(itr->fileName));
		else {
			out.push_back(LoadMaterial(itr->var,itr->vec_texture,itr->materialName));
		}

		
	}

	return out;
}
std::vector < ButiEngine::MaterialTag> ButiEngine::ResourceContainer::LoadMaterial(const std::vector<std::string>& arg_vec_filePathAndDirectory)
{
	std::vector<MaterialTag> out;
	out.reserve(arg_vec_filePathAndDirectory.size());

	for (auto itr = arg_vec_filePathAndDirectory.begin(); itr != arg_vec_filePathAndDirectory.end(); itr++) {
		out.push_back(LoadMaterial(*itr));
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
	if (arg_filePath[0] == ':') {

		if (container_textures.ContainValue( arg_filePath)) {
			return container_textures.GetTag(arg_filePath);
		}


		auto split = StringHelper::Split(arg_filePath, "/");
		if (split.size() < 4) {
			return TextureTag();
		}
		vec_filePathAndDirectory_tex.push_back( arg_filePath);

		auto tex = unq_resourceFactory->CreateRenderTargetTexture(std::stoi(split[2]), std::stoi(split[3]));
		tex->SetFilePath(arg_filePath);
		return container_textures.AddValue(tex,arg_filePath);
	}
	else if (arg_filePath[0] == ';') {
		if (container_textures.ContainValue(arg_filePath)) {
			return container_textures.GetTag(arg_filePath);
		}


		auto split = StringHelper::Split(arg_filePath, "/");
		if (split.size() < 4) {
			return TextureTag();
		}
		vec_filePathAndDirectory_tex.push_back(arg_filePath);

		auto tex = unq_resourceFactory->CreateDepthStencilTexture(std::stoi(split[2]), std::stoi(split[3]));
		tex->SetFilePath(arg_filePath);
		return container_textures.AddValue(tex, arg_filePath);
	}

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

std::vector < ButiEngine::TextureTag>  ButiEngine::ResourceContainer::LoadTexture(const std::vector<std::string>& arg_vec_filePathAndDirectory)
{
	std::vector<TextureTag> out;
	out.reserve(arg_vec_filePathAndDirectory.size());

	for (auto itr = arg_vec_filePathAndDirectory.begin(); itr != arg_vec_filePathAndDirectory.end(); itr++) {
		out.push_back(LoadTexture(*itr));
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

std::vector<ButiEngine::PixelShaderTag> ButiEngine::ResourceContainer::LoadPixelShader(const std::vector<std::string>& arg_vec_filePathAndDirectory)
{
	std::vector<PixelShaderTag> out;
	out.reserve(arg_vec_filePathAndDirectory.size());

	for (auto itr = arg_vec_filePathAndDirectory.begin(); itr != arg_vec_filePathAndDirectory.end(); itr++) {
		out.push_back(LoadPixelShader(*itr));
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

std::vector<ButiEngine::VertexShaderTag> ButiEngine::ResourceContainer::LoadVertexShader(const std::vector<std::string>& arg_vec_filePathAndDirectory)
{
	std::vector<VertexShaderTag> out;
	out.reserve(arg_vec_filePathAndDirectory.size());

	for (auto itr = arg_vec_filePathAndDirectory.begin(); itr != arg_vec_filePathAndDirectory.end(); itr++) {
		out.push_back(LoadVertexShader(*itr));
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

std::vector< ButiEngine::GeometryShaderTag> ButiEngine::ResourceContainer::LoadGeometryShader(const std::vector<std::string>& arg_vec_filePathAndDirectory)
{
	std::vector<GeometryShaderTag> out;
	out.reserve(arg_vec_filePathAndDirectory.size());

	for (auto itr = arg_vec_filePathAndDirectory.begin(); itr != arg_vec_filePathAndDirectory.end(); itr++) {
		out.push_back(LoadGeometryShader(*itr));
	}

	return out;
}

ButiEngine::ShaderTag ButiEngine::ResourceContainer::LoadShader(const  ShaderInfo& arg_shaderNames)
{
	if (container_shaders.ContainValue(arg_shaderNames.shaderName)) {
		return ShaderTag();
	}

	vec_shaderNames.push_back(arg_shaderNames);
	std::shared_ptr<IResource_GeometryShader> gsResource = nullptr;
	if(!arg_shaderNames.geometryShaderTag.IsEmpty())
	container_geometryShaders.GetValue(arg_shaderNames.geometryShaderTag);

	return container_shaders.AddValue(ObjectFactory::Create<Resource_Shader>(container_vertexShaders.GetValue(arg_shaderNames.vertexShaderTag), container_pixelShaders.GetValue( arg_shaderNames.pixelShaderTag),gsResource
		,  arg_shaderNames.vertexShaderTag.GetID() +  arg_shaderNames.pixelShaderTag.GetID() + arg_shaderNames.geometryShaderTag.GetID()
		), arg_shaderNames.shaderName);
}


std::vector < ButiEngine::ShaderTag> ButiEngine::ResourceContainer::LoadShader(const std::vector<ShaderInfo>& arg_vec_shaderNames)
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

std::vector<ButiEngine::SoundTag> ButiEngine::ResourceContainer::LoadSound(const std::vector<std::string>& arg_vec_filePathAndDirectory)
{
	std::vector<SoundTag> out;
	out.reserve(arg_vec_filePathAndDirectory.size());

	for (auto itr = arg_vec_filePathAndDirectory.begin(); itr != arg_vec_filePathAndDirectory.end(); itr++) {
		out.push_back(LoadSound(*itr));
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

std::vector < ButiEngine::ModelTag>  ButiEngine::ResourceContainer::LoadModel(const std::vector<std::string>& arg_vec_filePathAndDirectory)
{
	std::vector<ModelTag> out;
	out.reserve(arg_vec_filePathAndDirectory.size());

	for (auto itr = arg_vec_filePathAndDirectory.begin(); itr != arg_vec_filePathAndDirectory.end(); itr++) {
		out.push_back(LoadModel(*itr));
	}

	return out;
}

ButiEngine::ModelTag ButiEngine::ResourceContainer::LoadModel(std::shared_ptr<IResource_Model> arg_model, const std::string& arg_filePath, const std::string& arg_fileDirectory)
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

std::vector<ButiEngine::MotionTag> ButiEngine::ResourceContainer::LoadMotion(const std::vector<std::string>& arg_vec_filePathAndDirectory)
{
	std::vector<MotionTag> out;
	out.reserve(arg_vec_filePathAndDirectory.size());

	for (auto itr = arg_vec_filePathAndDirectory.begin(); itr != arg_vec_filePathAndDirectory.end(); itr++) {
		out.push_back(LoadMotion(*itr));
	}

	return out;
}

ButiEngine::MotionTag ButiEngine::ResourceContainer::LoadMotion(std::shared_ptr<IResource_Motion> arg_motion, const std::string& arg_filePath, const std::string& arg_fileDirectory)
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

	for (auto itr = vec_materialLoadInfos.begin(); itr != vec_materialLoadInfos.end(); itr++) {
		if ((*itr).materialName == tagName) {
			vec_materialLoadInfos.erase(itr);
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

ButiEngine::MeshTag ButiEngine::ResourceContainer::GetMeshTag(const MeshTag& arg_tag)
{
	return (arg_tag);
}

ButiEngine::TextureTag ButiEngine::ResourceContainer::GetTextureTag(const TextureTag& arg_tag)
{
	return (arg_tag);
}

ButiEngine::ShaderTag ButiEngine::ResourceContainer::GetShaderTag(const ShaderTag& arg_tag)
{
	return (arg_tag);
}

ButiEngine::SoundTag ButiEngine::ResourceContainer::GetSoundTag(const SoundTag& arg_tag)
{
	return (arg_tag);
}

ButiEngine::MaterialTag ButiEngine::ResourceContainer::GetMaterialTag(const MaterialTag& arg_tag)
{
	return (arg_tag);
}

ButiEngine::ModelTag ButiEngine::ResourceContainer::GetModelTag(const ModelTag& arg_tag)
{
	return (arg_tag);
}

ButiEngine::MotionTag ButiEngine::ResourceContainer::GetMotionTag(const MotionTag& arg_tag)
{
	return (arg_tag);
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

	auto  matCopy = vec_materialLoadInfos;
	vec_materialLoadInfos.clear();
	LoadMaterial(matCopy);

	copy = vec_filePathAndDirectory_model;
	vec_filePathAndDirectory_model.clear();
	for (auto itr = copy.begin(); itr != copy.end(); itr++) {
		(LoadModel(*itr));
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
