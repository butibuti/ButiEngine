#pragma once
#include"stdafx.h"
#include"../Resources/DrawData/IDrawObject.h"
namespace ButiEngine {

	namespace Collision {
		template<typename T>
		class CollisionLayer;
	}

	struct RegistCommand {
		UINT* p_index = nullptr;
		bool isAfter;
		std::shared_ptr< IDrawObject> shp_obj = nullptr;
	};
	struct DrawLayer {

		DrawLayer(std::weak_ptr<IRenderer> arg_wkp_renderer);
		void Clear();
		void BefRendering();
		UINT* Regist(std::shared_ptr< IDrawObject> arg_wkp_drawObject, const bool arg_isAfterRendering, std::shared_ptr<Collision::CollisionPrimitive_Box_OBB> arg_ret_pim=nullptr);
		void UnRegist(UINT* arg_path, const bool arg_isAfterRendering);
		void DeleteDrawObj(UINT* arg_path,const bool arg_isAfterRendering);

		std::vector<std::shared_ptr< IDrawObject>> vec_befDrawObj;
		std::vector<std::shared_ptr< IDrawObject>> vec_afterDrawObj;

		std::vector<RegistCommand> vec_registCommandBuff;

		int nowFrameAdditionObjectCount=0;
		int nowFrameAdditionObjectCount_after=0;
		std::vector<UINT*> vec_befIndex;
		std::vector<UINT*> vec_afterIndex;
		std::shared_ptr<Collision::CollisionLayer<IDrawObject>> shp_collisionLayer;
		std::weak_ptr<IRenderer> wkp_renderer;
	};

	class Renderer:public IRenderer
	{
	public:
		Renderer(std::weak_ptr<IScene> arg_wkp_iscene);
		void Initialize()override;
		void Update()override;
		void RenderingStart()override;
		void BefRendering()override;
		void AddLayer()override;
		UINT GetLayerCount() override;
		void Rendering(const UINT arg_layer )override;
		void RenderingEnd()override;
		void ClearDrawObjects()override;
		void Draw(const MeshTag& arg_meshTag)override; 
		std::weak_ptr<IScene> GetScene()override;
		void TextureAttach(const TextureTag& arg_textureTag, const UINT slot)override;
		void ShaderAttach(const ShaderTag& arg_shaderTag)override;
		void MaterialAttach(const UINT slotOffset, const MaterialTag& arg_materialTag)override;

		std::vector< std::shared_ptr<IDrawObject>> GetHitDrawObjects(std::shared_ptr<Collision::CollisionPrimitive> arg_prim,const int arg_layer)override;

		UINT* RegistDrawObject(std::shared_ptr< IDrawObject> arg_wkp_drawObject, const bool arg_afterDraw,const UINT arg_layer=0)override;

		void UnRegistDrawObject(UINT* arg_index, const bool arg_afterDraw, const UINT arg_layer = 0)override;

		void Release()override;
		void ReleaseFogBuffer()override;
		void UpdateFog(const Fog& arg_param)override;
		std::shared_ptr<CBuffer_Dx12<Fog>> GetFogCBuffer()override;
	private:

		inline void ZSort(std::vector < std::shared_ptr< IDrawObject>>& arg_vec_drawObjects) {
			sort(arg_vec_drawObjects.begin(), arg_vec_drawObjects.end(), [](const std::shared_ptr< IDrawObject> pmX, const std::shared_ptr<IDrawObject> pmY) {
				//if(pmX.lock()&&pmY.lock())
				return pmX->GetZ() > pmY->GetZ();
			});
		}

		std::weak_ptr<GraphicDevice> wkp_graphicDevice;
		std::weak_ptr<IScene> wkp_iScene;
		std::weak_ptr<IResourceContainer>wkp_resourceContainer;
		std::vector<DrawLayer> vec_drawLayers;
		std::shared_ptr<CBuffer_Dx12<Fog>> CBuffer_fog;
	};
}