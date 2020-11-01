#include"stdafx.h"

void ButiEngine::OutputCereal(std::shared_ptr<Transform>& v, const std::string& path)
{
	std::stringstream stream;


	cereal::BinaryOutputArchive binOutArchive(stream);

	binOutArchive(v);

	std::ofstream outputFile(path, std::ios::out);

	outputFile << stream.str();

	outputFile.close();
	stream.clear();
}

void ButiEngine::InputCereal(std::shared_ptr<Transform>& v, const std::string& path)
{
	std::stringstream stream;

	std::ifstream inputFile(path, std::ios::in);

	stream << inputFile.rdbuf();

	cereal::BinaryInputArchive binInputArchive(stream);


	binInputArchive(v);
}
