#include"stdafx.h"
#include "..\..\..\Header\Device\Helper\StringHelper.h"

StringHelper::StringHelper()
{
}


StringHelper::~StringHelper()
{
}

std::vector<std::string> StringHelper::Split(const std::string& source, const std::string& cuttingSouece)
{
	auto output = std::vector<std::string>();
	int first = 0;
	int last = source.find_first_of(cuttingSouece);
	if (last == std::string::npos)
		return output;
	while (first < source.size())
	{
		auto subString = source.substr(first, last - first);
		output.push_back(subString);
		first = last + 1;
		last = source.find_first_of(cuttingSouece, first);
		if (last == std::string::npos) {
			last = source.size();
		}
	}
	return output;
}

std::string StringHelper::Cut(const std::string& source, const std::string& frontCuttingSource, const std::string& backCuttingSouece, bool isContaisCutSource)
{
	auto pos = source.find(frontCuttingSource);
	auto endPos = source.find(backCuttingSouece, pos + frontCuttingSource.length());
	if (pos == std::string::npos || endPos == std::string::npos) {
		return "";
	}
	if (isContaisCutSource) {
		return source.substr(pos, endPos - pos + backCuttingSouece.size());
	}
	else
	{
		return source.substr(pos + frontCuttingSource.size(), endPos - pos - frontCuttingSource.size());
	}
}

std::string StringHelper::Remove(const std::string& source, const std::string& removeSouece)
{
	std::string output = source;
	size_t pos = output.find(removeSouece);
	while (pos != std::string::npos)
	{
		output.erase(pos, removeSouece.size());
		pos = output.find(removeSouece);
	}
	return output;
}

std::string StringHelper::RemoveComment(const std::string& source)
{
	std::string output = source;
	auto commentStartPos = output.find("/*");
	auto commentEndPos = output.find("*/");
	while (commentStartPos != std::string::npos && commentEndPos != std::string::npos) {
		output.erase(commentStartPos, commentEndPos - commentStartPos + 2);
		commentStartPos = output.find("/*");
		commentEndPos = output.find("*/");
	}
	commentStartPos = output.find("//");
	while (commentStartPos != std::string::npos) {
		commentEndPos = output.find("\n", commentStartPos);
		if (commentEndPos != std::string::npos) {
			output.erase(commentStartPos, commentEndPos - commentStartPos);
		}
		else
		{
			output.erase(commentStartPos, output.size() - commentStartPos);
		}
		commentStartPos = output.find("//");
	}
	return output;
}

bool StringHelper::Contains(const std::string& source, const std::string& findSource)
{
	if (source.find(findSource) != std::string::npos)
		return true;
	else
		return false;
}

bool StringHelper::Contains(const std::wstring& source, const std::wstring& findSource)
{
	if (source.find(findSource) != std::string::npos)
		return true;
	else
		return false;
}

bool StringHelper::Contains(const std::string& source, const char findSource)
{
	if (source.find(findSource) != std::string::npos)
		return true;
	else
		return false;
}

void StringHelper::WStringToSafetyConvert(std::wstring& source)
{
	std::wstring backSlash = L"\\";
	auto backSlashPos = source.find(backSlash);
	while (backSlashPos != std::wstring::npos)
	{
		source.erase(backSlashPos, backSlash.size());
		source.insert(backSlashPos, L"/");
		backSlashPos = source.find(backSlash);
	}
}

std::string StringHelper::GetDirectory(const std::string& source)
{
	if (!Contains(source, "/")) {
		return source;
	}
	auto splited = Split(source, "/");

	std::string out;

	for (auto itr = splited.begin(); itr != splited.end() - 1; itr++) {
		out += (*itr) + "/";
	}

	return out;
}

std::string StringHelper::GetFileName(const std::string& source, const bool isContainExtension)
{
	if (!Contains(source, "/")) {
		return source;
	}
	auto splited = Split(source, "/");

	std::string out = *splited.rbegin();

	if (isContainExtension)
		return out;
	if (!Contains(out, ".")) {
		return out;
	}
	out = Split(out, ".").at(0);

	return out;
}

