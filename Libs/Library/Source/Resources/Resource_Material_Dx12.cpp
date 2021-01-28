#include "stdafx.h"

#include"Header/GameParts/GraphicDevice_Dx12.h"
#include "..\..\Header\Resources\Resource_Material_Dx12.h"

ButiEngine::Resource_Material_Dx12::Resource_Material_Dx12(const MaterialVariable & arg_var, const TextureTag & arg_textureTag, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) :Resource_Material(arg_var)
{
	materialVar = arg_var;
	textureTag.push_back( arg_textureTag);
	wkp_graphicDevice = arg_wkp_graphicDevice;
}

ButiEngine::Resource_Material_Dx12::Resource_Material_Dx12(const MaterialVariable & arg_var, const std::vector<TextureTag>& arg_textureTag, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) : Resource_Material(arg_var)
{
	materialVar = arg_var;
	textureTag = arg_textureTag;
	wkp_graphicDevice = arg_wkp_graphicDevice;
}

void ButiEngine::Resource_Material_Dx12::Initialize()
{
	auto materialBuffer_Dx12 = ObjectFactory::Create<CBuffer_Dx12<MaterialVariable>>(1);
	materialBuffer_Dx12->SetGraphicDevice(wkp_graphicDevice.lock());

	materialBuffer_Dx12->Get().ambient = GetMaterialVariable().ambient;
	materialBuffer_Dx12->Get().emissive = GetMaterialVariable().emissive;
	materialBuffer_Dx12->Get().diffuse = GetMaterialVariable().diffuse;
	materialBuffer_Dx12->Get().specular = GetMaterialVariable().specular;
	materialBuffer_Dx12->CreateBuffer();
	materialBuffer = materialBuffer_Dx12;
	//wkp_graphicDevice.lock()->AddResource(&*materialBuffer);
}

void ButiEngine::Resource_Material_Dx12::Attach(const UINT slotOffset, std::shared_ptr<IRenderer> arg_shp_renderer)
{
	for (int i = 0; i < textureTag.size(); i++) {
		arg_shp_renderer->TextureAttach(textureTag[i], i);
	}
	materialBuffer->Attach(slotOffset);
}

void ButiEngine::Resource_Material_Dx12::Update()
{
	materialBuffer->Update();
}

UINT ButiEngine::Resource_Material_Dx12::GetTextureCount() const
{
	return textureTag.size();
}

bool ButiEngine::Resource_Material_Dx12::OnShowUI()
{
	bool isEdit = false;

	if (GUI::ColorEdit4("ambient", materialBuffer->Get().ambient)) {
		isEdit = true;
	}
	if (GUI::ColorEdit4("emissive", materialBuffer->Get().emissive)) {
		isEdit = true;
	}
	if (GUI::ColorEdit4("diffuse", materialBuffer->Get().diffuse)) {
		isEdit = true;
	}
	if (GUI::ColorEdit4("specular", materialBuffer->Get().specular)) {
		isEdit = true;
	}

	GUI::BulletText("TextureTags");
	for (auto itr = textureTag.begin(); itr != textureTag.end();) {

		auto tagName =wkp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer()->GetTagNameTexture(*itr);
		(GUI::BeginChild("TexTagWin##"+tagName, Vector2(GUI::GetFontSize() * (tagName.size() + 2), GUI::GetFontSize() * 2), true));
		GUI::Text(Util::ToUTF8(tagName).c_str());

		if (GUI::IsWindowHovered()) {
			auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetTextureTag();
			if (!tag.IsEmpty()) {
				*itr= tag;
				isEdit = true;
			}
		}


		GUI::EndChild();
		GUI::SameLine();
		if (GUI::Button("Remove")) {
			isEdit = true;
			itr = textureTag.erase(itr);

		}
		else {
			itr++;
		}
	}

	std::string tagName = "newTexture";
	(GUI::BeginChild("TexTagWin##" + tagName, Vector2(GUI::GetFontSize() * (tagName.size() + 2), GUI::GetFontSize() * 2), true));

	GUI::Text(Util::ToUTF8(tagName).c_str());
	if (GUI::IsWindowHovered()) {
		auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetTextureTag();
		if (!tag.IsEmpty()) {
			textureTag.push_back(tag);
			isEdit = true;
		}
	}


	GUI::EndChild();

	if (isEdit) {
		Update();
	}
	return isEdit;
}

void ButiEngine::Resource_Material_Dx12::SetTextureTag(const UINT index, TextureTag arg_tag)
{
	if (index >= textureTag.size()) {
		for (int i = 0; textureTag.size() < index+1; i++) {
			textureTag.push_back(TextureTag());
		}
	}
	textureTag[index] = arg_tag;
}

void ButiEngine::Resource_Material_Dx12::SetMaterialVariable(const MaterialVariable& arg_var)
{
	materialBuffer->Get() = arg_var;
}

ButiEngine::MaterialVariable& ButiEngine::Resource_Material_Dx12::GetMaterialVariable()
{
	if(materialBuffer)
	return materialBuffer->Get();
	else {
		return materialVar;
	}
}

