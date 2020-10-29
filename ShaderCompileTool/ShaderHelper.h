#pragma once 
#include"BinaryIO.h"
namespace ButiEngine {
	enum class CompileType {
		Dx11=0,Dx12=1
	};


	class ShaderHelper
	{

	public:
		static void Compile(const std::string& filePath,const CompileType& type);
		static void Compile(const std::string& filepath, const std::string& source, const char* entryPoint, const char* shaderModel, const CompileType& type);
		static void InputLayoutCompileDx11(const std::string& source, const std::string& fileName);
		static void InputLayoutCompileDx12(const std::string& source, const std::string& fileName);

		struct InputLayoutElementData {
			DXGI_FORMAT Format;
			std::string SemanticName;
		};
		static std::vector<InputLayoutElementData> CreateInputLayoutDataVector(const std::string& filePath, const std::string& source);
		static void ShaderFileInclude(const std::string& filePath, std::string& source);
		static void CreateDx11InputElementVector(const std::string& filePath, const std::string& source, std::vector<D3D11_INPUT_ELEMENT_DESC>& arg_vec_elementDesc);
		static void CreateDx12InputElementVector(const std::string& filePath, const std::string& source, std::vector<D3D12_INPUT_ELEMENT_DESC>& arg_vec_elementDesc);
		static DXGI_FORMAT GetFormat(const std::string& arg_formatStr, const std::string& arg_semanticName);
	};
}
