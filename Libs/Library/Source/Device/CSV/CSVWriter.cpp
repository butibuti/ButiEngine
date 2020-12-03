#include"stdafx.h"



ButiEngine::CSVWriter::CSVWriter()
{
}


ButiEngine::CSVWriter::~CSVWriter()
{
}
void  ButiEngine::CSVWriter::Write(CSVData arg_csvData, const std::string& filePath) {
	std::ofstream ofs(filePath);
	if (!ofs)
		return;
	std::string output = std::string();
	for (int i = 0; i < arg_csvData.size_y; i++) {
		for (int j = 0; j < arg_csvData.size_x; j++)
		{
			output += std::to_string(arg_csvData.data[j][i]);
			if (j != arg_csvData.size_x - 1) {
				output += ",";
			}
		}
		if (i != arg_csvData.size_y - 1) {
			output += "\n";
		}
	}
	ofs << output.c_str();
}

