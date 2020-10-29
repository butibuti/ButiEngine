#pragma once
#include"StringHelper.h"
namespace ButiEngine {
	typedef long long longLong;
	typedef unsigned long long ulongLong;
	typedef unsigned short ushort;

	namespace BitFlag {
		const unsigned int FLAG_MAX = 16;
		const unsigned int BIT_FLAG[]{ (1 << 0),(1 << 1),(1 << 2),(1 << 3),(1 << 4),(1 << 5),(1 << 6),(1 << 7),(1 << 8),(1 << 9),(1 << 10),(1 << 11),(1 << 12),(1 << 13),(1 << 14),(1 << 15) };
	}
	class BinaryHelper {
	public:
		template<typename T>
		static inline T SwapBytes(const T& input) {
			T output;
			char* conv = (char*)&input;
			char* ret = (char*)&output;
			auto size = sizeof(T);
			for (int i = 0; i < size; i++) {
				ret[i] = conv[size - 1 - i];
			}
			return output;
		}

		static int SwapByte(const int& arg_int);
		static UINT SwapByte(const UINT& arg_UINT);
		static float SwapByte(const float& arg_float);
		static double SwapByte(const double& arg_double);
	private:

		static short Swap16bit(const short& input);
		static int Swap32bit(const int& input);
		static longLong Swap64bit(const longLong& input);

		static ushort Swap16bit(const ushort& input);
		static UINT Swap32bit(const UINT& input);
		static ulongLong Swap64bit(const ulongLong& input);
		BinaryHelper();
	};

	class BinaryReader
	{
	public:
		bool ReadStart(const std::string& filePath);
		void ReadEnd();
		std::string ReadCharactor(const UINT& count);
		char* ReadCharactor();
		
		std::wstring ReadWCharactor(const UINT& count);

		inline bool IsEOF() {
			return fin.eof();
		}

		inline void Skip(int skipByte) {
			auto currentPos = fin.tellg();
			fin.seekg(((int)currentPos + skipByte), std::ios_base::beg);
		}

		template<typename T>
		inline T ReadVariable() {
			T out;
			fin.read((char*)&out, sizeof(T));

			BinaryHelper::SwapBytes(out);
			return out;
		}
		inline int ReadInt(const int& readRange) {
			int out;
			if (readRange == 1)
			{
				char byte1;
				fin.read((char*)&byte1, readRange);
				out = (int)byte1;
			}
			else if (readRange == 2)
			{
				short byte2;
				fin.read((char*)&byte2, readRange);
				out = (int)byte2;
			}
			else
			{
				fin.read((char*)&out, 4);
			}
			BinaryHelper::SwapBytes(out);
			return out;
		}
		inline UINT ReadUINT(const int& readRange) {
			UINT out;
			if (readRange == 1)
			{
				unsigned char byte1;
				fin.read((char*)&byte1, readRange);
				out = (UINT)byte1;
			}
			else if (readRange == 2)
			{
				unsigned short byte2;
				fin.read((char*)&byte2, readRange);
				out = (UINT)byte2;
			}
			else
			{
				fin.read((char*)&out, 4);
			}
			BinaryHelper::SwapBytes(out);
			return out;
		}
		inline std::streampos GetNowPos() {
			return fin.tellg();
		}
		static inline bool GetBitFlag(const int& arg_flag, const int& arg_bitIndex) {
			if (arg_bitIndex >= 16 || arg_bitIndex < 0) {
				return false;
			}
			return arg_flag & BitFlag::BIT_FLAG[arg_bitIndex];
		}
		static inline char GetBitNum(const char& arg_byte, const int& arg_readRange) {

			char out = arg_byte;
			out <<= (8 - arg_readRange);
			out >>= (8 - arg_readRange);


			return std::abs(out);
		}
		static inline char GetBitNum(const char& arg_byte, const int& arg_readRange, const int& arg_offset) {

			char out = arg_byte;
			out >>= (arg_offset);
			out <<= (8 - arg_readRange);
			out >>= (8 - arg_readRange);


			return std::abs(out);
		}
		BinaryReader() {};
	private:
		std::ifstream fin;
	};
	class BinaryWriter {
	public:
		bool WriteStart(const std::string& filePath);
		void WriteEnd();
		void WriteCharactor(const std::string& write);
		void WriteCharactor(const char* write, const UINT& size);
		void WriteWCharactor(const  std::wstring& write);

		template<typename T>
		inline void WriteVariable(T  writeVar) {

			BinaryHelper::SwapBytes(writeVar);

			fout.write((char*)&writeVar, sizeof(T));


		}
		inline void WriteInt(const int& write, const UINT& readRange) {
			int out = write;

			BinaryHelper::SwapBytes(out);
			if (readRange == 1)
			{
				char byte1 = write;
				fout.write((char*)&byte1, readRange);
			}
			else if (readRange == 2)
			{
				short byte2 = write;
				fout.write((char*)&byte2, readRange);
			}
			else
			{
				fout.write((char*)&out, 4);
			}
		}
		inline void WriteUINT(const UINT& write, const UINT& readRange) {
			UINT out = write;

			BinaryHelper::SwapBytes(out);
			if (readRange == 1)
			{
				unsigned char byte1 = write;
				fout.write((char*)&byte1, readRange);
			}
			else if (readRange == 2)
			{
				unsigned short byte2 = write;
				fout.write((char*)&byte2, readRange);
			}
			else
			{
				fout.write((char*)&out, 4);
			}
		}
		inline void Jump(std::streampos jumpPos) {
			fout.seekp(jumpPos, std::ios_base::beg);
		}
		inline std::streampos GetNowPos() {
			return fout.tellp();
		}
		BinaryWriter() {};
	private:
		std::ofstream fout;
	};
}

