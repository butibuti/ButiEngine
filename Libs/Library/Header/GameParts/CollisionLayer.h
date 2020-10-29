#pragma once
#include"stdafx.h"
#include"Header/Common/CollisionPrimitive.h"
namespace ButiEngine {

	namespace Collision {
		const char MaxLevel = 7;
		class OctCell; 
		class OctRegistObj;
		struct CollisionObject:public IObject {
			CollisionObject(std::weak_ptr<CollisionPrimitive> arg_wkp_coliisionPrim, std::weak_ptr<GameObject> arg_wkp_gameObj) {
				wkp_coliisionPrim = arg_wkp_coliisionPrim;
				wkp_gameObj = arg_wkp_gameObj;
			}
			~CollisionObject(){

			}
			void Initialize()override {}
			void PreInitialize()override {}
			std::weak_ptr<CollisionPrimitive> wkp_coliisionPrim;
			std::weak_ptr<GameObject> wkp_gameObj;
		};

		class OctRegistObj:public std::enable_shared_from_this<OctRegistObj>
		{
		public:
			inline OctRegistObj(std::shared_ptr< CollisionObject> arg_shp_collisionObj) {

				shp_collisionObject = arg_shp_collisionObj;

			}
			~OctRegistObj() {
				shp_collisionObject = nullptr;
			}
			OctCell* p_cell=nullptr;			
			std::shared_ptr< CollisionObject> shp_collisionObject;				
			std::shared_ptr<OctRegistObj> shp_next=nullptr;	
			std::shared_ptr<OctRegistObj> shp_bef=nullptr;	

			bool Remove();

			void RegistCell(OctCell* arg_pCell)
			{
				p_cell = arg_pCell;
			}

			std::shared_ptr<OctRegistObj> GetBefObj() {
				return shp_bef;
			}
		};


		class OctCell {
		public:
			OctCell() {

			}
			~OctCell() {
				while (shp_head&&shp_head->shp_next)
				{
					shp_head = shp_head->shp_next;
					shp_head->shp_bef = nullptr;
				}
				shp_head = nullptr;
			}
			inline void RegistObject(std::shared_ptr<OctRegistObj> arg_obj) {

				if (arg_obj->p_cell == this||!arg_obj)
					return;

				arg_obj->Remove();

				arg_obj->p_cell = this;

				if (shp_head) {
					arg_obj->shp_next = shp_head;
					shp_head->shp_bef = arg_obj;
					shp_head = arg_obj;
				}

				shp_head = arg_obj;
			}
			void OnRemove(OctRegistObj* arg_remove)
			{
				if ((shp_head.get() ==arg_remove )&& arg_remove) {
					shp_head = shp_head->shp_next;
				}
			}
			std::shared_ptr< OctRegistObj>& GetHead() {
				return shp_head;
			}
		private:
			std::shared_ptr< OctRegistObj> shp_head;

		};

		class CollisionLayer :public IObject
		{
		public:
			CollisionLayer(const unsigned char  arg_level,const Vector3& arg_minPos,const Vector3& arg_maxPos);
			~CollisionLayer() {
				vec_shp_collisionObjs.clear();
				for (auto itr = vec_index.begin(); itr != vec_index.end(); itr++) {
					delete (*itr);
				}
				for (int i = 0; i < maxCellNum; i++) {
					if (ary_cells[i] )
						delete ary_cells[i];
				}
				delete[] ary_cells;
			}
			unsigned int* RegistCollisionObj(std::shared_ptr< CollisionObject> arg_collisionObj);
			void UnRegistCollisionObj(unsigned int* arg_index);
			void Initialize()override;
			void PreInitialize()override{}
			void Update();
			inline void RegistOctree() {
				for (auto itr = vec_shp_collisionObjs.begin(); itr != vec_shp_collisionObjs.end(); itr++) {
					Vector3 minPoint, maxPoint;
					(*itr)->shp_collisionObject-> wkp_coliisionPrim.lock()->GetMaxPointAndMinPoint(maxPoint, minPoint);
					auto cellNum = GetMortonSpace(minPoint, maxPoint);

					if (cellNum > maxCellNum) {
						continue;
					}
					if (!ary_cells[cellNum]) {
						CreateCell(cellNum);
					}
					ary_cells[cellNum]->RegistObject(*itr);
				}
			}
			void HitCheck();
		private:
			inline DWORD  Get3DMortonNumber(const unsigned char  x,const unsigned char  y,const unsigned char  z)
			{
				return OctreeHelper::BitSeparate(x) | OctreeHelper::BitSeparate(y) << 1 | OctreeHelper::BitSeparate(z) << 2;
			}
			inline DWORD  Get3DMortonNumber(const Vector3& arg_position)
			{
				return Get3DMortonNumber(
					(unsigned char)((arg_position.x - rangeMin.x) / unit.x),
					(unsigned char)((arg_position.y - rangeMin.y) / unit.y),
					(unsigned char)((arg_position.z - rangeMin.z) / unit.z));
			}
			inline unsigned short GetMortonSpace(const Vector3& arg_minPos, const Vector3& arg_maxPos) {
				
				auto maxSpace = Get3DMortonNumber(arg_maxPos );
				auto levelCheckFlag = Get3DMortonNumber(arg_minPos) ^maxSpace;
				auto level = OctreeHelper::GetLevel(levelCheckFlag,maxLevel);
				auto num= (maxSpace >> ((level)*3 ));
				
				num += OctPowSevenDevided[maxLevel - level];
				return num;
			}
			inline void CreateCollisionObjectList(const unsigned short arg_cellNum, std::vector<std::shared_ptr< CollisionObject>>& arg_output, std::list<std::shared_ptr< CollisionObject>>& arg_stack) {
				
				auto objItr= ary_cells[arg_cellNum]->GetHead();
				while (objItr)
				{
					auto rObjItr =objItr->shp_next;
					while (rObjItr!=nullptr) {
						// 衝突リスト作成
						arg_output.push_back( objItr->shp_collisionObject);
						arg_output.push_back(rObjItr->shp_collisionObject);

						rObjItr = rObjItr->shp_next;
					}
					// ② 衝突スタックとの衝突リスト作成
					for (auto itr = arg_stack.begin(); itr != arg_stack.end(); itr++) {
						arg_output.push_back(objItr->shp_collisionObject);
						arg_output.push_back(*itr);
					}
					objItr = objItr->shp_next;
				}

				bool ChildFlag = false;
				// ③ 子空間に移動
				DWORD ObjNum = 0;
				DWORD i, nextCellNum;
				for (i = 0; i < 8; i++) {
					nextCellNum = arg_cellNum * 8 + 1 + i;
					if (nextCellNum < maxCellNum && ary_cells[arg_cellNum * 8 + 1 + i]) {
						if (!ChildFlag) {
							objItr = ary_cells[arg_cellNum]->GetHead();
							while (objItr) {
								arg_stack.push_back(objItr->shp_collisionObject);
								ObjNum++;
								objItr = objItr->shp_next;
							}
						}
						ChildFlag = true;
						CreateCollisionObjectList(arg_cellNum * 8 + 1 + i, arg_output, arg_stack);	
					}
				}

				// ⑤ スタックからオブジェクトを外す
				if (ChildFlag) {
					for (i = 0; i < ObjNum; i++)
						arg_stack.pop_back();
				}

			}
			inline void CreateCell(unsigned short arg_cellNum) {

				while (!ary_cells[arg_cellNum])
				{
					// 指定の要素番号に空間を新規作成
					ary_cells[arg_cellNum]= new OctCell();

					// 親空間にジャンプ
					arg_cellNum = (arg_cellNum - 1) >> 3;
					if (arg_cellNum >=maxCellNum) break;
				}

			}
			std::vector<std::shared_ptr< OctRegistObj>> vec_shp_collisionObjs;
			std::vector<UINT*>vec_index;
			OctCell** ary_cells;
			UINT OctPow[MaxLevel + 1];
			UINT OctPowSevenDevided[MaxLevel + 1];
			Vector3 width;	
			Vector3 rangeMin;	
			Vector3 rangeMax;	
			Vector3 unit;		
			DWORD maxCellNum;	
			unsigned char maxLevel;
		};

	}
}