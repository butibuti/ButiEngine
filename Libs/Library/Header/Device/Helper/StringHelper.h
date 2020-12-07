#pragma once
#include<string>
#include<vector>
class StringHelper
{
public:
	~StringHelper();
	static std::vector<std::string> Split(const std::string& source, const std::string& cuttingSouece);
	static std::string Cut(const std::string& source, const std::string& frontCuttingSouece,  const std::string& backCuttingSouece,bool isContaisCutSource=true);
	static std::string Remove(const std::string& source, const std::string& removeSouece);
	static std::string RemoveComment(const std::string& source);
	static bool Contains(const std::string& source, const std::string& findSource);
	static bool Contains(const std::wstring& source, const std::wstring& findSource);
	static bool Contains(const std::string& source, const char findSource);
	static void WStringToSafetyConvert(std::wstring& source);
	static std::string GetDirectory(const std::string& source);
	static std::string GetFileName(const std::string& source,const bool isContainExtension);
private:
	StringHelper();
};

