#include"stdafx.h"

namespace ButiEngine {	
	template <class T>
		class Container {
		public:

			std::shared_ptr<T> GetValue(const std::string& arg_key,const std::string& arg_directory = "")const {
				if (!map_values.count(arg_directory + arg_key)) {
					return map_values.begin()->second;
				}
				return map_values.at(arg_directory + arg_key);
			}
			std::shared_ptr<T> AddValue(std::shared_ptr<T> arg_value,const std::string& arg_key,const std::string& arg_directory = "") {
				if (map_values.count(arg_directory + arg_key)) {
					std::cout<<"This name is already registed."<<std::endl;
					return map_values.at(arg_directory + arg_key);
				}
				map_values.emplace(arg_directory + arg_key, arg_value);
				return arg_value;
			}
			void Remove(const std::string& arg_key, const std::string& arg_directory = "") {
				map_values.erase(arg_directory + arg_key);
			}
		private:
			std::map<std::string, std::shared_ptr<T>> map_values;
	};
}