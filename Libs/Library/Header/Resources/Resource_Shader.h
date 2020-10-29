#pragma once
#include"stdafx.h"
namespace ButiEngine {
	class GraphicDevice;
	class Resource_PixelShader :public IResource_PixelShader
	{
	public:
		void Initialize()override {}
		void PreInitialize()override {}
		virtual void Attach()const = 0;
		virtual void SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice) = 0;
	};
	class Resource_VertexShader :public IResource_VertexShader
	{
	public:
		void Initialize()override {}
		void PreInitialize()override {}
		virtual void Attach()const = 0;
		virtual void SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice) = 0;
	};
	class Resource_GeometryShader :public IResource_GeometryShader
	{
	public:
		void Initialize()override {}
		void PreInitialize()override {}
		virtual void Attach()const = 0;
		virtual void SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice) = 0;
	};

	class Resource_Shader :public IResource_Shader
	{
	public:
		Resource_Shader(std::weak_ptr<IResource_VertexShader> arg_wkp_vertexShader, std::weak_ptr<IResource_PixelShader> arg_wkp_pixelShader, std::weak_ptr<IResource_GeometryShader> arg_wkp_geometryShader, const std::string& arg_shaderName);
		void Initialize()override {}
		void PreInitialize()override {}
		virtual void Attach()const;

		bool GetIsContainGeometryShader()const;

		std::string GetShaderName()const;

		std::weak_ptr<IResource_VertexShader> GetVertexShader();
		std::weak_ptr<IResource_PixelShader> GetPixelShader();
		std::weak_ptr<IResource_GeometryShader> GetGeometryShader();
	private:
		std::weak_ptr<IResource_VertexShader> wkp_vertexShader;
		std::weak_ptr<IResource_PixelShader> wkp_pixelShader;
		std::weak_ptr<IResource_GeometryShader> wkp_geometryShader;
		std::string shaderName;
	};
}