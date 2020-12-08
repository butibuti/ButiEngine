#pragma once
#include<string>
#include<ctime>
#include<random>
#include<memory>
namespace ButiEngine {
	namespace BitFlag {
		const unsigned int FLAG_MAX = 16;
		const unsigned int BIT_FLAG[]{ (1 << 0),(1 << 1),(1 << 2),(1 << 3),(1 << 4),(1 << 5),(1 << 6),(1 << 7),(1 << 8),(1 << 9),(1 << 10),(1 << 11),(1 << 12),(1 << 13),(1 << 14),(1 << 15) };
	}
	class IApplication;
	class Util
	{
	public:

		static inline bool GetBitFlag(const int arg_flag, const int arg_bitIndex) {
			if (arg_bitIndex >= 16 || arg_bitIndex < 0) {
				return false;
			}
			return arg_flag & BitFlag::BIT_FLAG[arg_bitIndex];
		}
		static inline char GetBitNum(const char arg_byte, const int arg_readRange) {

			char out = arg_byte;
			out <<= (8 - arg_readRange);
			out >>= (8 - arg_readRange);


			return std::abs(out);
		}
		static inline char GetBitNum(const char arg_byte, const int arg_readRange, const int arg_offset) {

			char out = arg_byte;
			out >>= (arg_offset);
			out <<= (8 - arg_readRange);
			out >>= (8 - arg_readRange);


			return std::abs(out);
		}
		static void WStringtoMultiByte(const std::wstring & src, std::string& dest);
		static std::string WStringToString(std::wstring oWString);
		static std::wstring StringToWString(std::string oString);
		static void MultiBytetoWString(const std::string& src, std::wstring & dest) {
			size_t i;
			wchar_t* WCstr = new wchar_t[src.length() + 1];
			mbstowcs_s(
				&i,
				WCstr,
				src.length() + 1,
				src.c_str(),
				_TRUNCATE //すべて変換できなかったら切り捨て
			);
			dest = WCstr;
			delete[] WCstr;
		}

		static	std::string ToUTF8(std::string srcSjis) {
			//Unicodeへ変換後の文字列長を得る
			int lenghtUnicode = MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, NULL, 0);

			//必要な分だけUnicode文字列のバッファを確保
			wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

			//ShiftJISからUnicodeへ変換
			MultiByteToWideChar(CP_THREAD_ACP, 0, srcSjis.c_str(), srcSjis.size() + 1, bufUnicode, lenghtUnicode);


			//UTF8へ変換後の文字列長を得る
			int lengthUTF8 = WideCharToMultiByte(CP_UTF8, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

			//必要な分だけUTF8文字列のバッファを確保
			char* bufUTF8 = new char[lengthUTF8];

			//UnicodeからUTF8へ変換
			WideCharToMultiByte(CP_UTF8, 0, bufUnicode, lenghtUnicode + 1, bufUTF8, lengthUTF8, NULL, NULL);

			std::string strUTF8(bufUTF8);

			delete bufUnicode;
			delete bufUTF8;

			return strUTF8;
		}

		static bool CheckFileExistence(const std::string& arg_filePath) {
			std::ifstream checkedFile(arg_filePath);

			return checkedFile.is_open();

		}
		template<typename T>
		static std::wstring GetWStringTypeName() {
			std::wstring outputClassName;
			MultiBytetoWString(typeid(T).name(), outputClassName);
			return outputClassName;
		}
		template<typename T>
			static std::string GetStringTypeName() {
			return typeid(T).name();
		}

		static bool IsFileExistence(const std::string& arg_filePath);
	};
	const unsigned int LEVEL_FLAG[]{ (111 << 0),(111 << 3),(111 << 6),(111 << 9),(111 << 12),(111 << 15),(111 << 18),(111 << 21),(111<< 24),(111 << 27), };
	class OctreeHelper {
	public:
		static inline DWORD BitSeparate(BYTE n)
		{
			DWORD s = n;
			s = (s | s << 8) & 0x0000f00f;
			s = (s | s << 4) & 0x000c30c3;
			s = (s | s << 2) & 0x00249249;
			return s;
		}
		static inline char GetLevel(const unsigned int& arg_flag,const unsigned char  arg_level) {
			char out = 1;
			for (char i=0 ;i < arg_level; i++) {

				DWORD Check = (arg_flag >> (i * 3)) & 0x7;
				if (Check != 0)
					out = i + 1;
			}
			return out;
		}
	};

	class ButiException :public std::exception {

		std::wstring wstr_errorMessage;
	public:
		ButiException(std::wstring const message1, std::wstring const message2, std::wstring const message3) {
			wstr_errorMessage = message1;
			wstr_errorMessage += L"\n" + message2 + L"\n" + message3;
		}
		const std::wstring & what_w() const throw() {
			return wstr_errorMessage;
		}
	};
	namespace ButiTime {
		inline timespec *timespecSubstruction(const struct timespec *A, const struct timespec *B, struct timespec *C)
		{
			C->tv_sec = A->tv_sec - B->tv_sec;
			C->tv_nsec = A->tv_nsec - B->tv_nsec;
			if (C->tv_nsec < 0) {
				C->tv_sec--;
				C->tv_nsec += 1000000000;
			}

			return C;
		}
	}

	void ThrowButiException_Runtime(const std::wstring & message1, const std::wstring & message2, const std::wstring & message3);
	//throw runtime_error function
	void ThrowButiException_Runtime(const std::string& meesage1, const std::string& message2, const std::string& message3);

	template<typename T>
	std::shared_ptr<void> SharedPtrToVoid(const std::shared_ptr<T>& SrcPtr);
	template<typename T>
	std::shared_ptr<T> VoidToShared(const std::shared_ptr<void>& SrcPtr);

	class ButiRandom {
	public:

		static void Initialize();


		template<class T>
		inline static T GetRandom(T min, T max, int pase) {
			if (min == max) {
				return min;
			}
			if (min > max) {
				auto b = min;
				min = max;
				max = min;
			}

			shp_randRange = std::make_shared< std::uniform_int_distribution<>>(min*pase, max*pase);
			return (T)(*shp_randRange)(*shp_mt) / pase;

		};




	private:
		static std::shared_ptr< std::random_device> shp_rnd_device;// = std::make_shared<std::random_device>();
		static std::shared_ptr<std::mt19937>shp_mt;// = std::make_shared<std::mt19937>((*shp_rnd_device)());
		static std::shared_ptr< std::uniform_int_distribution<>> shp_randRange;// = std::make_shared< std::uniform_int_distribution<>>(0, 1);

		ButiRandom() {};
		~ButiRandom() {};
	};
	template<typename T>
	std::shared_ptr<void> SharedPtrToVoid(const std::shared_ptr<T>& SrcPtr)
	{
		std::shared_ptr<void> RetPtr = std::static_pointer_cast<void>(SrcPtr);
		return RetPtr;
	}
	template<typename T>
	std::shared_ptr<T> VoidToShared(const std::shared_ptr<void>& SrcPtr)
	{
		std::shared_ptr<T> RetPtr = std::static_pointer_cast<T>(SrcPtr);
		return RetPtr;
	}

	enum class SamplerState {
		SamplerNone,
		PointWrap,
		PointClamp,
		LinearWrap,
		LinearClamp,
		AnisotropicWrap,
		AnisotropicClamp,
		ComparisonLinear,
	};
	template<typename T>
	void hash_combine(size_t& seed, T const& v) {

		std::hash<T> primitive_type_hash;


		seed ^= primitive_type_hash(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	///Cereal Regist
	/// 
	///

}


