#pragma once
#include"stdafx.h"
#include"Header/Resources/DrawData/IDrawObject.h"
namespace ButiEngine {

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
		UINT* RegistDrawObject(std::weak_ptr< IDrawObject> arg_wkp_drawObject,const UINT arg_layer=0)override;

		void UnRegistDrawObject(UINT* arg_index, const UINT arg_layer = 0)override;

		void Release()override;
	private:

		inline void ZSort(std::vector < std::weak_ptr< IDrawObject>>& arg_vec_drawObjects) {
			sort(arg_vec_drawObjects.begin(), arg_vec_drawObjects.end(), [](const std::weak_ptr< IDrawObject> pmX, const std::weak_ptr<IDrawObject> pmY) {
				//if(pmX.lock()&&pmY.lock())
				return pmX.lock()->GetZ() > pmY.lock()->GetZ();
			});
		}

		std::vector< std::vector<UINT*>>vec_index;
		std::weak_ptr<GraphicDevice> wkp_graphicDevice;
		std::weak_ptr<IScene> wkp_iScene;
		std::weak_ptr<ResourceContainer>wkp_resourceContainer;
		std::vector< std::vector<std::weak_ptr< IDrawObject>>> vec_drawLayers;
	};
}