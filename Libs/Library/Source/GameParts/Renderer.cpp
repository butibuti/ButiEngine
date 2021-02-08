#pragma once
#include"stdafx.h"
#include "..\..\Header\GameParts\Renderer.h"
#include"Header/Common/Camera_Dx12.h"
#include"Header/GameParts/CollisionLayer.h"

ButiEngine::Vector2 ComputeFogCoord(float start, float end)
{
	return ButiEngine::Vector2(
		end / (end - start),
		-1.0f / (end - start)
	);
}

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
	CBuffer_fog->Get().fogColor = Vector4(100.0f/256.0f,149.0f/256.0f , 247.0f/256.0f, 0.0f);
	CBuffer_fog->Get().fogCoord = ComputeFogCoord(50,100.0f);
	CBuffer_fog->SetGraphicDevice(wkp_graphicDevice.lock());
	CBuffer_fog->CreateBuffer();

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
	CBuffer_fog->Get().cameraPos = Vector4(wkp_graphicDevice.lock()->GetCameraPos());
	CBuffer_fog->Update();

	{

		auto endDrawItr = vec_drawLayers.at(arg_layer).vec_befDrawObj.end();
		for (auto itr = vec_drawLayers.at(arg_layer).vec_befDrawObj.begin(); itr != endDrawItr; itr++) {
			(*itr)->Draw();
		}
	} 
	{

		auto afterDrawObjects = vec_drawLayers.at(arg_layer).vec_afterDrawObj;
		ZSort(afterDrawObjects);
		auto endDrawItr = afterDrawObjects.end();
		for (auto itr = afterDrawObjects.begin(); itr != endDrawItr; itr++) {
			(*itr)->Draw();
		}
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
		layerItr->BefRendering();

	}
}

void ButiEngine::Renderer::AddLayer()
{
	vec_drawLayers.push_back(DrawLayer(GetThis<IRenderer>()));
}

UINT ButiEngine::Renderer::GetLayerCount()
{
	return vec_drawLayers.size();
}


void ButiEngine::Renderer::ClearDrawObjects()
{
	for (auto layerItr = vec_drawLayers.begin(); layerItr != vec_drawLayers.end(); layerItr++) {
		layerItr->Clear();
	}
	vec_drawLayers.clear();
}



UINT* ButiEngine::Renderer::RegistDrawObject(std::shared_ptr<IDrawObject> arg_wkp_drawObject, const bool arg_afterDraw, const UINT arg_layer)
{

	if (arg_layer >= vec_drawLayers.size()) {
		return nullptr;
	}

	return vec_drawLayers.at(arg_layer).Regist(arg_wkp_drawObject,arg_afterDraw);
}

void ButiEngine::Renderer::UnRegistDrawObject(UINT* arg_index, const bool arg_afterDraw, const UINT arg_layer)
{
	if (arg_layer > vec_drawLayers.size()) {
		return;
	}
	vec_drawLayers.at(arg_layer).UnRegist(arg_index, arg_afterDraw);
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

void ButiEngine::Renderer::MaterialAttach(const UINT slotOffset, const MaterialTag& arg_materialTag)
{
	wkp_resourceContainer.lock()->GetMaterial(arg_materialTag).lock()->Attach(slotOffset, GetThis<Renderer>());
}

std::vector< std::shared_ptr<ButiEngine::IDrawObject>> ButiEngine::Renderer::GetHitDrawObjects(std::shared_ptr<Collision::CollisionPrimitive> arg_prim, const int arg_layer)
{
	std::vector< std::shared_ptr<ButiEngine::IDrawObject>> output;

	if (vec_drawLayers.size() <= arg_layer) {
		return output;
	}

	vec_drawLayers.at(arg_layer).shp_collisionLayer->Check(arg_prim,output);

	return output;
}


void ButiEngine::Renderer::Release()
{

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

ButiEngine::DrawLayer::DrawLayer(std::weak_ptr<IRenderer> arg_wkp_renderer)
{
	wkp_renderer = arg_wkp_renderer;
	shp_collisionLayer = ObjectFactory::Create<Collision::CollisionLayer<IDrawObject>>(5,Vector3(-500, -500, -500),Vector3(500,500,500));
}

void ButiEngine::DrawLayer::Clear()
{
	vec_afterDrawObj.clear();
	vec_befDrawObj.clear();

	for (auto itr = vec_befIndex.begin(); itr != vec_befIndex.end(); itr++) {
		delete* itr;
	}
	for (auto itr = vec_afterIndex.begin(); itr != vec_afterIndex.end(); itr++) {
		delete* itr;
	}
}

void ButiEngine::DrawLayer::BefRendering()
{
	shp_collisionLayer->Update();


	//描画オブジェクトの登録解除
	{
		auto endItr = vec_registCommandBuff.end();
		vec_befDrawObj.reserve(vec_befDrawObj.size() + vec_registCommandBuff.size());
		vec_afterDrawObj.reserve(vec_afterDrawObj.size() + vec_registCommandBuff.size());

		for (auto itr = vec_registCommandBuff.begin(); itr != endItr; itr++) {

			if (itr->shp_obj) {

				itr->shp_obj->CommandSet();
				(itr->shp_obj)->SetOctRegistPtr(shp_collisionLayer->RegistCollisionObj((itr->shp_obj)->GetPrimitive(), itr->shp_obj));

				if (itr->isAfter) {
					vec_afterIndex.push_back(itr->p_index);
					vec_afterDrawObj.push_back(itr->shp_obj);
				}
				else {

					vec_befIndex.push_back(itr->p_index);
					vec_befDrawObj.push_back(itr->shp_obj);
				}

			}
			else {

				DeleteDrawObj(itr->p_index, itr->isAfter);
			}

		}


		vec_registCommandBuff.clear();
		vec_befDrawObj.shrink_to_fit();
		vec_afterDrawObj.shrink_to_fit();

		nowFrameAdditionObjectCount = 0;
		nowFrameAdditionObjectCount_after = 0;
	}


	//描画オブジェクトの行列定数バッファ、ボーン定数バッファの更新
	{

		auto endItr = vec_befDrawObj.end();
		for (auto itr = vec_befDrawObj.begin(); itr != endItr; itr++) {
			(*itr)->DrawBefore();
		}
	}
	{

		auto endItr = vec_afterDrawObj.end();
		for (auto itr = vec_afterDrawObj.begin(); itr != endItr; itr++) {
			(*itr)->DrawBefore();
		}
	}
}

UINT* ButiEngine::DrawLayer::Regist(std::shared_ptr<IDrawObject> arg_wkp_drawObject, const bool arg_isAfterRendering, std::shared_ptr<Collision::CollisionPrimitive_Box_OBB> arg_ret_prim )
{
	if (!arg_ret_prim) {
		arg_ret_prim= arg_wkp_drawObject->GetMeshOBB();
	}
	arg_wkp_drawObject->SetPrimitive(arg_ret_prim);

	UINT* index;
	if (arg_isAfterRendering) {

		index = new UINT(vec_afterDrawObj.size()+nowFrameAdditionObjectCount_after);
		nowFrameAdditionObjectCount_after++;
	}
	else {

		index = new UINT(vec_befDrawObj.size() + nowFrameAdditionObjectCount);
		nowFrameAdditionObjectCount++;
	}

	RegistCommand cmd;

	cmd.isAfter = arg_isAfterRendering;
	cmd.p_index = index;
	cmd.shp_obj = arg_wkp_drawObject;

	vec_registCommandBuff.push_back(cmd);

	return index;
}

void ButiEngine::DrawLayer::UnRegist(UINT* arg_index, const bool arg_isAfterRendering)
{
	if (arg_isAfterRendering) {
		nowFrameAdditionObjectCount_after--;
	}
	else {
		nowFrameAdditionObjectCount--;
	}

	vec_registCommandBuff.push_back({ arg_index,arg_isAfterRendering,nullptr });
}

void ButiEngine::DrawLayer::DeleteDrawObj(UINT* arg_index, const bool arg_isAfterRendering)
{

	if (arg_isAfterRendering) {

		auto index = *arg_index;
		if (index >= vec_afterDrawObj.size()) {
			return;
		}
		auto itr = vec_afterDrawObj.begin() + index;


		shp_collisionLayer->UnRegistCollisionObj((*itr)->GetOctRegistPtr());

		vec_afterDrawObj.erase(itr);

		delete arg_index;
		auto numItr = vec_afterIndex.begin() + index;
		numItr = vec_afterIndex.erase(numItr);

		for (; numItr != vec_afterIndex.end(); numItr++) {
			*(*numItr) -= 1;
		}


		return;
	}
	auto index = *arg_index;
	if (index >= vec_befDrawObj.size()) {
		return;
	}
	auto itr = vec_befDrawObj.begin() + index;
	auto obj = (*itr);
	shp_collisionLayer->UnRegistCollisionObj(obj->GetOctRegistPtr());

	vec_befDrawObj.erase(itr);

	delete arg_index;
	auto numItr = vec_befIndex.begin() + index;
	numItr = vec_befIndex.erase(numItr);

	for (; numItr != vec_befIndex.end(); numItr++) {
		*(*numItr) -= 1;
	}
}
