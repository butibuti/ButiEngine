#include "stdafx.h"
#include"Header/Resources/ModelAnimation.h"
#include "..\..\Header\Resources\ModelAnimation.h"
#include "..\..\Header\Resources\DrawData\IDrawData.h"

ButiEngine::BoneMotionTimeLine::BoneMotionTimeLine()
{
	auto itr = vec_data.begin();
}

ButiEngine::BoneMotionTimeLine::~BoneMotionTimeLine()
{
}

void ButiEngine::BoneMotionTimeLine::SetBone(std::shared_ptr<Bone> arg_shp_bone)
{
	bone = arg_shp_bone;
}

void ButiEngine::BoneMotionTimeLine::PreStart()
{
	nowMotionItr = vec_data.begin();
	befMotionItr = vec_data.begin();
	isActive = true;
	Start();
}

void ButiEngine::BoneMotionTimeLine::Reset()
{
	PreStart();
}


void ButiEngine::BoneMotionTimeLine::SetMotionData(const MotionKeyFrameData& arg_motionData)
{
	vec_data.push_back(arg_motionData);
}

void ButiEngine::BoneMotionTimeLine::SetMotionData(const std::vector<MotionKeyFrameData>& arg_motionDatas)
{
	vec_data = arg_motionDatas;
}

void ButiEngine::BoneMotionTimeLine::Start()
{
	if (!bone) {
		isActive = false;
		return;
	}
	initPosition= bone->transform->GetLocalPosition();

	initRotate = bone->transform->GetLocalRotation().ToQuat();
}

void ButiEngine::BoneMotionTimeLine::FrameSet(const UINT frame)
{
	if (!isActive) {
		return;
	}

	if (frame >= nowMotionItr-> endFrame) {

		befMotionItr = nowMotionItr;
		nowMotionItr++;
		if (nowMotionItr == vec_data.end()) {
			isActive = false;
			return;
		}
		else {
			Start();

		}
	}
	auto relativeFrame = frame - befMotionItr-> endFrame;
	auto frameRange = nowMotionItr->endFrame - befMotionItr->endFrame;
	if (frameRange) {
		auto t = (float)relativeFrame / (float)frameRange;

		auto rotateT = nowMotionItr->larp.rotationBezier.GetYFromNuton(t);
		
		//std::cout << t << std::endl;

		auto nowRotation = Calculator::LearpQuat(initRotate, nowMotionItr->pose.rotation, rotateT);

		bone->transform->SetLocalRotation(Matrix4x4((nowRotation)));
		

		auto xt = nowMotionItr->larp.translationXBezier.GetYFromNuton(t);
		auto yt = nowMotionItr->larp.translationYBezier.GetYFromNuton(t);
		auto zt = nowMotionItr->larp.translationZBezier.GetYFromNuton(t);

		bone->transform->SetLocalPosition(Calculator::LarpPosition(initPosition, nowMotionItr->pose.position,xt,yt,zt));
	}
}

UINT ButiEngine::BoneMotionTimeLine::GetEndFrame()
{
	return vec_data.rbegin()->endFrame;
}

void ButiEngine::BoneMotionTimeLine::LocalPoseSet(std::shared_ptr<Transform> arg_parentBone)
{
	for (auto itr = vec_data.begin(); itr != vec_data.end(); itr++) {
		itr->pose.position += bone->transform->GetLocalPosition();
	}
}

void ButiEngine::ModelAnimation::Motion()
{
	if (frame >= endFrame) {
		if (isRoop) {
			frame = 1;
			Reset();
			int i = 0;
		}
		else {
			return;
		}
	}

	for (auto itr = vec_timeLines.begin(); itr != vec_timeLines.end(); itr++) {
		(*itr)->FrameSet(frame);
	}
	std::cout << frame << std::endl;
	//shp_boneDrawObj->InverseKinematic();
	//shp_boneDrawObj->BonePowerAdd();
	frame++;
}

void ButiEngine::ModelAnimation::IKTest()
{
	shp_boneDrawObj->InverseKinematic();
	shp_boneDrawObj->BonePowerAdd();
}


void ButiEngine::ModelAnimation::Reset()
{
	for (auto itr = vec_timeLines.begin(); itr != vec_timeLines.end(); itr++) {
		(*itr)->Reset();
	}
}

void ButiEngine::ModelAnimation::PreMotionStart(std::shared_ptr<Transform> arg_parentBoneTransform)
{
	for (auto itr = map_motionTimeLine.begin(); itr != map_motionTimeLine.end();itr++ ) {
		auto bone = shp_boneDrawObj->SerchBoneByName(itr->first);
		if (!bone) {
			continue;
		}
		(itr->second)->SetBone(bone);
		(itr->second)->LocalPoseSet(arg_parentBoneTransform);
		(itr->second)->PreStart();
		//(itr->second)->Start();
		endFrame = max((itr->second)->GetEndFrame(), endFrame);
	}
}

std::shared_ptr<ButiEngine::IMotionTimeLine> ButiEngine::ModelAnimation::AddMotionTimeLine(const std::wstring& arg_name, std::shared_ptr<IMotionTimeLine> arg_motion)
{
	map_motionTimeLine.emplace(arg_name, arg_motion);

	vec_timeLines.push_back(arg_motion);

	return arg_motion;
}

bool ButiEngine::ModelAnimation::SetLoop(const bool arg_isloop)
{
	isRoop = arg_isloop;
	return isRoop;
}

void ButiEngine::ModelAnimation::SetBoneDrawObj(std::shared_ptr<IBoneDrawObject> arg_shp_boneDrawObj)
{
	shp_boneDrawObj = arg_shp_boneDrawObj;
}

ButiEngine::Pose::Pose(const Vector3& arg_position, const Vector3& arg_rotate):position(arg_position)
{
	Matrix4x4 rotationMatrix = DirectX::XMMatrixRotationX(
		DirectX::XMConvertToRadians(arg_rotate.x)
	) *
		DirectX::XMMatrixRotationY(
			DirectX::XMConvertToRadians(arg_rotate.y)
		) *
		DirectX::XMMatrixRotationZ(
			DirectX::XMConvertToRadians(arg_rotate.z)
		);

	rotation = rotationMatrix.ToQuat();
}

ButiEngine::MotionKeyFrameData::MotionKeyFrameData(const UINT arg_endFrame, const Vector3& arg_position, const Vector3& arg_rotation)
	:MotionKeyFrameData(arg_endFrame,Pose(arg_position,arg_rotation))
{
}

ButiEngine::MotionKeyFrameData::MotionKeyFrameData(const UINT arg_endFrame, const Pose& arg_pose)
{
	endFrame = arg_endFrame;
	pose = arg_pose;
}

ButiEngine::LarpData::LarpData(BYTE* arg_data)
	: rotationBezier(Vector2(0,0), Vector2(arg_data[3]/127, arg_data[7]/127), Vector2(arg_data[11]/127, arg_data[15]/127), Vector2(1,1))
	, translationXBezier(Vector2(0, 0), Vector2(arg_data[0]/127, arg_data[4]/127), Vector2(arg_data[8]/127, arg_data[12]/127), Vector2(1, 1))
	, translationYBezier(Vector2(0, 0), Vector2(arg_data[1]/127, arg_data[5]/127), Vector2(arg_data[9]/127, arg_data[13]/127), Vector2(1, 1))
	, translationZBezier(Vector2(0, 0), Vector2(arg_data[2]/127, arg_data[6]/127), Vector2(arg_data[10]/127, arg_data[14]/127), Vector2(1, 1))
{
}

ButiEngine::LarpData::LarpData()
	:rotationBezier(Vector2(0,0), Vector2(0,0), Vector2(1,1), Vector2(1,1))
	,translationXBezier(Vector2(0,0), Vector2(0,0), Vector2(1,1), Vector2(1,1))
	,translationYBezier(Vector2(0,0), Vector2(0,0), Vector2(1,1), Vector2(1,1))
	,translationZBezier(Vector2(0,0), Vector2(0,0), Vector2(1,1), Vector2(1,1))
{
	
}
