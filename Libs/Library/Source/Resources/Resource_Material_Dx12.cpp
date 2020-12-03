#include "stdafx.h"

#include"Header/GameParts/GraphicDevice_Dx12.h"
#include "..\..\Header\Resources\Resource_Material_Dx12.h"

ButiEngine::Resource_Material_Dx12::Resource_Material_Dx12(const MaterialVariable & arg_var, const TextureTag & arg_textureTag, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) :Resource_Material(arg_var)
{
	materialVar = arg_var;
	textureTag = arg_textureTag;
	wkp_graphicDevice = arg_wkp_graphicDevice;
}

ButiEngine::Resource_Material_Dx12::Resource_Material_Dx12(const MaterialVariable & arg_var, const std::vector<TextureTag>& arg_textureTag, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice) : Resource_Material(arg_var)
{
	materialVar = arg_var;
	textureTag = arg_textureTag.at(0);
	if (arg_textureTag.size() > 1) {
		sphereTextureTag= arg_textureTag.at(1);
	}
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

void ButiEngine::Resource_Material_Dx12::Attach(std::shared_ptr<IRenderer> arg_shp_renderer)
{
	arg_shp_renderer->TextureAttach(textureTag,0);
	materialBuffer->Attach();
}

void ButiEngine::Resource_Material_Dx12::Update()
{
	materialBuffer->Update();
}

