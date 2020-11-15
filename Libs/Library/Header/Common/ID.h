#pragma once
#include "stdafx.h"
namespace ButiEngine {

	template <class T>
	class ID {
	public:
		ID() {}
		ID(std::shared_ptr<UINT> arg_id) { id = (arg_id); }

		UINT* GetID()const {
			return id.get();
		}
		bool operator == (const ID& other) {
			return *other.id == *id;
		}
		bool operator != (const ID& other) {
			return *other.id != *id;
		}
		bool IsEmpty() const {
			return id == nullptr;
		}
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(id);
		}
	private:
		std::shared_ptr<UINT> id = nullptr;
	};

	template <class T>
	class IDContainer {
	public:
		void Clear() {
			map_values.clear();
			vec_p_resource.clear();
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
		std::string GetIDName(ID<T> arg_id) {
			if (arg_id.IsEmpty()) {
				return "none";
			}
			auto endItr = map_values.end();
			for (auto mapItr = map_values.begin(); mapItr!=endItr ; mapItr++) {
				if(mapItr->second==arg_id)
				return mapItr->first;
			}
			return "none";
		}
		ID<T> AddValue(std::shared_ptr<T> arg_value, const std::string& arg_key, const std::string& arg_directory = "") {
			if (map_values.count(arg_directory + arg_key)) {
				auto index = *(map_values.at(arg_directory + arg_key).GetID());
				if (index >= vec_p_resource.size()) {
					vec_p_resource.push_back(arg_value);
				}
				return map_values.at(arg_directory + arg_key);
			}
			vec_p_resource.push_back(arg_value);
			vec_p_id.push_back(std::make_shared<UINT> ((UINT)vec_p_resource.size() - 1));
			ID<T> output((vec_p_id.at(vec_p_id.size() - 1)));

			map_values.emplace(arg_directory + arg_key, output);
			return output;
		}

		void RecoverValue(std::shared_ptr<T> arg_value) {
			vec_p_resource.push_back(arg_value);
		}

		bool ContainValue(const std::string& arg_key, const std::string& arg_directory = "") {
			if (map_values.count(arg_directory + arg_key)) {

				auto index = *(map_values.at(arg_directory + arg_key).GetID());
				if (index >= vec_p_resource.size()) {
					return false;
				}else

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

			vec_p_id.erase(vec_p_id.begin() + index);
			for (int i = index; i < vec_p_id.size(); i++) {
				(*vec_p_id.at(i)) = i;
			}
		}

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(map_values);
			archive(vec_p_id);
		}

		ID<T> ShowGUI(ImGuiIO& arg_io) {
			auto enditr = map_values.end();
			ID<T> out;
			for (auto itr = map_values.begin(); itr != enditr; itr++) {
				if (ImGui::Button(Util::ToUTF8( itr->first).c_str())) {
					out = (itr->second);
				}

				if (ImGui::IsItemActive()) {
					ImGui::GetForegroundDrawList()->AddLine(arg_io.MouseClickedPos[0], arg_io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f); // Draw a line between the button and the mouse cursor
					out = (itr->second);
				}
			}
			return out;
		}

		const std::vector< std::shared_ptr<T>>& GetResources() {
			return vec_p_resource;
		}

	private:
		std::unordered_map<std::string, ID<T>> map_values;
		std::vector< std::shared_ptr<T>> vec_p_resource;
		std::vector<std::shared_ptr< UINT>>vec_p_id;

	};
	template <class T>
		class IDManager {
		public:
			~IDManager(){
				Clear();
			}
			void Clear() {
				map_values.clear();
				
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
				auto p_id = std::make_shared<UINT>(vec_p_id.size());
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
				vec_p_id.erase(vec_p_id.begin() + index);
				for (int i = index; i < vec_p_id.size(); i++) {
					(*vec_p_id.at(i)) = i;
				}
			}
			template<class Archive>
			void serialize(Archive& archive)
			{
				archive(map_values);
				archive(vec_p_id);
			}
		private:
			std::unordered_map<std::string, ID<T>> map_values;
			std::vector<std::shared_ptr< UINT>>vec_p_id;

	};
}