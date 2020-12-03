#pragma once
#include"stdafx.h"
namespace  ButiEngine {

	class Result
	{
	public:
		static void CreateInstance();
		static std::shared_ptr<Result> GetInstance();
		static void Output();
		int SetScore(const std::string& arg_stageName, const int score);
		int SetCurrentScore(const std::string& arg_stageName,const int score);
		int GetCurrentScore();
		std::vector<int> GetScores(const std::string& arg_stageName);
		int GetMaxScore(const std::string& arg_stageName);
		void SetReplayScene(const std::string& currentReplayScene);
		const std::string& GetReplayScene();
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(map_score);
		}
		Result() {}
	private:
		static std::shared_ptr<Result> shp_instance;
		std::map<std::string, std::vector<int>>map_score;
		std::string currentReplayScene = "SampleScene";
		int currentScore = 0;
	};

}
