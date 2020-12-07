#include"stdafx.h"
#include"Header/Common/CerealUtill.h"
//CEREAL_REGISTER_TYPE(ButiEngine::SceneRenderingInformation);


ButiEngine::SceneInformation::~SceneInformation()
{
}

void ButiEngine::OutputCereal(const std::shared_ptr<SceneRenderingInformation>& v, std::string arg_path)
{
	std::stringstream stream;


	cereal::BinaryOutputArchive binOutArchive(stream);
	binOutArchive(v);

	std::ofstream outputFile(arg_path, std::ios::binary);

	outputFile << stream.str();

	outputFile.close();
	stream.clear();
}

void ButiEngine::InputCereal(std::shared_ptr<SceneRenderingInformation>& v, std::string arg_path)
{
	std::stringstream stream;

	std::ifstream inputFile( arg_path, std::ios::binary);

	stream << inputFile.rdbuf();

	cereal::BinaryInputArchive binInputArchive(stream);


	binInputArchive(v);

}
