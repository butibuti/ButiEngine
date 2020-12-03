#pragma once
#include"stdafx.h"
#include"Header/Resources/DrawData/IDrawObject.h"
namespace ButiEngine {
	struct DrawLayer {
		void Clear();
		void BefRendering();
		UINT* Regist(std::weak_ptr< IDrawObject> arg_wkp_drawObject, const bool arg_isAfterRendering);
		void UnRegist(UINT* arg_path,const bool arg_isAfterRendering);

		std::vector<std::weak_ptr< IDrawObject>> vec_befDrawObj;
		std::vector<std::weak_ptr< IDrawObject>> vec_afterDrawObj;
		std::vector<UINT*> vec_index;
		std::vector<UINT*> vec_afterIndex;
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
		void MaterialAttach(const MaterialTag& arg_materialTag)override;

		UINT* RegistDrawObject(std::weak_ptr< IDrawObject> arg_wkp_drawObject, const bool arg_afterDraw,const UINT arg_layer=0)override;

		void UnRegistDrawObject(UINT* arg_index, const bool arg_afterDraw, const UINT arg_layer = 0)override;

		void Release()override;
		void ReleaseFogBuffer()override;
		void UpdateFog(const Fog& arg_param)override;
		std::shared_ptr<CBuffer_Dx12<Fog>> GetFogCBuffer()override;
	private:

		inline void ZSort(std::vector < std::weak_ptr< IDrawObject>>& arg_vec_drawObjects) {
			sort(arg_vec_drawObjects.begin(), arg_vec_drawObjects.end(), [](const std::weak_ptr< IDrawObject> pmX, const std::weak_ptr<IDrawObject> pmY) {
				//if(pmX.lock()&&pmY.lock())
				return pmX.lock()->GetZ() > pmY.lock()->GetZ();
			});
		}

		std::weak_ptr<GraphicDevice> wkp_graphicDevice;
		std::weak_ptr<IScene> wkp_iScene;
		std::weak_ptr<IResourceContainer>wkp_resourceContainer;
		std::vector<DrawLayer> vec_drawLayers;
		std::shared_ptr<CBuffer_Dx12<Fog>> CBuffer_fog;
	};
}