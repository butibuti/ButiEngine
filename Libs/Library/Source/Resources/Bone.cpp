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
			/*if (itr-> p_jointPoints) {
				free(itr->p_jointPoints);
			}*/
			//itr->p_jointPoints = (Vector3*)malloc(sizeof(Vector3) * (itr->links.size() + 1));
			int i = 0;
			for (auto linkItr = itr->links.begin(); linkItr != itr->links.end(); linkItr++,i++) {
				linkItr->shp_linkBone = arg_vec_bones.at(linkItr->linkBone);

				auto bonePos = linkItr->shp_linkBone->transform->GetWorldPosition();
				linkItr->bonesLength= ((Vector3)(befTrans->GetWorldPosition() - bonePos)).GetLength();
				itr->sum_bonesLength += linkItr->bonesLength;
				Vector3 axis = Vector3::XAxis;
				if (linkItr->axis) {
					axis = *(linkItr->axis);
				}
				linkItr->befBoneInitRotation = linkItr->shp_linkBone->transform->GetLookAtRotation(befTrans->GetWorldPosition(), axis).Inverse().ToQuat();
				//itr->p_jointPoints[i + 1] = linkItr->shp_linkBone->transform->GetWorldPosition();
				befTrans = linkItr->shp_linkBone->transform;
			}
			//itr->p_jointPoints[0] = extremity->transform->GetWorldPosition();
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

		auto bonelinkEnd_r = itr->links.rend();

		float rootDis =((Vector3) (targetPos - root->shp_linkBone->transform->GetWorldPosition())).GetLength();

		//届かない距離ならスキップ
		if (itr->sum_bonesLength*transform->GetWorldScale().x <= rootDis) {
			auto bonelinkEnd = itr->links.rend();
			for (auto linkItr = itr->links.rbegin(); linkItr != bonelinkEnd; linkItr++) {
				linkItr->shp_linkBone->transform->SetWorldRotation((linkItr->befBoneInitRotation* linkItr->shp_linkBone->transform->GetLookAtRotation(targetPos, Vector3::XAxis*transform->GetWorldRotation()).ToQuat()).ToMatrix() );

			}
			continue;
		}

		auto extremityPos = extremity->transform->GetWorldPosition();
		if (((Vector3)(extremityPos - targetPos)).GetLength() <= 0.01) {
			break;
		}
		auto bonelinkEnd = itr->links.end();

		int i = 0;

		const int size = itr->links.size();

		auto scale = transform->GetWorldScale();
		for (auto linkItr = itr->links.begin(); linkItr != bonelinkEnd; linkItr++, i++) {
			//itr->p_jointPoints[i + 1] = linkItr->shp_linkBone->transform->GetWorldPosition();

		}
		

		for (int loop = 0; loop < 1; loop++) {
			int index = size;

			i = 0;
			//forward
			Vector3 befTargetPos;
			for (auto linkItr = itr->links.begin(); linkItr != bonelinkEnd; linkItr++, i++) {

				//itr->p_jointPoints[i] = targetPos;
				befTargetPos = targetPos;

				//targetPos = targetPos - (((targetPos-itr->p_jointPoints[i + 1] ).GetNormalize() * itr->links[i].bonesLength) * scale);


				auto nextItr = linkItr + 1;
				if (linkItr->radianLimit && nextItr != bonelinkEnd) {

					auto initPos = linkItr->shp_linkBone->transform->GetWorldPosition();
					linkItr->shp_linkBone->transform->SetWorldPosition(targetPos);



					Vector3 target;// = itr->p_jointPoints[i];
					std::shared_ptr<Bone> nextBone;
					Vector3 nextBonePos;


					if (linkItr == itr->links.begin()) {
						nextBone = extremity;
					}
					else {
						nextBone = (linkItr - 1)->shp_linkBone;
					}

					nextBonePos = nextBone->transform->GetWorldPosition();



					Vector3 bonePos = linkItr->shp_linkBone->transform->GetWorldPosition();
					auto targetVec = (target - bonePos).GetNormalize();

					auto boneVec = (nextBonePos - bonePos).GetNormalize();
					Vector3 axis;
					if ((boneVec - targetVec).isZero()) {
						axis = Vector3::XAxis;
					}
					else {

						axis = ((targetVec).GetCross(boneVec));
						axis.Normalize();
					}
					auto rotation = linkItr->shp_linkBone->transform->GetLookAtRotation(target, axis * transform->GetWorldRotation()).ToQuat();
					Quat invRotation = MathHelper::GetLookAtRotation(linkItr->shp_linkBone->position, nextBone->position, axis).ToQuat();


					auto noConstraintRotate = invRotation.Inverse() * rotation*linkItr->shp_linkBone->transform->GetBaseTransform()-> GetWorldRotation().Inverse().ToQuat();

					linkItr->shp_linkBone->transform->SetLocalRotation(noConstraintRotate.ToMatrix());

					Vector3 noConstraintPos = nextBone->transform->GetWorldPosition();


					auto euler = noConstraintRotate.ToMatrix().GetEulerOneValue_local();
					euler.SetLimit(linkItr->upLimit, linkItr->downLimit);

					linkItr->shp_linkBone->transform->SetLocalRotation_radian((euler));

					auto different = nextBone->transform->GetWorldPosition() - noConstraintPos;
					//itr->p_jointPoints[i] -= different;
					targetPos =linkItr->shp_linkBone->transform->GetWorldPosition()- different;

					linkItr->shp_linkBone->transform->SetWorldPosition(initPos);

				}
				else {

				}
			}
			//backword
			i = 0;
			Vector3 offset =targetPos- root->shp_linkBone->transform->GetWorldPosition();
			GUI::Text(offset);

			for (auto linkItr = itr->links.begin(); linkItr != bonelinkEnd; linkItr++, i++) {
				//itr->p_jointPoints[i] += offset;
			}



			targetPos = transform->GetWorldPosition();

			//solve
			for (auto linkItr = itr->links.rbegin(); linkItr != bonelinkEnd_r; linkItr++, index--) {
				if (linkItr->axis) {
					//linkItr->shp_linkBone->transform->SetWorldRotation((linkItr->befBoneInitRotation * linkItr->shp_linkBone->transform->GetLookAtRotation(itr->p_jointPoints[index - 1], *linkItr->axis * linkItr->shp_linkBone->transform->GetWorldRotation()).ToQuat()).ToMatrix());
					continue;
				}
				Vector3 target;// = itr->p_jointPoints[index - 1];
				Vector3 nextBonePos;

				auto nextItr = linkItr + 1;
				if (nextItr == bonelinkEnd_r) {
					nextBonePos = extremity->transform->GetWorldPosition();
				}
				else {
					nextBonePos = nextItr->shp_linkBone->transform->GetWorldPosition();
				}

				Vector3 bonePos = linkItr->shp_linkBone->transform->GetWorldPosition();
				auto targetVec = (target - bonePos).GetNormalize();

				auto boneVec = (nextBonePos - bonePos).GetNormalize();
				Vector3 axis;
				if ((boneVec - targetVec).isZero()) {
					axis = Vector3::XAxis;
				}
				else {

					axis = ((targetVec).GetCross(boneVec));
					axis.Normalize();
				}
				auto rotation = linkItr->shp_linkBone->transform->GetLookAtRotation(target, axis * transform->GetWorldRotation()).ToQuat();
				Quat invRotation;

				if (nextItr == bonelinkEnd_r) {
					invRotation = MathHelper::GetLookAtRotation(linkItr->shp_linkBone->position, extremity->position, axis).ToQuat();
				}
				else {
					invRotation = MathHelper::GetLookAtRotation(linkItr->shp_linkBone->position, nextItr->shp_linkBone->position, axis).ToQuat();
				}

				linkItr->shp_linkBone->transform->SetWorldRotation(((invRotation.Inverse()) * rotation).ToMatrix());

				if (linkItr->radianLimit) {
					auto quat = linkItr->shp_linkBone->transform->GetLocalRotation().ToQuat();
					auto euler = quat.ToMatrix().GetEulerOneValue_local();
					euler.SetLimit(linkItr->upLimit, linkItr->downLimit);

					linkItr->shp_linkBone->transform->SetLocalRotation_radian(euler);

				}
			}
			index = 0;
		}

	}
}

void ButiEngine::Bone::CCDInverseKinematic()
{

	auto targetPos = transform->GetWorldPosition();
	

	for (auto itr = ikDatas.begin(); itr != ikDatas.end(); itr++)
	{
		auto extremity = itr->shp_targetBone;
		auto root = itr->links.end() - 1;
		float befDis = 0.0f;
		float nowDis = ((Vector3)(extremity->transform->GetWorldPosition() - targetPos)).GetLengthSqr();

		auto extremityPos = extremity->transform->GetWorldPosition();
		float rootDis = ((Vector3)(targetPos - root->shp_linkBone->transform->GetWorldPosition())).GetLength();

		auto bonelinkEnd = itr->links.end();
		//届かない距離ならスキップ
		if (itr->sum_bonesLength * transform->GetWorldScale().x <= rootDis) {
			auto bonelinkEnd = itr->links.rend();
			for (auto linkItr = itr->links.rbegin(); linkItr != bonelinkEnd; linkItr++) {
				linkItr->shp_linkBone->transform->SetWorldRotation((linkItr->befBoneInitRotation * linkItr->shp_linkBone->transform->GetLookAtRotation(targetPos, Vector3::XAxis * transform->GetWorldRotation()).ToQuat()).ToMatrix());

			}
			continue;
		}



		for (int i = 0; i < itr->loopCount/2; i++)
		{

			for (auto linkItr = itr->links.begin(); linkItr != bonelinkEnd; linkItr++) {
				if (nowDis < 0.0025f) {
					return;
				}
				befDis = nowDis;



				auto nowJoint = linkItr->shp_linkBone;


				auto inv = nowJoint->transform->GetBoneMatrix().GetInverse();

				auto localTargetPos = targetPos * inv;
				auto localextremityPos = extremityPos * inv;

				Vector3 targetVec = Vector3(localTargetPos).GetNormalize();//transform->GetWorldPosition();
				Vector3 boneVec = Vector3(localextremityPos ).GetNormalize();

				const Quat initRotationQuat = nowJoint->transform->GetLocalRotation().ToQuat();

				Quat rotationQuat;


				Vector3 rotationAxis;


				float rotationDotProduct = targetVec.Dot(boneVec);
				float rotationAngle = acos(rotationDotProduct);



				if (rotationAngle > 1.0e-5f) {
					// 回転軸
					rotationAxis = boneVec.Cross(targetVec);
					rotationAxis.Normalize();
					rotationQuat = Quat(rotationAxis, rotationAngle) * initRotationQuat;
					rotationQuat.Normalize();

				}
				if (linkItr->radianLimit) {
					auto eular = rotationQuat.ToMatrix().GetEulerOneValue();

					//auto freeze = eular.GetFreeze(linkItr->xFreeze, linkItr->yFreeze, linkItr->zFreeze);

					eular.SetLimit(linkItr->upLimit, linkItr->downLimit);
					Quat realRot;

					realRot.CreateFromEular(eular);
					realRot.Normalize();


					auto surplusRotate = realRot.GetInverse() * rotationQuat;

					nowJoint->transform->SetLocalRotation(Matrix4x4(realRot));// *remainRot));

					auto next = linkItr + 1;
					if (next != bonelinkEnd) {


						next->shp_linkBone->transform->RollLocalRotation(MathHelper::LearpQuat(Quat(), surplusRotate,nowDis/itr->sum_bonesLength).ToMatrix());

					}

					extremityPos = extremity->transform->GetWorldPosition();


					nowDis = ((Vector3)(extremityPos - targetPos)).GetLengthSqr();
					

				}
				else {
					nowJoint->transform->SetLocalRotation(Matrix4x4(rotationQuat));// *remainRot));

					extremityPos = extremity->transform->GetWorldPosition();

					nowDis = ((Vector3)(extremityPos - targetPos)).GetLengthSqr();
				}

				if (nowDis < 0.0025f) {
					return;
				}


			}
		}
	}
}

ButiEngine::IKData::~IKData()
{
}
