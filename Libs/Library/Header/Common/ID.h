#pragma once
#include<memory>
#include<map>
#include<vector>
namespace ButiEngine {
	static const std::string noneIDStr = "none";
	template <class T>
	class ID {
	public:
		ID() {}
		ID(std::shared_ptr<std::string> arg_id) { id = (arg_id); }
		ID(std::string arg_str) { id = std::make_shared<std::string>(arg_str); }

		const std::string& GetID()const {
			if(id)
				return *id;
			else {
				return noneIDStr;
			}
		}
		bool operator == (const ID& other) {
			if (other.IsEmpty()||IsEmpty()) {
				return false;
			}
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
		std::shared_ptr<std::string> id = nullptr;
	};

	template <class T>
	class IDContainer {
	public:
		void Clear() {
			map_shp_resource.clear();
		}
		ID<T> GetTag(const std::string& arg_key)const {
			return ID<T>(std::make_shared<std::string>(arg_key));
		}
		std::shared_ptr<T> GetValue(const ID<T>& arg_tag) {
			if (arg_tag.IsEmpty()) {
				std::cout <<arg_tag.GetID() <<": This tag is unregisted." << std::endl;
				return FailedValue();
			}
			if (map_shp_resource.count(arg_tag.GetID()))
			{
				return map_shp_resource.at(arg_tag.GetID());
			}			
			else
			{
				return FailedValue();
			}
		}
		std::shared_ptr<T> FailedValue() {

			if (map_shp_resource.size()) {
				return map_shp_resource.begin()->second;
			}
			else {
				std::cout << "ResourceContainer is Empty." << std::endl;
				return nullptr;
			}

		}
		std::shared_ptr<T> GetValue(const std::string& arg_key) {

			return map_shp_resource.at(arg_key);
		}
		std::string GetIDName(ID<T> arg_id) {
			if (arg_id.IsEmpty()) {
				return "none";
			}
			return arg_id.GetID();
		}
		ID<T> AddValue(std::shared_ptr<T> arg_value, const std::string& arg_key) {
			ID<T> output(std::make_shared<std::string>(arg_key));
			if (map_shp_resource.count( arg_key)) {
				return output;
			}
			map_shp_resource.emplace( arg_key, arg_value);
			return output;
		}

		bool ContainValue(const std::string& arg_key) {
			if (map_shp_resource.count( arg_key)) {

				return true;
			}
			return false;
		}
		void Remove(const std::string& arg_key) {
			if (!map_shp_resource.count( arg_key)) {
				return;
			}
			map_shp_resource.erase(arg_key);
		}
		void Remove(ID<T> arg_id) {
			Remove(arg_id.GetID());
		}

		template<class Archive>
		void serialize(Archive& archive)
		{
		}

		ID<T> ShowGUI(GUI::GuiIO& arg_io) {

			if (GUI::InputTextWithHint("SerchStr", "serch", serchStrAry, 256)) {
				serchStr = serchStrAry;
			}

			auto enditr = map_shp_resource.end();
			ID<T> out;
			if (serchStr.size() <= 0) {

				for (auto itr = map_shp_resource.begin(); itr != enditr; itr++) {
					if (GUI::Button(Util::ToUTF8(itr->first).c_str())) {
						out = ID<T>(itr->first);
					}

					if (GUI::IsItemActive()) {
						auto p1 = arg_io.MouseClickedPos[0];
						auto p2 = arg_io.MousePos;
						GUI::Line(Vector2(p1.x, p1.y), Vector2(p2.x, p2.y), GUI::GetColorU32(GUI::GuiCol_::GuiCol_Button), 4.0f); // Draw a line between the button and the mouse cursor

						out = ID<T>(itr->first);
					}
				}
			}
			else {
				for (auto itr = map_shp_resource.begin(); itr != enditr; itr++) {
					if (!StringHelper::Contains(itr->first, serchStr)) {
						continue;
					}
					if (GUI::Button(Util::ToUTF8(itr->first).c_str())) {
						out = ID<T>(itr->first);
					}

					if (GUI::IsItemActive()) {
						auto p1 = arg_io.MouseClickedPos[0];
						auto p2 = arg_io.MousePos;
						GUI::Line(Vector2(p1.x, p1.y), Vector2(p2.x, p2.y), GUI::GetColorU32(GUI::GuiCol_::GuiCol_Button), 4.0f); // Draw a line between the button and the mouse cursor

						out = ID<T>(itr->first);
					}
				}
			}
			return out;
		}

		std::vector< std::shared_ptr<T>> GetResources() {
			std::vector< std::shared_ptr<T>> output;
			output.reserve(map_shp_resource.size());
			auto end = map_shp_resource.end();
			for (auto itr = map_shp_resource.begin(); itr != end; itr++) {
				output.push_back(itr->second);
			}

			return output;
		}

	private:
		std::map< std::string,std::shared_ptr<T>> map_shp_resource;
		std::string serchStr;
		char serchStrAry[256];

	};
	template <class T>
		class IDManager {
		public:
			~IDManager(){
				Clear();
			}
			void Clear() {
				map_values.clear();
			}
			ID<T> GetTag(const std::string& arg_key) {
				if (!map_values.count( arg_key)) {
					return AddTag( arg_key);
				}
				return map_values.at( arg_key);
			}
			ID<T> AddTag( const std::string& arg_key) {
				if (map_values.count( arg_key)) {
					return map_values.at(  arg_key);
				}
				ID<T> output(arg_key);

				map_values.emplace(arg_key,output);
				return output;
			}
			std::string GetIDName(ID<T> arg_id) {
				if (arg_id.IsEmpty()) {
					return "none";
				}
				return arg_id.GetID();
			}
			void Remove(const std::string& arg_key) {
				if (!map_values.count( arg_key)) {
					return;
				}
				map_values.erase( arg_key);
			}
			void Remove(ID<T> arg_id) {
				if (!map_values.count(arg_id.GetID())) {
					return;
				}
				map_values.erase(arg_id.GetID());
			}
			template<class Archive>
			void serialize(Archive& archive)
			{
				archive(map_values);
			}
		private:
			std::unordered_map<std::string, ID<T>> map_values;

	};
}