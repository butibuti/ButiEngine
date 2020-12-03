#include"stdafx.h"
#include "..\..\Header\Device\BinaryReader.h"
#include"zlib/zlib.h"
#pragma comment(lib,"ZDll.lib")

bool ButiEngine::BinaryReader::ReadStart(const std::string & filePath)
{
	fin=std::ifstream (filePath, std::ios::in | std::ios::binary);
	if (!fin) {
		std::cout << "ファイルが見つかりません\n";

		return false;
	}
	return true;
}

void ButiEngine::BinaryReader::ReadEnd()
{
	fin.close();
}

std::string ButiEngine::BinaryReader::ReadCharactor(const UINT count)
{
	char* readChars=(char*)malloc(count);

	fin.read(readChars, count);

	std::string out;

	for (int i = 0; i <count; i++) {
		if (readChars[i] > 9) {
			out += readChars[i];
			continue;
		}
		out += readChars[i];
	}
	delete readChars;
	return out;
}

char* ButiEngine::BinaryReader::ReadCharactor()
{
	auto all= std::string(std::istreambuf_iterator<char>(fin),
		std::istreambuf_iterator<char>());
	char* out = (char*)malloc(all.size());
	memcpy(out, all.c_str(), all.size());

	return out;
}

void* ButiEngine::BinaryReader::ReadData(const UINT size)
{
	void* out = malloc(size);

	fin.read((char*)out, size);

	return out;
}

void ButiEngine::BinaryReader::ReadData(char* out, const UINT size)
{
	out =(char*) malloc(size);

	fin.read((char*)out, size);
}

void ButiEngine::BinaryReader::ReadDefrateData(const UINT arg_compressedSize, UINT uncompressedSize, const UINT arraySize, unsigned char* outBuffer)
{
	unsigned char* inBuffer;
	inBuffer = (unsigned char*)malloc(arg_compressedSize);

	z_stream z;


	z.zalloc = Z_NULL;
	z.zfree = Z_NULL;
	z.opaque = Z_NULL;

	int res = inflateInit(&z);
	z.next_in = NULL;
	z.avail_in = 0;
	z.next_out = outBuffer;
	z.avail_out = uncompressedSize;

	fin.read((char*)inBuffer, arg_compressedSize);

	z.next_in = inBuffer;
	z.avail_in = arg_compressedSize;

	res = inflate(&z, Z_SYNC_FLUSH);


	inflateEnd(&z);

	free(inBuffer);
}


std::wstring ButiEngine::BinaryReader::ReadWCharactor(const UINT count)
{
	wchar_t* readChars = (wchar_t*)malloc(count* sizeof(wchar_t));

	fin.read((char*)readChars, count*sizeof(wchar_t));

	std::wstring out;

	for (UINT i = 0; i < count; i++) {
		if (!readChars[i]) {
			break;
		}
		out +=readChars[i];
	}
	free( readChars);
	return out;
}

std::wstring ButiEngine::BinaryReader::ReadShift_jis(const UINT count)
{
	char* readChars = (char*)malloc(count * sizeof(char));

	fin.read(readChars, count * sizeof(char));

	std::string out = std::string(readChars);
	
	
	free(readChars);
	return Util::StringToWString(out);
}

int ButiEngine::BinaryHelper::SwapByte(const int arg_int)
{
	return Swap32bit(arg_int);
}

UINT ButiEngine::BinaryHelper::SwapByte(const UINT arg_UINT)
{
	return Swap32bit(arg_UINT);
}

float ButiEngine::BinaryHelper::SwapByte(const float arg_float)
{
	float output;
	UINT reverseValue;
	memcpy(&reverseValue, &arg_float, sizeof(arg_float));
	reverseValue = Swap32bit(reverseValue);
	memcpy(&output, &reverseValue, sizeof(reverseValue));
	return output;
}

double ButiEngine::BinaryHelper::SwapByte(const double & arg_double)
{
	float output;
	ulongLong reverseValue;
	memcpy(&reverseValue, &arg_double, sizeof(arg_double));
	reverseValue = Swap64bit(reverseValue);
	memcpy(&output, &reverseValue, sizeof(reverseValue));
	return output;
}

short ButiEngine::BinaryHelper::Swap16bit(const short input)
{
	short output;
	char *conv = (char*)& input;
	char *ret = (char*)& output;


	ret[0] = conv[1];
	ret[1] = conv[0];
	return output;
}

int ButiEngine::BinaryHelper::Swap32bit(const int input)
{
	int output;
	char *conv = (char*)& input;
	char *ret = (char*)& output;


	ret[0] = conv[3];
	ret[1] = conv[2];
	ret[2] = conv[1];
	ret[3] = conv[0];
	return output;
}

ButiEngine::longLong ButiEngine::BinaryHelper::Swap64bit(const longLong & input)
{
	longLong output;
	char *conv = (char*)& input;
	char *ret = (char*)& output;


	ret[0] = conv[7];
	ret[1] = conv[6];
	ret[2] = conv[5];
	ret[3] = conv[4];
	ret[4] = conv[3];
	ret[5] = conv[2];
	ret[6] = conv[1];
	ret[7] = conv[0];
	return output;
}

ButiEngine::ushort ButiEngine::BinaryHelper::Swap16bit(const ushort & input)
{
	ushort output;
	char *conv = (char*)& input;
	char *ret = (char*)& output;


	ret[0] = conv[1];
	ret[1] = conv[0];
	return output;
}

UINT ButiEngine::BinaryHelper::Swap32bit(const UINT input)
{
	int output;
	char *conv = (char*)& input;
	char *ret = (char*)& output;


	ret[0] = conv[3];
	ret[1] = conv[2];
	ret[2] = conv[1];
	ret[3] = conv[0];
	return output;
}

ButiEngine::ulongLong ButiEngine::BinaryHelper::Swap64bit(const ulongLong & input)
{
	ulongLong output;
	char *conv = (char*)& input;
	char *ret = (char*)& output;


	ret[0] = conv[7];
	ret[1] = conv[6];
	ret[2] = conv[5];
	ret[3] = conv[4];
	ret[4] = conv[3];
	ret[5] = conv[2];
	ret[6] = conv[1];
	ret[7] = conv[0];
	return output;
}


bool ButiEngine::BinaryWriter::WriteStart(const std::string & filePath)
{
	fout = std::ofstream(filePath, std::ios::out | std::ios::binary);
	if (!fout) {
		std::cout << "ファイルが見つかりません\n";
		return false;
	}
	return true;
}

void ButiEngine::BinaryWriter::WriteEnd()
{
	fout.close();
}

void ButiEngine::BinaryWriter::WriteCharactor(const std::string & write)
{
	fout.write(write.c_str(), write.size());
}

void ButiEngine::BinaryWriter::WriteCharactor(const char* write, const UINT size)
{
	fout.write(write,size);
}

void ButiEngine::BinaryWriter::WriteWCharactor(const std::wstring & write)
{
	fout.write((char*)write.c_str(), write.size()*sizeof(wchar_t));
}
