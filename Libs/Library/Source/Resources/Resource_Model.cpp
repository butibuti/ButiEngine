#include "stdafx.h"

#include"Header/Resources/Resource_Model.h"
#include "..\..\Header\Resources\Resource_Model.h"

ButiEngine::Resource_Model::Resource_Model()
{
}

void ButiEngine::Resource_Model::SetMeshTag(const MeshTag & arg_meshTag)
{
	meshTag = arg_meshTag;
}

void ButiEngine::Resource_Model::SetMaterialVec(const std::vector<MaterialTag>& arg_materialTags)
{
	vec_materialTags = arg_materialTags;
}

void ButiEngine::Resource_Model::SetName(const std::wstring & arg_name)
{
	name = arg_name;
}

void ButiEngine::Resource_Model::SetEngName(const std::wstring & arg_engName)
{
	nameEng = arg_engName;
}

void ButiEngine::Resource_Model::SetModelName(const std::wstring & arg_modelName)
{
	modelName = arg_modelName;
}

void ButiEngine::Resource_Model::SetEngModelName(const std::wstring & arg_engModelName)
{
	modelNameEng = arg_engModelName;
}

void ButiEngine::Resource_Model::SetComment(const std::wstring & arg_comment)
{
	comment = arg_comment;
}

void ButiEngine::Resource_Model::SetEngComment(const std::wstring & arg_engComment)
{
	commentEng = arg_engComment;
}

void ButiEngine::Resource_Model::AddBone(Bone & arg_bone)
{
	vec_bone.push_back(arg_bone);
}

void ButiEngine::Resource_Model::AddMorph(std::shared_ptr<Morph::Morph> arg_morph)
{
	vec_morphs.push_back(arg_morph);
}

void ButiEngine::Resource_Model::AddMaterial(const MaterialTag & arg_materialTag)
{
	vec_materialTags.push_back(arg_materialTag);
}

void ButiEngine::Resource_Model::SetVersion(const float arg_version)
{
	version = arg_version;
}

void ButiEngine::Resource_Model::SetSubset(const std::vector<UINT>& arg_subset)
{
	subset = arg_subset;
}

const ButiEngine::MeshTag ButiEngine::Resource_Model::GetMeshTag()
{
	return meshTag;
}


std::vector<ButiEngine::MaterialTag>& ButiEngine::Resource_Model::GetMaterialTags()
{
	return vec_materialTags;
}

std::vector<std::shared_ptr< ButiEngine::Bone>> ButiEngine::Resource_Model::GetBone()
{
	std::vector<std::shared_ptr<Bone>> out;

	out.reserve(vec_bone.size());

	for (auto itr = vec_bone.begin(); itr != vec_bone.end(); itr++) {
		auto push = ObjectFactory::CreateCopy<Bone>(*itr);
		push->transform = ObjectFactory::Create<BoneTransform>(push->position);
		out.push_back(push);
	}

	out.shrink_to_fit();

	for (auto itr = out.begin(); itr != out.end(); itr++) {

		int parentTransformIndex = (*itr)->parentBoneIndex;

		if (parentTransformIndex >= 0) {
			(*itr)->parentBone = out.at(parentTransformIndex);
			(*itr)->transform->SetBaseTransform((*itr)->parentBone->transform,false);
			(*itr)->transform->SetParentTransform((*itr)->parentBone->transform->GetThis<BoneTransform>(),true);
		}
		(*itr)->bindPose = (*itr)->transform->GetMatrix();
		(*itr)->bindPoseInverse = (*itr)->transform->GetMatrix().GetInverse();

	}


	for (auto itr = out.begin(); itr != out.end(); itr++) {
		(*itr)->SetOtherBoneLinks(out);
	}

	return out;
}

std::vector<UINT> ButiEngine::Resource_Model::GetSubset()
{
	return subset;
}

const std::wstring & ButiEngine::Resource_Model::GetName()
{
	return name;
}

const std::wstring & ButiEngine::Resource_Model::GetEngName()
{
	return nameEng;
}

const std::wstring & ButiEngine::Resource_Model::GetModelName()
{
	return modelName;
}

const std::wstring & ButiEngine::Resource_Model::GetEngModelName()
{
	return modelNameEng;
}

const std::wstring & ButiEngine::Resource_Model::GetComment()
{
	return comment;
}

const std::wstring & ButiEngine::Resource_Model::GetEngComment()
{
	return commentEng;
}
