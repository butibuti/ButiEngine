#include "stdafx.h"
#include "..\..\Header\Resources\Bone.h"

ButiEngine::Matrix4x4 ButiEngine::Bone::GetPoseMatrix()
{

	return  (XMMATRIX)bindPoseInverse*(XMMATRIX)transform->GetBoneMatrix();
}

void ButiEngine::Bone::SetOtherBoneLinks(const std::vector<std::shared_ptr< Bone>>& arg_vec_bones)
{
	if (ikDatas.size()) {
		isIK = true;
		for (auto itr = ikDatas.begin(); itr != ikDatas.end(); itr++) {
			itr->shp_targetBone = arg_vec_bones.at(itr->targetBoneIndex);

			auto extremity = itr->shp_targetBone;
			auto bonelinkEnd = itr->links.end();
			std::shared_ptr<Transform> befTrans = itr->shp_targetBone->transform;
			bool first = true;
			for (auto linkItr = itr->links.begin(); linkItr != itr->links.end(); linkItr++) {
				linkItr->shp_linkBone = arg_vec_bones.at(linkItr->linkBone);

				auto bonePos = linkItr->shp_linkBone->transform->GetWorldPosition();
				linkItr->bonesLength= ((Vector3)(befTrans->GetWorldPosition() - bonePos)).GetLength();
				itr->sum_bonesLength += linkItr->bonesLength;
				Vector3 axis = Vector3::XAxis;
				if (linkItr->axis) {
					axis = *(linkItr->axis);
				}
				linkItr->befBoneInitRotation = linkItr->shp_linkBone->transform->GetLookAtRotation(befTrans->GetWorldPosition(), axis).Inverse().ToQuat();

				befTrans = linkItr->shp_linkBone->transform;
			}
			itr->p_jointPoints = (Vector3*)malloc(sizeof(Vector3) * (itr->links.size() + 1));
		}

	}
	if (addBoneIndex < 0) {
		return;
	}
	addBone = arg_vec_bones.at(addBoneIndex);

}

void ButiEngine::Bone::AddBonePower()
{
	if (!addBone) {
		return;
	}

	if (addRotate) {
		auto addRotate = addBone->transform->GetLocalRotation().ToQuat();
		addRotate.w *= addPercentage;
		transform->SetLocalRotation(Matrix4x4(addRotate));
	}
	if (addMove) {
		transform->SetLocalPosition(addBone->transform->GetLocalPosition() * addPercentage);
	}
}

void ButiEngine::Bone::InverseKinematic()
{
	//worldPos
	auto targetPos = transform->GetWorldPosition();

	for (auto itr = ikDatas.begin(); itr != ikDatas.end(); itr++)
	{
		auto extremity = itr->shp_targetBone;
		auto root = itr->links.end() - 1;

		float rootDis =((Vector3) (targetPos - root->shp_linkBone->transform->GetWorldPosition())).GetLength();

		if (itr->sum_bonesLength*transform->GetWorldScale().x <= rootDis) {
			auto bonelinkEnd = itr->links.rend();
			for (auto linkItr = itr->links.rbegin(); linkItr != bonelinkEnd; linkItr++) {
				linkItr->shp_linkBone->transform->SetWorldRotation((linkItr->befBoneInitRotation* linkItr->shp_linkBone->transform->GetLookAtRotation(targetPos, Vector3::XAxis*transform->GetWorldRotation()).ToQuat()).ToMatrix() );

			}
			continue;
		}

		auto bonelinkEnd = itr->links.end();

		for (int loop = 0; loop <1;loop++) {
			auto extremityPos = extremity->transform->GetWorldPosition();
			if ((extremityPos - targetPos).GetLength()<0.005) {
				break;
			}
			const int size = itr->links.size();
			

			for (int i = 0; i < size; i++) {
				itr->p_jointPoints[1+i] = itr->links[i].shp_linkBone->transform->GetWorldPosition();
			}
			itr->p_jointPoints[0] = extremityPos;


			for (int i = 0; i < size; i++) {
				itr->p_jointPoints[i] = targetPos;

				auto scale = transform->GetWorldScale();
				targetPos = targetPos +( ((itr->p_jointPoints[i + 1] - targetPos).GetNormalize() * itr->links[i].bonesLength) *scale);
			}

			int i = size;

			auto bonelinkEnd = itr->links.rend();
			for (auto linkItr = itr->links.rbegin(); linkItr != bonelinkEnd; linkItr++,i--) {
				if (linkItr->axis) {
					linkItr->shp_linkBone->transform->SetWorldRotation((linkItr->befBoneInitRotation * linkItr->shp_linkBone->transform->GetLookAtRotation(itr->p_jointPoints[i - 1], *linkItr->axis*linkItr->shp_linkBone->transform->GetWorldRotation()).ToQuat()).ToMatrix());
					continue;
				}
				Vector3 target = itr->p_jointPoints[i - 1];
				Vector3 nextBonePos;

				auto nextItr = linkItr+1;
				if (nextItr == bonelinkEnd) {
					nextBonePos= extremity->transform->GetWorldPosition();
				}
				else {
					nextBonePos = nextItr->shp_linkBone->transform->GetWorldPosition();
				}

				Vector3 bonePos = linkItr->shp_linkBone->transform->GetWorldPosition();
				auto targetVec = (target - bonePos).GetNormalize() * transform->GetWorldRotation();
				targetVec.RemoveEps();
				auto boneVec = (nextBonePos - bonePos).GetNormalize() * transform->GetWorldRotation();
				Vector3 axis;
				if ((boneVec- targetVec).isZero() ) {
					axis = Vector3::XAxis;
				}
				else {

					axis = ((targetVec).GetCross(boneVec));
					axis.Normalize();
				}
				auto rotation = linkItr->shp_linkBone->transform->GetLookAtRotation(target, axis * transform->GetWorldRotation()).ToQuat();
				Quat invRotation;

				if (nextItr == bonelinkEnd) {
					invRotation = MathHelper::GetLookAtRotation(linkItr->shp_linkBone->position, extremity ->position, axis).ToQuat();
				}
				else {
					invRotation = MathHelper::GetLookAtRotation(linkItr->shp_linkBone->position, nextItr->shp_linkBone->position, axis).ToQuat();
				}

				linkItr->shp_linkBone->transform->SetWorldRotation(((invRotation.Inverse())*rotation).ToMatrix());
				
				
			}

			if(GameDevice::GetInput()->CheckKey(Keys::Q))
			for (auto linkItr = itr->links.rbegin(); linkItr != bonelinkEnd; linkItr++) {

				if (linkItr->radianLimit) {
					auto euler = linkItr->shp_linkBone->transform->GetLocalRotation().GetEulerOneValue_local();
					euler.SetLimit(linkItr->upLimmit, linkItr->downLimmit);
					linkItr->shp_linkBone->transform->SetLocalRotation_radian(euler);
				}
			}

			targetPos = transform->GetWorldPosition();

		}



	}
}

void ButiEngine::Bone::CCDInverseKinematic()
{
	auto targetPos = transform->GetWorldPosition();

	for (auto itr = ikDatas.begin(); itr != ikDatas.end(); itr++)
	{
		auto extremity = itr->shp_targetBone;

		float befDis = 0.0f;
		float nowDis = ((Vector3)(extremity->transform->GetWorldPosition() - targetPos)).GetLengthSqr();

		auto extremityPos = extremity->transform->GetWorldPosition();




		for (int i = 0; i < itr->loopCount; i++)
		{

			for (auto linkItr = itr->links.begin(); linkItr != itr->links.end(); linkItr++) {
				if (nowDis < 0.05f) {
					return;
				}
				befDis = nowDis;



				auto nowJoint = linkItr->shp_linkBone;

				auto nowJointPos = nowJoint->transform->GetWorldPosition();

				auto inv = nowJoint->transform->GetBoneMatrix().GetInverse();

				auto localTargetPos = targetPos * inv;
				auto localnowJointPos = nowJointPos * inv;
				auto localextremityPos = extremityPos * inv;

				Vector3 targetVec = Vector3(localTargetPos - localnowJointPos).GetNormalize();//transform->GetWorldPosition();
				Vector3 boneVec = Vector3(localextremityPos - localnowJointPos).GetNormalize();

				const Quat initRotationQuat = nowJoint->transform->GetLocalRotation().ToQuat();

				Quat rotationQuat;


				Vector3 rotationAxis;


				float rotationDotProduct = targetVec.Dot(boneVec);
				float rotationAngle = acos(rotationDotProduct);



				if (rotationAngle > 1.0e-5f) {
					// ‰ñ“]Ž²
					rotationAxis = boneVec.Cross(targetVec);
					rotationAxis.Normalize();
					rotationQuat = Quat(rotationAxis, rotationAngle) * initRotationQuat;
					rotationQuat.Normalize();

				}
				if (linkItr->radianLimit) {
					auto eular = rotationQuat.ToMatrix().GetEulerOneValue();

					//auto freeze = eular.GetFreeze(linkItr->xFreeze, linkItr->yFreeze, linkItr->zFreeze);

					eular.SetLimit(linkItr->upLimmit, linkItr->downLimmit);
					Quat realRot;

					realRot.CreateFromEular(eular);
					realRot.Normalize();



					nowJoint->transform->SetLocalRotation(Matrix4x4(realRot));// *remainRot));

					extremityPos = extremity->transform->GetWorldPosition();

					nowDis = ((Vector3)(extremityPos - targetPos)).GetLengthSqr();


				}
				else {
					nowJoint->transform->SetLocalRotation(Matrix4x4(rotationQuat));// *remainRot));

					extremityPos = extremity->transform->GetWorldPosition();

					nowDis = ((Vector3)(extremityPos - targetPos)).GetLengthSqr();
				}

				if (nowDis < 0.05f) {
					return;
				}


			}
		}
	}
}

ButiEngine::IKData::~IKData()
{
	if(p_jointPoints)
	free(p_jointPoints);
}
