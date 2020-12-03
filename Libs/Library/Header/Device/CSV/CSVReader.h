#pragma once
#include"stdafx.h"


namespace ButiEngine {

	class CSVReader
	{
	public:
		~CSVReader();
		static std::shared_ptr<CSVData> GetMatrix(const std::string& filePath);
	private:
		CSVReader();
	};

}