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

			auto bonelinkEnd = itr->links.end();
			std::shared_ptr<Transform> befTrans = itr->shp_targetBone->transform;
			bool first = true;
			for (auto linkItr = itr->links.begin(); linkItr != itr->links.end(); linkItr++) {
				linkItr->shp_linkBone = arg_vec_bones.at(linkItr->linkBone);

				auto bonePos = linkItr->shp_linkBone->transform->GetWorldPosition();
				linkItr->bonesLength= ((Vector3)(befTrans->GetWorldPosition() - bonePos)).GetLength();
				itr->sum_bonesLength += linkItr->bonesLength;

				linkItr->befBoneInitRotation = linkItr->shp_linkBone->transform->GetLookAtRotation(befTrans->GetWorldPosition(),Vector3::YAxis).Inverse().ToQuat();

				befTrans = linkItr->shp_linkBone->transform;
			}
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

		if (itr->sum_bonesLength <= rootDis) {
			auto bonelinkEnd = itr->links.rend();
			for (auto linkItr = itr->links.rbegin(); linkItr != bonelinkEnd; linkItr++) {
				linkItr->shp_linkBone->transform->SetWorldRotation((linkItr->befBoneInitRotation* linkItr->shp_linkBone->transform->GetLookAtRotation(targetPos, Vector3::YAxis).ToQuat()).ToMatrix() );

			}
			continue;
		}

		auto bonelinkEnd = itr->links.end();

		for (int i = 0; i <itr->loopCount;i++) {
			auto extremityPos = extremity->transform->GetWorldPosition();
			if ((extremityPos - targetPos).GetLength()<0.005) {
				break;
			}

			//worldPos
			Vector3 nextBonePos = extremityPos;
			for (auto linkItr = itr->links.begin(); linkItr != bonelinkEnd; linkItr++) {

				auto inv = linkItr->shp_linkBone->transform->GetBoneMatrix().Inverse();


				auto nowJointPos = linkItr->shp_linkBone->transform->GetWorldPosition();


				auto localnowJointPos = nowJointPos * inv;

				//Vector3 targetVec = Vector3(targetPos-nowJointPos);//transform->GetWorldPosition();
				//Vector3 boneVec = Vector3(nextBonePos-nowJointPos);

				Vector3 targetVec = Vector3(targetPos*inv- localnowJointPos);//transform->GetWorldPosition();
				Vector3 boneVec = Vector3(nextBonePos * inv- localnowJointPos);
				targetVec.Normalize();
				boneVec.Normalize();

				Vector3 rotationAxis = boneVec.GetCross(targetVec);

				//if (boneName == L"ç∂ë´ÇhÇj") {

				//	GUI::Text(targetVec);
				//	GUI::Text(boneVec);
				//}
				float angle =acos(boneVec.Dot(targetVec));
				if (abs(angle) <= FLT_MIN*10) {


					nextBonePos = linkItr->shp_linkBone->transform->GetWorldPosition();
					targetPos = targetPos - Vector3(targetPos - nowJointPos).GetNormalize() * linkItr->bonesLength;
					continue;
				}

				//
				//Ç©Ç≠Ç«ÇπÇ¢Ç∞ÇÒÇÒÇÒÇé
				///
				//
				rotationAxis.Normalize();


				const Quat initRotation = linkItr->shp_linkBone->transform->GetLocalRotation().ToQuat();

				
				if(!rotationAxis.isZero())
				linkItr->shp_linkBone->transform->SetLocalRotation((Quat(rotationAxis, angle).Normalize()*initRotation).ToMatrix());
				else {
					int i = 0;
				}

				nextBonePos = linkItr->shp_linkBone->transform->GetWorldPosition();
				targetPos = targetPos - Vector3(targetPos - nowJointPos).GetNormalize() * linkItr->bonesLength;
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
					// âÒì]é≤
					rotationAxis = boneVec.Cross(targetVec);
					rotationAxis.Normalize();
					rotationQuat = Quat(rotationAxis, rotationAngle) * initRotationQuat;
					rotationQuat.Normalize();

				}
				if (linkItr->radianLimit) {
					auto eular = rotationQuat.ToMatrix().GetEuler();

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

