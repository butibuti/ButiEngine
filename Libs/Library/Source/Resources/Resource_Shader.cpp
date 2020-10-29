#include"stdafx.h"

#include"Header/Resources/Resource_Shader.h"

ButiEngine::Resource_Shader::Resource_Shader(std::weak_ptr<IResource_VertexShader> arg_wkp_vertexShader, std::weak_ptr<IResource_PixelShader> arg_wkp_pixelShader, std::weak_ptr<IResource_GeometryShader> arg_wkp_geometryShader, const std::string& arg_shaderName)
{
	wkp_vertexShader = arg_wkp_vertexShader;
	wkp_pixelShader = arg_wkp_pixelShader;
	wkp_geometryShader = arg_wkp_geometryShader;
	shaderName = arg_shaderName;
}

void ButiEngine::Resource_Shader::Attach() const
{
	wkp_vertexShader.lock()->Attach();
	wkp_pixelShader.lock()->Attach();
	if (wkp_geometryShader.lock()) {
		wkp_geometryShader.lock()->Attach();
	}
}

bool ButiEngine::Resource_Shader::GetIsContainGeometryShader() const
{
	if (wkp_geometryShader.lock()) {
		return true;
	}
	return false;
}

std::string ButiEngine::Resource_Shader::GetShaderName()const
{
	return shaderName;
}

std::weak_ptr<ButiEngine::IResource_VertexShader> ButiEngine::Resource_Shader::GetVertexShader()
{
	return wkp_vertexShader;
}

std::weak_ptr<ButiEngine::IResource_PixelShader> ButiEngine::Resource_Shader::GetPixelShader()
{
	return wkp_pixelShader;
}

std::weak_ptr<ButiEngine::IResource_GeometryShader> ButiEngine::Resource_Shader::GetGeometryShader()
{
	return wkp_geometryShader;
}