#pragma once
#include "stdafx.h"
#include "..\..\Header\GameParts\ResourceFactory_Dx11.h"
#include"..\..\Header\Resources\Resource_Shader_Dx11.h"
#include"..\..\Header\Resources\Resource_Texture_Dx11.h"
#include"..\..\Header\Resources\Resource_Material_Dx11.h"

ButiEngine::ResourceFactory_Dx11::ResourceFactory_Dx11(std::weak_ptr<GraphicDevice> arg_wkp_graphicDevice)
{
	wkp_graphicDevice = arg_wkp_graphicDevice;
}

std::shared_ptr<ButiEngine::Resource_Texture> ButiEngine::ResourceFactory_Dx11::CreateTextureFromFile(const std::string& filePath)
{
	auto data = GetTextureResourceDataFromFile(filePath);

	return ObjectFactory::Create<Resource_Texture_Dx11>(data.rawData.data(), data.width, data.height, wkp_graphicDevice.lock());

}

std::shared_ptr<ButiEngine::Resource_PixelShader> ButiEngine::ResourceFactory_Dx11::CreatePixelShaderFromFile(const std::string& filePath)
{
	std::shared_ptr<Resource_PixelShader_Dx11> shader = std::make_shared<Resource_PixelShader_Dx11>(filePath, wkp_graphicDevice.lock());

	return shader;
}

std::shared_ptr<ButiEngine::Resource_VertexShader> ButiEngine::ResourceFactory_Dx11::CreateVertexShaderFromFile(const std::string& filePath)
{

	std::shared_ptr<Resource_VertexShader_Dx11> shader = std::make_shared<Resource_VertexShader_Dx11>(filePath, wkp_graphicDevice.lock());

	return shader;
}

std::shared_ptr< ButiEngine::Resource_GeometryShader> ButiEngine::ResourceFactory_Dx11::CreateGeometryShaderFromFile(const std::string& filePath)
{
	std::shared_ptr<Resource_GeometryShader_Dx11> shader = std::make_shared<Resource_GeometryShader_Dx11>(filePath, wkp_graphicDevice.lock());

	return shader;
}


std::shared_ptr<ButiEngine::Resource_Material> ButiEngine::ResourceFactory_Dx11::CreateMaterial(const MaterialVariable& arg_materialVar, const TextureTag& arg_textureTag)
{
	return ObjectFactory::Create<Resource_Material_Dx11>(arg_materialVar, arg_textureTag,wkp_graphicDevice.lock()->GetThis<GraphicDevice_Dx11>());
}

std::shared_ptr<ButiEngine::Resource_Material> ButiEngine::ResourceFactory_Dx11::CreateMaterial(const MaterialVariable& arg_materialVar, const std::vector<TextureTag>& arg_textureTag)
{
	return ObjectFactory::Create<Resource_Material_Dx11>(arg_materialVar, arg_textureTag, wkp_graphicDevice.lock()->GetThis<GraphicDevice_Dx11>());
}
