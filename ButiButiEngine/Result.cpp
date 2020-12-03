#pragma once
#include "stdafx.h"
#include "Result.h"
#include "..\..\Header\Common\CerealUtill.h"

std::shared_ptr<ButiEngine::Result> ButiEngine::Result::shp_instance = nullptr;

void ButiEngine::Result::CreateInstance()
{
    shp_instance = std::make_shared< Result>();

	if (!Util::CheckFileExistence("Resources/Application/GameResult.res")) {

		shp_instance->map_score.emplace("SampleScene", std::vector<int>());

		shp_instance->map_score.at("SampleScene").resize(5);

		shp_instance->map_score.emplace("Stage_1", std::vector<int>());
		shp_instance->map_score.emplace("Stage_2", std::vector<int>());
		shp_instance->map_score.emplace("Stage_3", std::vector<int>());

		shp_instance->map_score.at("Stage_1").resize(5);
		shp_instance->map_score.at("Stage_2").resize(5);
		shp_instance->map_score.at("Stage_3").resize(5);
		return;
	}

	std::stringstream stream;

	std::ifstream inputFile("Resources/Application/GameResult.res", std::ios::binary);

	stream << inputFile.rdbuf();

	cereal::BinaryInputArchive binInputArchive(stream);


	binInputArchive(shp_instance);
}

std::shared_ptr<ButiEngine::Result> ButiEngine::Result::GetInstance()
{
    return shp_instance;
}

void ButiEngine::Result::Output()
{
	std::stringstream stream;


	cereal::BinaryOutputArchive binOutArchive(stream);

	binOutArchive(shp_instance);

	std::ofstream outputFile("Resources/Application/GameResult.res", std::ios::binary);

	outputFile << stream.str();

	outputFile.close();
	stream.clear();
	shp_instance = nullptr;
}

int ButiEngine::Result::SetScore(const std::string& arg_stageName, const int score)
{
	if (!map_score.count(arg_stageName)) {
		return 0;
	}
	currentScore = score;
	map_score.at(arg_stageName).push_back(score);

	sort(map_score.at(arg_stageName).begin(), map_score.at(arg_stageName).end(), [](const int& x,const int& y) {
		return x > y;
	});
	map_score.at(arg_stageName).resize(5);

	int rank=1;

	for (int i = 0; i < map_score.at(arg_stageName).size(); i++,rank++) {
		if (map_score.at(arg_stageName).at(i) == score) {
			break;
		}
	}

    return rank;
}

int ButiEngine::Result::SetCurrentScore(const std::string& arg_stageName, const int score)
{
	if (!map_score.count(arg_stageName)) {
		return 0;
	}
	currentScore = score;

	return currentScore;
}

int ButiEngine::Result::GetCurrentScore()
{
	return currentScore;
}

std::vector<int> ButiEngine::Result::GetScores(const std::string& arg_stageName)
{
	return map_score.at(arg_stageName);
}

int ButiEngine::Result::GetMaxScore(const std::string& arg_stageName)
{
	return map_score.at(arg_stageName).at(0);
}

void ButiEngine::Result::SetReplayScene(const std::string& arg_currentReplayScene)
{
	currentReplayScene = arg_currentReplayScene;
}

const std::string& ButiEngine::Result::GetReplayScene()
{
	return currentReplayScene;
}
