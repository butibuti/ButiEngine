
#include"stdafx.h"
#include"../Bone.h"

#ifndef IDRAWOBJ_H
#define IDRAWOBJ_H

namespace ButiEngine {


	namespace Collision {
		class CollisionPrimitive;
		class CollisionPrimitive_Box_AABB;
		class CollisionPrimitive_Box_OBB;
	}

	class IDrawObject {
	public:
		virtual void Draw() = 0;
		virtual void DrawBefore() {}
		virtual float GetZ() = 0;

		virtual std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> GetMeshAABB() = 0;
		virtual std::shared_ptr<Collision::CollisionPrimitive_Box_OBB> GetMeshOBB() = 0;
		virtual void SetPrimitive(std::shared_ptr<Collision::CollisionPrimitive>arg_prim) = 0;
		virtual void SetOctRegistPtr(unsigned int* arg_ptr) = 0;
		virtual std::shared_ptr<Collision::CollisionPrimitive> GetPrimitive() = 0;
		virtual unsigned int* GetOctRegistPtr() = 0;
	};
	class IBoneDrawObject {
	public:
		virtual void ModelUpdate() = 0;
		std::vector<std::shared_ptr<Bone>> vec_bone;
		std::vector<std::shared_ptr<Bone>> vec_IKBone;
		std::vector<std::shared_ptr<Bone>> vec_addBone;

		void InverseKinematic();
		inline void BonePowerAdd() {

			for (auto itr = vec_addBone.begin(); itr != vec_addBone.end(); itr++) {
				(*itr)->AddBonePower();
			}
		}

		std::vector<std::shared_ptr<Bone>>& GetBones() {
			return vec_bone;
		}
		std::vector<std::shared_ptr<Bone>>& GetIKBones() {
			return vec_IKBone;
		}
		void SetIKBone() {
			for (auto itr = vec_bone.begin(); itr != vec_bone.end(); itr++) {
				if ((*itr)->isIK) {
					vec_IKBone.push_back(*itr);
				}
			}
		}
		void SetAddBone() {
			for (auto itr = vec_bone.begin(); itr != vec_bone.end(); itr++) {
				if ((*itr)->addBoneIndex >= 0) {
					vec_addBone.push_back(*itr);
				}
			}
		}
		inline std::shared_ptr<Bone> SerchBoneByName(const std::wstring& arg_boneName) {
			for (auto itr = vec_bone.begin(); itr != vec_bone.end(); itr++) {
				if ((*itr)->boneName == arg_boneName) {
					return (*itr);
				}
			}
			return nullptr;
		}
		inline std::shared_ptr<Bone> SerchBoneByEngName(const std::wstring& arg_engBoneName) {
			for (auto itr = vec_bone.begin(); itr != vec_bone.end(); itr++) {
				if ((*itr)->boneNameEng == arg_engBoneName) {
					return (*itr);
				}
			}
			return nullptr;
		}
		inline std::shared_ptr<Bone> SerchBoneByIndex(const int arg_index) {
			if (arg_index < 0 || arg_index>vec_bone.size() - 1) {
				return nullptr;
			}
			else
			{
				return vec_bone.at(arg_index);
			}
		}
	};

}

#endif // !IDRAWOBJ_H
