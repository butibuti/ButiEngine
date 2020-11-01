#pragma once
#include "stdafx.h"
namespace ButiEngine {
	template <class T>
	class ID {
	public:
		ID() {}
		ID(UINT* arg_id) :id(arg_id) {}

		UINT* GetID()const {
			return id;
		}
		bool operator == (const ID& other) {
			return *other.id == *id;
		}
		bool IsEmpty() const {
			return id == nullptr;
		}
	private:
		UINT* id = nullptr;
	};

	template <class T>
	class IDContainer {
	public:
		void Clear() {
			map_values.clear();
			vec_p_resource.clear();
			for (auto itr = vec_p_id.begin(); itr != vec_p_id.end(); itr++) {
				delete* itr;
			}
			vec_p_id.clear();
		}
		ID<T> GetTag(const std::string& arg_key, const std::string& arg_directory = "")const {
			if (!map_values.count(arg_directory + arg_key)) {
				return map_values.begin()->second;
			}
			return map_values.at(arg_directory + arg_key);
		}
		std::shared_ptr<T> GetValue(const ID<T>& arg_tag) {
			if (arg_tag.IsEmpty()) {
				std::cout << "This tag is unregisted." << std::endl;
				return vec_p_resource.at(0);
			}
			if ((*arg_tag.GetID()) < vec_p_resource.size())
				return vec_p_resource.at((*arg_tag.GetID()));
			else {
				return nullptr;
			}
		}
		std::shared_ptr<T> GetValue(const std::string& arg_key) {
			auto tag = GetTag(arg_key);
			return vec_p_resource.at((*(tag.GetID())));
		}
		ID<T> AddValue(std::shared_ptr<T> arg_value, const std::string& arg_key, const std::string& arg_directory = "") {
			if (map_values.count(arg_directory + arg_key)) {
				std::cout << "This name is already registed." << std::endl;
				return map_values.at(arg_directory + arg_key);
			}
			vec_p_resource.push_back(arg_value);
			vec_p_id.push_back(new UINT((UINT)vec_p_resource.size() - 1));
			ID<T> output((vec_p_id.at(vec_p_id.size() - 1)));

			map_values.emplace(arg_directory + arg_key, output);
			return output;
		}
		bool ContainValue(const std::string& arg_key, const std::string& arg_directory = "") {
			if (map_values.count(arg_directory + arg_key)) {
				return true;
			}
			return false;
		}
		void Remove(const std::string& arg_key, const std::string& arg_directory = "") {
			if (!map_values.count(arg_directory + arg_key)) {
				return;
			}
			auto index = *(map_values.at(arg_directory + arg_key).GetID());
			vec_p_resource.erase(vec_p_resource.begin() + index);
			map_values.erase(arg_directory + arg_key);
			delete vec_p_id.at(index);
			vec_p_id.erase(vec_p_id.begin() + index);
			for (int i = index; i < vec_p_id.size(); i++) {
				(*vec_p_id.at(i)) = i;
			}
		}
		void Remove(ID<T> arg_id) {
			auto index = *arg_id.GetID();
			auto mapItr = map_values.begin();
			for (auto i = 0; i < index; i++)
				mapItr++;
			map_values.erase(mapItr->first);
			vec_p_resource.erase(vec_p_resource.begin() + index);
			delete vec_p_id.at(index);
			vec_p_id.erase(vec_p_id.begin() + index);
			for (int i = index; i < vec_p_id.size(); i++) {
				(*vec_p_id.at(i)) = i;
			}
		}
	private:
		std::map<std::string, ID<T>> map_values;
		std::vector< std::shared_ptr<T>> vec_p_resource;
		std::vector<UINT*>vec_p_id;

	};
	template <class T>
		class IDManager {
		public:
			~IDManager(){
				Clear();
			}
			void Clear() {
				map_values.clear();
				for (auto itr = vec_p_id.begin(); itr != vec_p_id.end(); itr++) {
					delete* itr;
				}
				vec_p_id.clear();
			}
			ID<T> GetTag(const std::string& arg_key, const std::string& arg_directory = "") {
				if (!map_values.count(arg_directory + arg_key)) {
					return AddTag(arg_directory + arg_key);
				}
				return map_values.at(arg_directory + arg_key);
			}
			ID<T> AddTag( const std::string& arg_key, const std::string& arg_directory = "") {
				if (map_values.count(arg_directory + arg_key)) {
					return map_values.at(arg_directory + arg_key);
				}
				auto p_id = new unsigned int(vec_p_id.size());
				vec_p_id.push_back(p_id);
				ID<T> output(p_id);

				map_values.emplace(arg_directory + arg_key, output);
				return output;
			}
			std::string GetIDName(ID<T> arg_id) {
				if (arg_id.IsEmpty()) {
					return "none";
				}
				auto index = *arg_id.GetID();
				auto mapItr = map_values.begin();
				for (auto i = 0; i < index; i++)
					mapItr++;
				return mapItr->first;
			}
			void Remove(const std::string& arg_key, const std::string& arg_directory = "") {
				if (!map_values.count(arg_directory + arg_key)) {
					return;
				}
				auto index = *(map_values.at(arg_directory + arg_key).GetID());
				map_values.erase(arg_directory + arg_key);
				delete vec_p_id.at(index);
				vec_p_id.erase(vec_p_id.begin() + index);
				for (int i = index; i < vec_p_id.size(); i++) {
					(*vec_p_id.at(i)) = i;
				}
			}
			void Remove(ID<T> arg_id) {
				auto index = *arg_id.GetID();
				auto mapItr = map_values.begin();
				for (auto i = 0; i < index; i++)
					mapItr++;
				map_values.erase(mapItr->first);
				delete vec_p_id.at(index);
				vec_p_id.erase(vec_p_id.begin() + index);
				for (int i = index; i < vec_p_id.size(); i++) {
					(*vec_p_id.at(i)) = i;
				}
			}
		private:
			std::map<std::string, ID<T>> map_values;
			std::vector<UINT*>vec_p_id;

	};
}