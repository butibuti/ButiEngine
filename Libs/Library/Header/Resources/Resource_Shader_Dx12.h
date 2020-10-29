#pragma once
#include"stdafx.h"
#include"Resource_Shader.h"
namespace ButiEngine {

	namespace Dx12Compile {
		static void Compile(const std::string& source, const std::string& fileName, const char* entryPoint, const char* shaderModel);
		static void InputLayoutCompile(const std::string& source, const std::string& fileName);

		static void ReadCompiled(const std::string& fileName, Microsoft::WRL::ComPtr<ID3DBlob>& out);
		static void ReadCompiledInputLayout( const std::string& fileName, std::vector<D3D12_INPUT_ELEMENT_DESC>& vec_inputElementDesc);
	}

	class Resource_PixelShader_Dx12 :public Resource_PixelShader
	{
	public:
		Resource_PixelShader_Dx12(const std::string& fileName, std::shared_ptr<GraphicDevice> arg_graphicDevice);
		~Resource_PixelShader_Dx12();
		void Attach()const override;
		void SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice) override;
		
		Microsoft::WRL::ComPtr<ID3DBlob>& GetPixelShaderBlob();
	private:
		Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderBlob = nullptr;
		std::weak_ptr<GraphicDevice_Dx12> wkp_graphicDevice;
	};
	class Resource_VertexShader_Dx12 :public Resource_VertexShader
	{
	public:
		Resource_VertexShader_Dx12( const std::string& fileName, std::shared_ptr<GraphicDevice> arg_graphicDevice);
		~Resource_VertexShader_Dx12();
		void Attach()const override;
		void SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice) override;
		
		Microsoft::WRL::ComPtr<ID3DBlob>& GetVertexShaderBlob();
		std::vector<D3D12_INPUT_ELEMENT_DESC>& GetInputLayoutVector();
	private:
		Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderBlob = nullptr;
		std::vector<D3D12_INPUT_ELEMENT_DESC> vec_inputElementDesc;
		std::weak_ptr<GraphicDevice_Dx12> wkp_graphicDevice;
	};
	class Resource_GeometryShader_Dx12 :public Resource_GeometryShader {
	public:
		Resource_GeometryShader_Dx12(const std::string& fileName, std::shared_ptr<GraphicDevice> arg_graphicDevice);
		~Resource_GeometryShader_Dx12();
		void Attach()const override;
		void SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice) override;

		Microsoft::WRL::ComPtr<ID3DBlob>& GetGeometryShaderBlob();
	private:
		Microsoft::WRL::ComPtr<ID3DBlob> geometryShaderBlob = nullptr;
		std::weak_ptr<GraphicDevice_Dx12> wkp_graphicDevice;
	};
}