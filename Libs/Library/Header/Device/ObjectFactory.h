#pragma once
#include"stdafx.h"
namespace ButiEngine {
	class ObjectFactory {
	public:
		template<typename T, typename... Ts>
		static inline std::shared_ptr<T> Create(Ts&&... params) {
			std::shared_ptr<T> Ptr = std::shared_ptr<T>(new T(params...));
			//仮想関数呼び出し
			Ptr->PreInitialize();
			Ptr->Initialize();
			Ptr->SetCreated(true);
			return Ptr;
		}; 
		template<typename T, typename... Ts>
			static inline std::shared_ptr<T> CreateCopy(T value) {
			std::shared_ptr<T> Ptr = std::make_shared<T>(value);
			return Ptr;
		};

		template<typename T, typename... Ts>
		static inline std::shared_ptr<T> CreateFromCereal(const std::string& arg_filePath) {
			std::shared_ptr<T> Ptr = std::shared_ptr<T>();
			if(Util::CheckFileExistence(arg_filePath))
				InputCereal(Ptr, arg_filePath);
			else {
				Ptr = std::make_shared<T>();
			}
			//仮想関数呼び出し
			Ptr->PreInitialize();
			Ptr->Initialize();
			Ptr->SetCreated(true);
			return Ptr;
		}; 

	};

	class IObject :public std::enable_shared_from_this<IObject>
	{
		friend class ObjectFactory;
		bool isCreated = false;
		void SetCreated(bool arg_created) {
			isCreated = arg_created;
		};
	protected:
		IObject() {}
		virtual ~IObject() {}

	public:
		template<typename T>
		inline std::shared_ptr<T> GetThis() {
			auto Ptr = std::dynamic_pointer_cast<T>(shared_from_this());
			if (Ptr) {
				return Ptr;
			}
			else {
				std::wstring str(L"Can't cast this to");
				str += Util::GetWStringTypeName<T>();
				str += L" type.";
				throw ButiException(
					str,
					L"if( ! dynamic_pointer_cast<T>(shared_from_this()) )",
					L"IObject::GetThis()"
				);
			}
			return nullptr;

		}
		template<typename T>
			inline T* GetThis_ptr() {
				auto Ptr = dynamic_cast<T*>(this);
			if (Ptr) {
				return Ptr;
			}
			else {
				std::wstring str(L"Can't cast this to");
				str += Util::GetWStringTypeName<T>();
				str += L" type.";
				throw ButiException(
					str,
					L"if( ! dynamic_cast<T>(this) )",
					L"IObject::GetThis()"
				);
			}
			return nullptr;

		}
		template<typename T>
		inline bool IsThis() {
			auto Ptr = std::dynamic_pointer_cast<T>(shared_from_this());
			if (Ptr) {
				return true;
			}
			else {
				return false;
			}
			//return nullptr;

		}
		template<typename T>
		inline bool IsThis_ptr() {
			auto Ptr =dynamic_cast<T*>( this);
			if (Ptr) {
				return true;
			}
			else {
				return false;
			}
			//return nullptr;

		}
		bool IsCreated() { return isCreated; }
		virtual void PreInitialize() = 0;
		virtual void Initialize() = 0;

	};
}