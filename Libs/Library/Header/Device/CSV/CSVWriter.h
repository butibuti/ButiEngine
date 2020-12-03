#pragma once
#include "stdafx.h"

namespace ButiEngine {

	class CSVWriter
	{
	public:
		~CSVWriter();
		static void Write(CSVData arg_csvData, const std::string& filePath);
	private:
		CSVWriter();
	};

}