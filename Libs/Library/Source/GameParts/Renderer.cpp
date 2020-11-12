#pragma once
#include"stdafx.h"
#include "..\..\Header\GameParts\Renderer.h"
#include"Header/GameParts/ResourceContainer.h"

ButiEngine::Renderer::Renderer(std::weak_ptr<IScene> arg_wkp_iscene)
{
	wkp_iScene = arg_wkp_iscene;
}
void ButiEngine::Renderer::Initialize()
{
	
	wkp_resourceContainer =wkp_iScene.lock()->GetSceneManager().lock()->GetApplication().lock()->GetResourceContainer();
	wkp_graphicDevice= wkp_iScene.lock()->GetSceneManager().lock()->GetApplication().lock()->GetGraphicDevice();

	
	CBuffer_fog = ObjectFactory::Create<CBuffer_Dx12<Fog>>(2);

	CBuffer_fog->SetExName("FogParameter");
	CBuffer_fog->SetGraphicDevice(wkp_graphicDevice.lock());
	CBuffer_fog->CreateBuffer();
	AddLayer();
}
void ButiEngine::Renderer::Update()
{
	wkp_graphicDevice.lock()->Update();
}

void ButiEngine::Renderer::RenderingStart()
{
	wkp_graphicDevice.lock()->ClearWindow();
	wkp_graphicDevice.lock()->ResourceUpload();
}

void ButiEngine::Renderer::Rendering(const UINT arg_layer)
{
	auto drawObjects = vec_drawLayers.at(arg_layer);
	ZSort(drawObjects);
	auto endDrawItr = drawObjects.end();
	for (auto itr = drawObjects.begin(); itr != endDrawItr; itr++) {
		(*itr).lock()->Draw();
	}
	
	
}


void ButiEngine::Renderer::RenderingEnd()
{
	wkp_graphicDevice.lock()->Update();

	wkp_graphicDevice.lock()->ResourceUploadRelease();
}

void ButiEngine::Renderer::BefRendering()
{
	for (auto layerItr = vec_drawLayers.begin(); layerItr != vec_drawLayers.end(); layerItr++) {
		for(auto objItr = (layerItr)->begin(); objItr != (layerItr)->end();objItr++)
		(objItr)->lock()->DrawBefore();
	}
}

void ButiEngine::Renderer::AddLayer()
{
	vec_drawLayers.push_back(std::vector<std::weak_ptr< IDrawObject>>());
	vec_index.push_back(std::vector<UINT*>());
}

UINT ButiEngine::Renderer::GetLayerCount()
{
	return vec_drawLayers.size();
}


void ButiEngine::Renderer::ClearDrawObjects()
{
	for (auto layerItr = vec_drawLayers.begin(); layerItr != vec_drawLayers.end(); layerItr++) {
		layerItr->clear();
	}
	vec_drawLayers.clear();
}



UINT* ButiEngine::Renderer::RegistDrawObject(std::weak_ptr<IDrawObject> arg_wkp_drawObject, const UINT arg_layer)
{

	if (arg_layer > vec_drawLayers.size()) {
		return nullptr;
	}
	UINT* index = new UINT(vec_drawLayers.at(arg_layer).size());

	vec_drawLayers.at(arg_layer).push_back(arg_wkp_drawObject);
	vec_index.at(arg_layer).push_back(index);

	return index;
}

void ButiEngine::Renderer::UnRegistDrawObject(UINT* arg_index, const UINT arg_layer)
{
	if (arg_layer > vec_drawLayers.size()) {
		return;
	}
	auto index = *arg_index;
	if (index >= vec_drawLayers.at(arg_layer).size()) {
		return;
	}
	auto itr = vec_drawLayers.at(arg_layer).begin()+index;
	

	vec_drawLayers.at(arg_layer).erase(itr);

	delete arg_index;
	auto numItr = vec_index.at(arg_layer).begin() + index;
	numItr = vec_index.at(arg_layer).erase(numItr);

	for (; numItr != vec_index.at(arg_layer).end(); numItr++) {
		*(*numItr) -= 1;
	}
}
void ButiEngine::Renderer::Draw(const MeshTag& arg_meshTag)
{
	wkp_resourceContainer.lock()->GetMesh(arg_meshTag).lock()->Draw();
}

std::weak_ptr< ButiEngine::IScene> ButiEngine::Renderer::GetScene()
{
	return wkp_iScene;
}

void ButiEngine::Renderer::TextureAttach(const TextureTag& arg_textureTag, const UINT slot)
{
	wkp_resourceContainer.lock()->GetTexture(arg_textureTag).lock()->Attach(slot);
}

void ButiEngine::Renderer::ShaderAttach(const ShaderTag& arg_shaderTag)
{
	wkp_resourceContainer.lock()->GetShader(arg_shaderTag).lock()->Attach();
}

void ButiEngine::Renderer::MaterialAttach(const MaterialTag& arg_materialTag)
{
	wkp_resourceContainer.lock()->GetMaterial(arg_materialTag).lock()->Attach(GetThis<Renderer>());
}


void ButiEngine::Renderer::Release()
{
	for (auto layerItr = vec_index.begin(); layerItr != vec_index.end(); layerItr++) {
		for (auto indexItr = layerItr->begin(); indexItr != layerItr->end(); indexItr++) {
			delete* indexItr;
		}
		layerItr->clear();
	}
	vec_index.clear();

	ClearDrawObjects();
}

void ButiEngine::Renderer::ReleaseFogBuffer()
{
	CBuffer_fog = nullptr;
}

void ButiEngine::Renderer::UpdateFog(const Fog &arg_param)
{
	CBuffer_fog->Get().fogColor = arg_param.fogColor;
	CBuffer_fog->Get().fogCoord = arg_param.fogCoord;

}

std::shared_ptr< ButiEngine::CBuffer_Dx12< ButiEngine::Fog>> ButiEngine::Renderer::GetFogCBuffer()
{
	return CBuffer_fog;
}

