#pragma once
#include "stdafx.h"
#include "..\..\Header\GameParts\ResourceFactory_Dx12.h"
#include"..\..\Header\Resources\Resource_Shader_Dx12.h"
#include"..\..\Header\Resources\Resource_Texture_Dx12.h"
#include"..\..\Header\Resources\Resource_Texture_Dx12_RenderTarget.h"
#include"..\..\Header\Resources\Resource_Material_Dx12.h"


ButiEngine::ResourceFactory_Dx12::ResourceFactory_Dx12(std::weak_ptr<GraphicDevice> arg_wkp_graphicDevice)
{
	wkp_graphicDevice = arg_wkp_graphicDevice;
}

std::shared_ptr<ButiEngine::Resource_Texture> ButiEngine::ResourceFactory_Dx12::CreateTextureFromFile(const std::string& filePath)
{
	auto data = GetTextureResourceDataFromFile(filePath);

	return ObjectFactory::Create<Resource_Texture_Dx12>(data.rawData, data.width, data.height, wkp_graphicDevice.lock());
}

std::shared_ptr<ButiEngine::Resource_Texture> ButiEngine::ResourceFactory_Dx12::CreateRenderTargetTexture(const UINT width, const UINT height)
{
	return ObjectFactory::Create<Resource_Texture_Dx12_RenderTarget>( width,height, wkp_graphicDevice.lock());
}

std::shared_ptr<ButiEngine::Resource_PixelShader> ButiEngine::ResourceFactory_Dx12::CreatePixelShaderFromFile(const std::string& filePath)
{

	std::shared_ptr<Resource_PixelShader_Dx12> shader = std::make_shared<Resource_PixelShader_Dx12>(filePath,wkp_graphicDevice.lock());

	return shader;
}

std::shared_ptr<ButiEngine::Resource_VertexShader> ButiEngine::ResourceFactory_Dx12::CreateVertexShaderFromFile(const std::string& filePath)
{

	std::shared_ptr<Resource_VertexShader_Dx12> shader = std::make_shared<Resource_VertexShader_Dx12>(filePath, wkp_graphicDevice.lock());

	return shader;
}

std::shared_ptr<ButiEngine::Resource_GeometryShader> ButiEngine::ResourceFactory_Dx12::CreateGeometryShaderFromFile(const std::string& filePath)
{
	std::shared_ptr<Resource_GeometryShader_Dx12> shader = std::make_shared<Resource_GeometryShader_Dx12>(filePath, wkp_graphicDevice.lock());

	return shader;
}


std::shared_ptr<ButiEngine::Resource_Material> ButiEngine::ResourceFactory_Dx12::CreateMaterial(const MaterialVariable& arg_materialVar, const TextureTag& arg_textureTag)
{
	return ObjectFactory::Create<Resource_Material_Dx12>(arg_materialVar, arg_textureTag, wkp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>());
}

std::shared_ptr<ButiEngine::Resource_Material> ButiEngine::ResourceFactory_Dx12::CreateMaterial(const MaterialVariable& arg_materialVar, const std::vector<TextureTag>& arg_textureTag)
{
	return ObjectFactory::Create<Resource_Material_Dx12>(arg_materialVar, arg_textureTag, wkp_graphicDevice.lock()->GetThis<GraphicDevice_Dx12>());
}
