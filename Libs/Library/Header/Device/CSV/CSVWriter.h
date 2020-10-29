#pragma once
#include "stdafx.h"

namespace ButiEngine {

	class CSVWriter
	{
	public:
		~CSVWriter();
		static void Write(CSVData arg_csvData, const char filePath[]);
	private:
		CSVWriter();
	};

}