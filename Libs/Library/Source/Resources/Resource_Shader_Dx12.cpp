#include"stdafx.h"
#include"Header/Device/GraphicResourceUtil_Dx12.h"
#include"Header/Device/Helper/ShaderHelper.h"

#include"Header/GameParts/GraphicDevice_Dx12.h"
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
ButiEngine::Resource_PixelShader_Dx12::Resource_PixelShader_Dx12( const std::string& fileName, std::shared_ptr<GraphicDevice> arg_graphicDevice)
	
{
	wkp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
	
	//Dx12Compile::Compile(source, fileName, "PSMain", "ps_5_1");


	Dx12Compile::ReadCompiled(fileName,pixelShaderBlob);
	
}


ButiEngine::Resource_PixelShader_Dx12::~Resource_PixelShader_Dx12()
{

}

void ButiEngine::Resource_PixelShader_Dx12::Attach() const
{

}

void ButiEngine::Resource_PixelShader_Dx12::SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice)
{

	wkp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}


Microsoft::WRL::ComPtr<ID3D10Blob>& ButiEngine::Resource_PixelShader_Dx12::GetPixelShaderBlob()
{
	return pixelShaderBlob;
}

ButiEngine::Resource_VertexShader_Dx12::Resource_VertexShader_Dx12( const std::string& filePath, std::shared_ptr<GraphicDevice> arg_graphicDevice)
{
	wkp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();

	{
		//Dx12Compile::Compile(source, filePath, "VSMain", "vs_5_1");
		
		Dx12Compile::ReadCompiled(filePath,vertexShaderBlob);
		Dx12Compile::ReadCompiledInputLayout(filePath, vec_inputElementDesc);

	}
}

ButiEngine::Resource_VertexShader_Dx12::~Resource_VertexShader_Dx12()
{
	for (int i = 0; i < vec_inputElementDesc.size(); i++) {
		delete (vec_inputElementDesc.at(i).SemanticName);
	}
}

void ButiEngine::Resource_VertexShader_Dx12::Attach() const
{
}

void ButiEngine::Resource_VertexShader_Dx12::SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice)
{
	wkp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}

Microsoft::WRL::ComPtr<ID3DBlob>& ButiEngine::Resource_VertexShader_Dx12::GetVertexShaderBlob()
{
	return vertexShaderBlob;
}

std::vector<D3D12_INPUT_ELEMENT_DESC>& ButiEngine::Resource_VertexShader_Dx12::GetInputLayoutVector()
{
	return vec_inputElementDesc;
}

void ButiEngine::Dx12Compile::Compile(const std::string& source, const std::string& filePath, const char* entryPoint, const char* shaderModel)
{
	Microsoft::WRL::ComPtr<ID3DBlob> error = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> buffer = nullptr;
	auto hr = D3DCompile(source.c_str(), source.length(), filePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel, D3DCOMPILE_ENABLE_STRICTNESS, 0,buffer.GetAddressOf(), error.GetAddressOf());

	if (error != nullptr) {
		MessageBoxA(nullptr, (char*)error->GetBufferPointer(), "Shader error", MB_ICONERROR | MB_OK);
	}
	auto dirPath =StringHelper::GetDirectory( filePath)+"Compiled/";
	
	auto fileName = StringHelper::GetFileName(filePath,false);

	auto dirRes = _mkdir(dirPath.c_str());
	BinaryWriter shaderWriter;
	shaderWriter.WriteStart(dirPath+fileName + ".dx12cps");
	shaderWriter.WriteVariable<int>(buffer->GetBufferSize());
	shaderWriter.WriteCharactor((char*)buffer->GetBufferPointer(),buffer->GetBufferSize());
	shaderWriter.WriteEnd();


}

void ButiEngine::Dx12Compile::InputLayoutCompile(const std::string& source, const std::string& filePath)
{
	std::vector<D3D12_INPUT_ELEMENT_DESC> vec_inputElementDesc;
	ShaderHelper::CreateDx12InputElementVector(filePath, source, vec_inputElementDesc);

	auto dirPath = StringHelper::GetDirectory(filePath) + "Compiled/";

	auto fileName = StringHelper::GetFileName(filePath, false);
	BinaryWriter inputLayoutWriter;
	inputLayoutWriter.WriteStart(dirPath + fileName + ".dx12ied");
	inputLayoutWriter.WriteVariable<int>(vec_inputElementDesc.size());
	for (int i = 0; i < vec_inputElementDesc.size(); i++) {
		auto write = vec_inputElementDesc.at(i);
		std::string semantic = write.SemanticName;
		inputLayoutWriter.WriteVariable<int>(semantic.size());
		inputLayoutWriter.WriteCharactor(semantic);
		inputLayoutWriter.WriteVariable<DXGI_FORMAT>(write.Format);
		inputLayoutWriter.WriteVariable<UINT>(write.InputSlot);
		inputLayoutWriter.WriteVariable<UINT>(write.AlignedByteOffset);
		inputLayoutWriter.WriteVariable<D3D12_INPUT_CLASSIFICATION>(write.InputSlotClass);
		inputLayoutWriter.WriteVariable<UINT>(write.InstanceDataStepRate);
		inputLayoutWriter.WriteVariable<UINT>(write.SemanticIndex);
	}
	inputLayoutWriter.WriteEnd();

	for (int i = 0; i < vec_inputElementDesc.size(); i++) {

		delete (vec_inputElementDesc.at(i).SemanticName);
	}
	vec_inputElementDesc.clear();

}

void ButiEngine::Dx12Compile::ReadCompiled(const std::string& filePath,  Microsoft::WRL::ComPtr<ID3DBlob>& out)
{


	auto dirPath = StringHelper::GetDirectory(filePath) ;

	auto fileName = StringHelper::GetFileName(filePath, false);


	BinaryReader shaderReader;
	shaderReader.ReadStart(dirPath + fileName + ".dx12cps");

	int size = shaderReader.ReadVariable<int>();

	auto hr = D3DCreateBlob(size, out.GetAddressOf());
	auto compiledData = shaderReader.ReadCharactor();

	memcpy(out->GetBufferPointer(), compiledData,size);
	shaderReader.ReadEnd();

	delete compiledData;
}

void ButiEngine::Dx12Compile::ReadCompiledInputLayout(const std::string& filePath, std::vector<D3D12_INPUT_ELEMENT_DESC>& vec_inputElementDesc)
{
	auto dirPath = StringHelper::GetDirectory(filePath);
	BinaryReader inputLayoutReader;

	auto fileName = StringHelper::GetFileName(filePath.c_str(),false);

	inputLayoutReader.ReadStart(dirPath + fileName + ".dx12ied");
	auto size = inputLayoutReader.ReadVariable<int>();
	vec_inputElementDesc.reserve(size);
	for (int i = 0; i < size; i++) {
		auto read = D3D12_INPUT_ELEMENT_DESC();
		auto semanticSize = inputLayoutReader.ReadVariable<int>();
		auto semanticName = inputLayoutReader.ReadCharactor(semanticSize);
		char* semanticP = (char*)malloc(semanticName.size() + 1);
		memcpy(semanticP, semanticName.c_str(), semanticName.size() + 1);
		read.SemanticName = semanticP;
		read.Format = inputLayoutReader.ReadVariable<DXGI_FORMAT>();
		read.InputSlot = inputLayoutReader.ReadVariable<UINT>();
		read.AlignedByteOffset = inputLayoutReader.ReadVariable<UINT>();
		read.InputSlotClass = inputLayoutReader.ReadVariable<D3D12_INPUT_CLASSIFICATION>();
		read.InstanceDataStepRate = inputLayoutReader.ReadVariable<UINT>();
		read.SemanticIndex = inputLayoutReader.ReadVariable<UINT>();

		vec_inputElementDesc.push_back(read);
	}

	inputLayoutReader.ReadEnd();
}

ButiEngine::Resource_GeometryShader_Dx12::Resource_GeometryShader_Dx12(const std::string& fileName, std::shared_ptr<GraphicDevice> arg_graphicDevice)
{
	Dx12Compile::ReadCompiled(fileName,geometryShaderBlob);
}

ButiEngine::Resource_GeometryShader_Dx12::~Resource_GeometryShader_Dx12()
{
}

void ButiEngine::Resource_GeometryShader_Dx12::Attach() const
{
}

void ButiEngine::Resource_GeometryShader_Dx12::SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice)
{
	wkp_graphicDevice = arg_graphicDevice->GetThis<GraphicDevice_Dx12>();
}

Microsoft::WRL::ComPtr<ID3DBlob>& ButiEngine::Resource_GeometryShader_Dx12::GetGeometryShaderBlob()
{
	return geometryShaderBlob;
}
