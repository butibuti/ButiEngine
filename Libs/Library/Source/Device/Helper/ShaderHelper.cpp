#include "stdafx.h"
#include"Header/Device/Helper/ShaderHelper.h"
void ButiEngine::ShaderHelper::Compile(const std::string& filePath, const CompileType& type)
{

	std::ifstream ifs(filePath);
	std::string source = std::string(std::istreambuf_iterator<char>(ifs),
		std::istreambuf_iterator<char>());

	bool isVs = false;


	std::string entryPoint;
	std::string shaderModel;

	if (StringHelper::Contains(source, "VSMain")) {
		isVs = true;
		entryPoint = "VSMain";
		shaderModel = "vs_";
	}
	else if (StringHelper::Contains(source, "PSMain")) {
		entryPoint = "PSMain";
		shaderModel = "ps_";
	}
	else if (StringHelper::Contains(source, "GSMain")) {
		entryPoint = "GSMain";
		shaderModel = "gs_";
	}
	else {
		std::cout << filePath << "にはエントリーポイントが見つかりません" << std::endl;
		return;
	}


	switch (type)
	{
	case CompileType::Dx11:


		shaderModel += "4_0";
		break;
	case CompileType::Dx12:

		shaderModel += "5_1";
		break;
	default:
		break;
	}

	Compile(filePath, source, entryPoint.c_str(), shaderModel.c_str(), type);

	if (isVs) {
		switch (type)
		{
		case CompileType::Dx11:

			InputLayoutCompileDx11(source, filePath);
			break;
		case CompileType::Dx12:
			InputLayoutCompileDx12(source, filePath);
			break;
		default:
			break;
		}
	}

}

void ButiEngine::ShaderHelper::Compile(const std::string& filePath, const std::string& source, const char* entryPoint, const char* shaderModel, const CompileType& type)
{


	Microsoft::WRL::ComPtr<ID3DBlob> error = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> buffer = nullptr;
	auto hr = D3DCompile(source.c_str(), source.length(), filePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel, D3DCOMPILE_ENABLE_STRICTNESS, 0, buffer.GetAddressOf(), error.GetAddressOf());
	if (error != nullptr) {
		MessageBoxA(nullptr, (char*)error->GetBufferPointer(), "Shader error", MB_ICONERROR | MB_OK);
	}
	else {
		std::cout << filePath << "のコンパイル成功" << std::endl;
	}
	auto dirPath = StringHelper::GetDirectory(filePath);

	if (dirPath == filePath) {
		dirPath = "Compiled/";
	}
	else {
		dirPath += "Compiled/";
	}

	auto fileName = StringHelper::GetFileName(filePath, false);

	auto dirRes = _mkdir(dirPath.c_str());
	BinaryWriter shaderWriter;

	std::string extension;
	switch (type)
	{
	case CompileType::Dx11:
		extension = ".dx11cps";
		break;
	case CompileType::Dx12:
		extension = ".dx12cps";
		break;
	default:
		break;
	}

	shaderWriter.WriteStart(dirPath + fileName + extension);
	shaderWriter.WriteVariable<int>(buffer->GetBufferSize());
	shaderWriter.WriteCharactor((char*)buffer->GetBufferPointer(), buffer->GetBufferSize());
	shaderWriter.WriteEnd();
}

void ButiEngine::ShaderHelper::InputLayoutCompileDx11(const std::string& source, const std::string& filePath)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> vec_inputElementDesc;
	ShaderHelper::CreateDx11InputElementVector(filePath, source, vec_inputElementDesc);

	auto dirPath = StringHelper::GetDirectory(filePath) + "Compiled/";

	auto fileName = StringHelper::GetFileName(filePath, false);
	BinaryWriter inputLayoutWriter;
	inputLayoutWriter.WriteStart(dirPath + fileName + ".dx11ied");
	inputLayoutWriter.WriteVariable<int>(vec_inputElementDesc.size());
	for (int i = 0; i < vec_inputElementDesc.size(); i++) {
		auto write = vec_inputElementDesc.at(i);
		std::string semantic = write.SemanticName;
		inputLayoutWriter.WriteVariable<int>(semantic.size());
		inputLayoutWriter.WriteCharactor(semantic);
		inputLayoutWriter.WriteVariable<DXGI_FORMAT>(write.Format);
		inputLayoutWriter.WriteVariable<UINT>(write.InputSlot);
		inputLayoutWriter.WriteVariable<UINT>(write.AlignedByteOffset);
		inputLayoutWriter.WriteVariable<D3D11_INPUT_CLASSIFICATION>(write.InputSlotClass);
		inputLayoutWriter.WriteVariable<UINT>(write.InstanceDataStepRate);
		inputLayoutWriter.WriteVariable<UINT>(write.SemanticIndex);
	}
	inputLayoutWriter.WriteEnd();

	for (int i = 0; i < vec_inputElementDesc.size(); i++) {

		delete (vec_inputElementDesc.at(i).SemanticName);
	}
	vec_inputElementDesc.clear();

}

void ButiEngine::ShaderHelper::InputLayoutCompileDx12(const std::string& source, const std::string& filePath)
{
	std::vector<D3D12_INPUT_ELEMENT_DESC> vec_inputElementDesc;
	ShaderHelper::CreateDx12InputElementVector(filePath, source, vec_inputElementDesc);

	auto dirPath = StringHelper::GetDirectory(filePath) + "Compiled/";

	auto fileName = StringHelper::GetFileName(filePath, false);
	BinaryWriter inputLayoutWriter;
	inputLayoutWriter.WriteStart(dirPath + fileName + ".dx12ied");
	auto descsSize = vec_inputElementDesc.size();
	inputLayoutWriter.WriteVariable<int>(descsSize);
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


std::vector<ButiEngine::ShaderHelper::InputLayoutElementData> ButiEngine::ShaderHelper::CreateInputLayoutDataVector(const std::string& filePath, const std::string& source)
{
	std::vector<InputLayoutElementData> output;

	std::string inputLayoutStr = source;

	if (StringHelper::Contains(source, "#include")) {


		ShaderFileInclude(filePath, inputLayoutStr);


	}
	std::string vsMain = StringHelper::Cut(inputLayoutStr, "VSMain(", ")", false);
	auto args = StringHelper::Split(vsMain, " ");
	std::string argmentType;
	for (auto itr = args.begin(); itr != args.end(); itr++) {
		if (itr->length() > 2) {
			argmentType = *itr;
			break;
		}
	}

	inputLayoutStr = StringHelper::Cut(inputLayoutStr, "struct " + argmentType + "\n{\n", "\n};", false);


	inputLayoutStr = StringHelper::RemoveComment(inputLayoutStr);

	inputLayoutStr = StringHelper::Remove(inputLayoutStr, "\t");
	inputLayoutStr = StringHelper::Remove(inputLayoutStr, "\n");

	auto vec_layout = StringHelper::Split(inputLayoutStr, ";");


	for (auto itr = vec_layout.begin(); itr != vec_layout.end(); itr++) {
		auto spaceSplitedSource = StringHelper::Split(*itr, " ");
		auto format = spaceSplitedSource.begin();
		while (format->size() < 2)
		{
			format++;
		}
		std::string semanticName = StringHelper::Remove(*(StringHelper::Split(*itr, ":").end() - 1), " ");
		output.push_back({ GetFormat(*format,semanticName),semanticName });
	}
	return output;
}

void ButiEngine::ShaderHelper::ShaderFileInclude(const std::string& filePath, std::string& source)
{
	while (StringHelper::Contains(source, "#include")) {
		std::string includeFileName = StringHelper::Cut(source, "#include\"", "\"", false);
		source = StringHelper::Remove(source, "#include\"" + includeFileName + "\"");
		auto path = StringHelper::GetDirectory(filePath);
		std::ifstream includeFile(path + includeFileName);
		source = std::string(std::istreambuf_iterator<char>(includeFile),
			std::istreambuf_iterator<char>()) + source;
	}
}

void ButiEngine::ShaderHelper::CreateDx11InputElementVector(const std::string& filePath, const std::string& source, std::vector<D3D11_INPUT_ELEMENT_DESC>& arg_vec_elementDesc)
{

	auto inputLayoutDatas = CreateInputLayoutDataVector(filePath, source);
	for (auto itr = inputLayoutDatas.begin(); itr != inputLayoutDatas.end(); itr++) {
		char* out = (char*)malloc(itr->SemanticName.size() + 1);
		strncpy_s(out, itr->SemanticName.size() + 1, itr->SemanticName.c_str(), itr->SemanticName.size());
		arg_vec_elementDesc.push_back(D3D11_INPUT_ELEMENT_DESC{ out,0,itr->Format,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

	}
}

void ButiEngine::ShaderHelper::CreateDx12InputElementVector(const std::string& filePath, const std::string& source, std::vector<D3D12_INPUT_ELEMENT_DESC>& arg_vec_elementDesc)
{
	auto inputLayoutDatas = CreateInputLayoutDataVector(filePath, source);
	for (auto itr = inputLayoutDatas.begin(); itr != inputLayoutDatas.end(); itr++) {

		char* out = (char*)malloc(itr->SemanticName.size() + 1);
		strncpy_s(out, itr->SemanticName.size() + 1, itr->SemanticName.c_str(), itr->SemanticName.size());

		arg_vec_elementDesc.push_back(D3D12_INPUT_ELEMENT_DESC{ out,0,itr->Format,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 });

	}
}



DXGI_FORMAT ButiEngine::ShaderHelper::GetFormat(const std::string& arg_formatStr, const std::string& arg_semanticName)
{
	if (arg_semanticName == "POSITION") {
		return DXGI_FORMAT_R32G32B32_FLOAT;
	}
	if (arg_formatStr == "float4") {
		return  DXGI_FORMAT_R32G32B32A32_FLOAT;
	}
	else if (arg_formatStr == "float3")
	{
		return  DXGI_FORMAT_R32G32B32_FLOAT;
	}
	else if (arg_formatStr == "float2")
	{
		return  DXGI_FORMAT_R32G32_FLOAT;
	}
	else if (arg_formatStr == "float")
	{
		return  DXGI_FORMAT_R32_FLOAT;
	}
	else if (arg_formatStr == "uint4")
	{

		return  DXGI_FORMAT_R32G32B32A32_UINT;
	}
	else if (arg_formatStr == "uint3")
	{

		return  DXGI_FORMAT_R32G32B32_UINT;
	}
	else if (arg_formatStr == "uint2")
	{

		return  DXGI_FORMAT_R32G32_UINT;
	}
	else if (arg_formatStr == "uint")
	{

		return  DXGI_FORMAT_R32_UINT;
	}
	else if (arg_formatStr == "min16uint")
	{
		return  DXGI_FORMAT_R16_UINT;
	}
	else if (arg_formatStr == "int4")
	{

		return  DXGI_FORMAT_R32G32B32A32_SINT;
	}
	else if (arg_formatStr == "int3")
	{
		return  DXGI_FORMAT_R32G32B32_SINT;
	}
	else if (arg_formatStr == "int2")
	{
		return  DXGI_FORMAT_R32G32_SINT;
	}
	else if (arg_formatStr == "int")
	{
		return  DXGI_FORMAT_R32_SINT;
	}
	else if (arg_formatStr == "min16int")
	{
		return  DXGI_FORMAT_R16_SINT;
	}
	return DXGI_FORMAT_UNKNOWN;
}

