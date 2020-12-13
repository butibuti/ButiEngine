#pragma once
#include"stdafx.h"
#include"../Common/CollisionPrimitive.h"
namespace ButiEngine {

	namespace Collision {
		const char MaxLevel = 7;

		template<typename T>
		class OctCell;
		template<typename T>
		class OctRegistObj;

		template<typename T>
		struct CollisionObject :public IObject {

			CollisionObject(std::weak_ptr<CollisionPrimitive> arg_wkp_coliisionPrim, std::weak_ptr<T> arg_wkp_obj) {
				wkp_coliisionPrim = arg_wkp_coliisionPrim;
				wkp_obj = arg_wkp_obj;
			}
			~CollisionObject() {

			}
			void Initialize()override {}
			void PreInitialize()override {}
			std::weak_ptr<CollisionPrimitive> wkp_coliisionPrim;
			std::weak_ptr<T> wkp_obj;
		};

		using CollisionObject_GameObject = CollisionObject<GameObject>;
		using CollisionObject_DrawObject = CollisionObject<IDrawObject>;

		template<typename T>
		class OctRegistObj:public std::enable_shared_from_this<OctRegistObj<T>>
		{
		public:
			inline OctRegistObj(std::shared_ptr<CollisionObject <T>> arg_shp_collisionObj) {

				shp_collisionObject = arg_shp_collisionObj;

			}
			~OctRegistObj() {
				shp_collisionObject = nullptr;
			}
			OctCell<T>* p_cell=nullptr;		
			std::shared_ptr<CollisionObject< T>> shp_collisionObject;				
			std::shared_ptr<OctRegistObj<T>> shp_next=nullptr;	
			std::shared_ptr<OctRegistObj<T>> shp_bef=nullptr;	

			bool Remove()
			{
				if (!p_cell)
					return false;


				if (shp_next)
				{
					shp_next->shp_bef = shp_bef;
				}
				if (shp_bef)
				{
					shp_bef->shp_next = shp_next;
				}
				p_cell->OnRemove(this);
				shp_next = nullptr;
				shp_bef = nullptr;
				p_cell = nullptr;
				return true;
			}

			void RegistCell(OctCell<T>* arg_pCell)
			{
				p_cell = arg_pCell;
			}

			std::shared_ptr<OctRegistObj<T>> GetBefObj() {
				return shp_bef;
			}
		};


		template<typename T>
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
			inline void RegistObject(std::shared_ptr<OctRegistObj< T>> arg_obj) {

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
			void OnRemove(OctRegistObj< T>* arg_remove)
			{
				if ((shp_head.get() ==arg_remove )&& arg_remove) {
					shp_head = shp_head->shp_next;
				}
			}
			std::shared_ptr< OctRegistObj<T>>& GetHead() {
				return shp_head;
			}
		private:
			std::shared_ptr< OctRegistObj< T>> shp_head;

		};

		template<typename T>
		class CollisionLayer :public IObject
		{
		public:
			CollisionLayer(const unsigned char  arg_level,const Vector3& arg_minPos,const Vector3& arg_maxPos) {

				if (arg_level <= MaxLevel)
					maxLevel = arg_level;
				else {
					maxLevel = MaxLevel;
				}
				rangeMax = arg_maxPos;
				rangeMin = arg_minPos;
				width = rangeMax - rangeMin;
				OctPow[0] = 1;
				OctPowSevenDevided[0] = 0;
				for (int i = 1; i < MaxLevel + 1; i++) {
					OctPow[i] = OctPow[i - 1] * 8;
					OctPowSevenDevided[i] = (OctPow[i] - 1) / 7;
				}
				maxCellNum = OctPowSevenDevided[MaxLevel - 1];

				unit = width / ((float)(1 << maxLevel));
				ary_cells = new OctCell<T> * [maxCellNum]();

				CreateCell(0);

			}
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
			unsigned int* RegistCollisionObj(std::shared_ptr< CollisionPrimitive> arg_primitive, std::shared_ptr< T> arg_collisionObj) {

				UINT* index = new UINT(vec_shp_collisionObjs.size());

				vec_shp_collisionObjs.push_back(std::make_shared<OctRegistObj<T>>(ObjectFactory::Create<CollisionObject<T>>(arg_primitive,arg_collisionObj)));

				vec_index.push_back(index);
				return index;

			}
			void UnRegistCollisionObj(unsigned int* arg_index) {
				auto index = *arg_index;
				if (index >= vec_shp_collisionObjs.size()) {
					return;
				}
				auto itr = vec_shp_collisionObjs.begin();
				itr += index;
				(*itr)->Remove();
				vec_shp_collisionObjs.erase(itr);

				delete arg_index;
				auto numItr = vec_index.begin() + index;
				numItr = vec_index.erase(numItr);

				for (; numItr != vec_index.end(); numItr++) {
					*(*numItr) -= 1;
				}
			}
			void Initialize()override{}
			void PreInitialize()override{}
			void Update() {
				RegistOctree();

			}
			inline void RegistOctree() {
				Vector3 minPoint, maxPoint;
				for (auto itr = vec_shp_collisionObjs.begin(); itr != vec_shp_collisionObjs.end(); itr++) {
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
			void Check(std::vector<std::shared_ptr<CollisionObject< T>>>& vec_collisionObjects) {
				std::list<std::shared_ptr<CollisionObject< T>>> list_objStack;


				CreateCollisionObjectList(0, vec_collisionObjects, list_objStack);

			}

			void Check(std::shared_ptr<CollisionPrimitive> arg_shp_checkPrimitive, std::vector<std::shared_ptr<CollisionObject< T>>>& arg_vec_collisionObjects) {

				Vector3 minPoint, maxPoint;

				arg_shp_checkPrimitive->GetMaxPointAndMinPoint(maxPoint, minPoint);
				auto cellNum = GetMortonSpace(minPoint, maxPoint);
				if (!ary_cells[cellNum] && cellNum < maxCellNum) {
					CreateCell(cellNum);
				}
				std::vector<std::shared_ptr<CollisionObject< T>>> vec_collisionObjects;
				CreateObjectList(cellNum, vec_collisionObjects);
				auto endItr = vec_collisionObjects.end();
				for (auto itr = vec_collisionObjects.begin(); itr != endItr; itr++) {
					if ((*itr)->wkp_coliisionPrim.lock()->IsHit(arg_shp_checkPrimitive)) {
						arg_vec_collisionObjects.push_back((*itr));
					}
				}

			}
			void Check(std::shared_ptr<CollisionPrimitive> arg_shp_checkPrimitive, std::vector<std::shared_ptr< T>>& arg_vec_collisionObjects) {

				Vector3 minPoint, maxPoint;

				arg_shp_checkPrimitive->GetMaxPointAndMinPoint(maxPoint, minPoint);
				auto cellNum = GetMortonSpace(minPoint, maxPoint);
				if (!ary_cells[cellNum] && cellNum < maxCellNum) {
					CreateCell(cellNum);
				}
				std::vector<std::shared_ptr<CollisionObject< T>>> vec_collisionObjects;
				CreateObjectList(cellNum, vec_collisionObjects);
				auto endItr = vec_collisionObjects.end();
				for (auto itr = vec_collisionObjects.begin(); itr != endItr; itr++) {
					if ((*itr)->wkp_coliisionPrim.lock()->IsHit(arg_shp_checkPrimitive)) {
						arg_vec_collisionObjects.push_back((*itr)->wkp_obj.lock());
					}
				}

			}

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
				if (arg_minPos == Vector3::Zero && arg_maxPos == Vector3::Zero) {
					return 0;
				}
				auto maxSpace = Get3DMortonNumber(arg_maxPos );
				auto levelCheckFlag = Get3DMortonNumber(arg_minPos) ^maxSpace;
				auto level = OctreeHelper::GetLevel(levelCheckFlag,maxLevel);
				auto num= (maxSpace >> ((level)*3 ));
				
				num += OctPowSevenDevided[maxLevel - level];
				return (unsigned short)num;
			}
			inline void CreateCollisionObjectList(const unsigned short arg_cellNum, std::vector<std::shared_ptr< CollisionObject<T>>>& arg_output, std::list<std::shared_ptr<  CollisionObject<T>>>& arg_stack) {

				auto objItr = ary_cells[arg_cellNum]->GetHead();
				while (objItr)
				{
					auto rObjItr = objItr->shp_next;
					while (rObjItr != nullptr) {
						// 衝突リスト作成
						arg_output.push_back(objItr->shp_collisionObject);
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
			inline void CreateObjectList(const unsigned short arg_cellNum, std::vector<std::shared_ptr<  CollisionObject<T>>>& arg_output) {

				auto objItr = ary_cells[arg_cellNum]->GetHead();
				while (objItr)
				{
					arg_output.push_back(objItr->shp_collisionObject);
					
					objItr = objItr->shp_next;
				}

				// ③ 子空間に移動
				DWORD ObjNum = 0;
				DWORD i, nextCellNum;
				for (i = 0; i < 8; i++) {
					nextCellNum = arg_cellNum * 8 + 1 + i;
					if (nextCellNum < maxCellNum && ary_cells[arg_cellNum * 8 + 1 + i]) {
						
						CreateObjectList(arg_cellNum * 8 + 1 + i, arg_output);
					}
				}


			}
			inline void CreateCell(unsigned short arg_cellNum) {

				while (!ary_cells[arg_cellNum])
				{
					// 指定の要素番号に空間を新規作成
					ary_cells[arg_cellNum]= new OctCell<T>();

					// 親空間にジャンプ
					arg_cellNum = (arg_cellNum - 1) >> 3;
					if (arg_cellNum >=maxCellNum) break;
				}

			}
			std::vector<std::shared_ptr< OctRegistObj<T>>> vec_shp_collisionObjs;
			std::vector<UINT*>vec_index;
			OctCell<T>** ary_cells;
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