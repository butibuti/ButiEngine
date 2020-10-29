#pragma once
#include"stdafx.h"
#include"Bone.h"
#include"Morph.h"
namespace ButiEngine {


	class Resource_Model:public IResource_Model
	{
	public:
		Resource_Model();

		void Initialize()override {};
		void PreInitialize()override {};

		void SetMeshTag(const MeshTag& arg_meshTag) override;
		void SetMaterialVec(const std::vector<MaterialTag>& arg_materialTags) override;
		void SetName(const std::wstring & arg_name) override;
		void SetEngName(const std::wstring & arg_engName) override;
		void SetModelName(const std::wstring & arg_modelName) override;
		void SetEngModelName(const std::wstring & arg_engModelName) override;
		void SetComment(const std::wstring & arg_comment) override;
		void SetEngComment(const std::wstring & arg_engComment) override;
		void AddBone(Bone& arg_bone) override;
		void AddMorph(std::shared_ptr<Morph::Morph> arg_morph) override;
		void AddMaterial(const MaterialTag & arg_materialTag) override;
		void SetVersion(const float arg_version) override;
		void SetSubset(const std::vector<UINT>& arg_subset) override;
		const MeshTag GetMeshTag() override;
		std::vector<MaterialTag>& GetMaterialTags() override;
		std::vector<UINT> GetSubset() override;
		const std::wstring & GetName() override;
		const std::wstring & GetEngName() override;
		const std::wstring & GetModelName() override;
		const std::wstring & GetEngModelName() override;
		const std::wstring & GetComment() override;
		const std::wstring & GetEngComment() override;
		std::vector<std::shared_ptr<Bone>> GetBone() override;
	private:
		std::wstring modelName;
		std::wstring modelNameEng;
		std::wstring comment;
		std::wstring commentEng;
		std::wstring name;
		std::wstring nameEng;
		std::vector<std::shared_ptr< Morph::Morph>> vec_morphs;

		std::vector<UINT> subset;

		MeshTag meshTag;
		std::vector<MaterialTag> vec_materialTags;

		std::vector<Bone> vec_bone;

		float version;
	};

}