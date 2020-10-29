#pragma once
#include"stdafx.h"
#include "..\..\Header\Resources\Resource_Motion.h"
#include"Header/Resources/ModelAnimation.h"


std::shared_ptr<ButiEngine::ModelAnimation> ButiEngine::Resource_Motion::GetAnimation()
{
	auto output = ObjectFactory::Create<ModelAnimation>();

	for (auto itr = map_boneKeyFrames.begin(); itr != map_boneKeyFrames.end(); itr++) {
		auto motionLane = ObjectFactory::Create<  BoneMotionTimeLine>();
		motionLane->SetMotionData(itr->second);

		output->AddMotionTimeLine(itr->first, motionLane);
	}
	return output;
}

void ButiEngine::Resource_Motion::AddKeyFrame(const std::wstring& arg_boneName, const MotionKeyFrameData& arg_data)
{
	if (!map_boneKeyFrames.count(arg_boneName)) {
		auto vec_boneName = std::vector<MotionKeyFrameData>();
		
		map_boneKeyFrames.emplace(arg_boneName, vec_boneName);
		map_boneKeyFrames.at(arg_boneName).push_back(arg_data);
		return;
	}

	map_boneKeyFrames.at(arg_boneName).push_back(arg_data);
}

void ButiEngine::Resource_Motion::AddKeyFrameLane(const std::wstring& arg_boneName, const std::vector<MotionKeyFrameData>& arg_datas)
{

	if (map_boneKeyFrames.count(arg_boneName)) {
		return;
	}

	map_boneKeyFrames.emplace(arg_boneName,arg_datas);
}
