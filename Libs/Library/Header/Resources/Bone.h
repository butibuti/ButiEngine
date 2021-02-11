#pragma once
#include"../Common/Transform.h"
#include<string>
namespace ButiEngine {

	struct ModelDrawData;
	struct Bone;
	struct Bone;
	struct IKLink {
		~IKLink() {
			if (axis) {
				delete axis;
			}
		}
		int linkBone;
		std::shared_ptr<Bone> shp_linkBone;
		bool radianLimit = false;
		float bonesLength = 0;
		Quat befBoneInitRotation;
		Vector3 upLimit;
		Vector3 downLimit;
		Vector3* axis = nullptr;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(radianLimit);
			archive(upLimit);
			archive(downLimit);
			archive(bonesLength);
			archive(linkBone);
		}
	};
	struct IKData {
		~IKData();
		int targetBoneIndex;
		int extremityBoneIndex;
		std::shared_ptr<Bone> shp_targetBone;
		int loopCount;
		float maxRadian;
		float sum_bonesLength = 0;
		std::vector<IKLink> links;
		//Vector3* p_jointPoints=nullptr;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(targetBoneIndex);
			archive(extremityBoneIndex);
			archive(loopCount);
			archive(maxRadian);
			archive(sum_bonesLength);
			archive(links);
		}
	};
	struct Bone
	{
		int ownIndex;
		std::string boneName;
		std::string boneNameEng;

		Vector3 position;

		int parentBoneIndex=-1;
		int transformFloor=0;

		std::shared_ptr<Bone> parentBone;

		bool isIK=false;

		Matrix4x4 bindPose;
		Matrix4x4 bindPoseInverse;
		
		Matrix4x4 GetPoseMatrix();

		std::shared_ptr<BoneTransform> transform;

		bool canRotate;
		bool canMove;
		bool canControll;

		bool isFreezeAxis=false;
		Vector3 freezeAxis;

		bool addRotate=false;
		bool addMove=false;
		int addBoneIndex=-1;
		float addPercentage;

		std::shared_ptr<Bone> addBone;

		int outParentTransform = -1;
		bool isPhisicsAfterTransform;

		Vector3 localXaxis;
		Vector3 localZaxis;

		int jointBone=-1;
		Vector3 jointPos;
		std::vector<IKData> ikDatas;

		bool addLocal=false;

		void SetOtherBoneLinks(const std::vector<std::shared_ptr< Bone>>& arg_vec_bones);

		void AddBonePower();

		void InverseKinematic();
		void CCDInverseKinematic();

	};
}