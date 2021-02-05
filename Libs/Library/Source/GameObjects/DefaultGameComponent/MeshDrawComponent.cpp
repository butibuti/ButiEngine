#pragma once
#include"stdafx.h"
#include"Header/Resources/DrawData/DrawData_Dx12.h"
#include"Header/Common/CollisionPrimitive.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\MeshDrawComponent.h"

ButiEngine::MeshDrawComponent::MeshDrawComponent(const MeshTag& arg_meshTag, const ShaderTag& arg_shaderTag, const MaterialTag& arg_materialTag, std::shared_ptr<DrawInformation> arg_shp_drawInfo, const UINT arg_layer, std::shared_ptr<Transform> arg_shp_transform)
{
	layer = arg_layer;
	meshTag = arg_meshTag;
	shaderTag = arg_shaderTag;
	materialTag.push_back(arg_materialTag);

	shp_transform = arg_shp_transform;
	shp_drawInfo = arg_shp_drawInfo;
	isCereal = false;
}

ButiEngine::MeshDrawComponent::MeshDrawComponent(const MeshTag& arg_meshTag, const ShaderTag& arg_shaderTag, const std::vector<MaterialTag>& arg_materialTag, std::shared_ptr<DrawInformation> arg_shp_drawInfo, const UINT arg_layer, std::shared_ptr<Transform> arg_shp_transform)
{
	layer = arg_layer;
	meshTag = arg_meshTag;
	shaderTag = arg_shaderTag;
	materialTag = (arg_materialTag);
	shp_drawInfo = arg_shp_drawInfo;
	if (arg_shp_transform) {
		shp_transform = arg_shp_transform;
	}
	isCereal = false;
}

ButiEngine::MeshDrawComponent::MeshDrawComponent(const ModelTag& arg_modelTag, const ShaderTag& arg_shaderTag, std::shared_ptr<DrawInformation> arg_shp_drawInfo, const UINT arg_layer, std::shared_ptr<Transform> arg_shp_transform)
{
	layer = arg_layer;
	modelTag = arg_modelTag;
	shaderTag = arg_shaderTag;
	shp_drawInfo = arg_shp_drawInfo;
	if (arg_shp_transform) {
		shp_transform = arg_shp_transform;
	}
	isCereal = false;
}

void ButiEngine::MeshDrawComponent::OnUpdate()
{
}

void ButiEngine::MeshDrawComponent::ChangeFillMode(const bool arg_isFill)
{
	data->ChangeFillMode(arg_isFill);
}

void ButiEngine::MeshDrawComponent::SwitchFillMode(const bool arg_isFill)
{
	data->ChangeSwitchFillMode();
}

void ButiEngine::MeshDrawComponent::ChangeCullMode(const CullMode& arg_cullMode)
{
	data->ChangeCullMode(arg_cullMode);
}

void ButiEngine::MeshDrawComponent::OnSet()
{
	if (!shp_drawInfo) {
		shp_drawInfo = ObjectFactory::Create<DrawInformation>();
	}

	if (!isCereal)
	{
		if (!shp_drawInfo->GetExCBuffer("LightBuffer")) {
			auto lightBuffer_Dx12 = ObjectFactory::Create<CBuffer_Dx12<LightVariable>>(3);

			shp_drawInfo->vec_exCBuffer.push_back(lightBuffer_Dx12);

			auto light = LightVariable();
			light.lightDir = Vector4(Vector3(-1.0f, -1.0f, 0.0f), 1);
			lightBuffer_Dx12->SetExName("LightBuffer");
			lightBuffer_Dx12->Get() = light;
		}
	}
	else {

		auto graphicDevice = gameObject.lock()->GetGraphicDevice();
		meshTag = graphicDevice->GetApplication().lock()->GetResourceContainer()->GetMeshTag(meshTag);
		shaderTag = graphicDevice->GetApplication().lock()->GetResourceContainer()->GetShaderTag(shaderTag);
		modelTag = graphicDevice->GetApplication().lock()->GetResourceContainer()->GetModelTag(modelTag);

		auto matEnd = materialTag.end();
		for (auto itr = materialTag.begin(); itr != matEnd; itr++) {
			(*itr)= graphicDevice->GetApplication().lock()->GetResourceContainer()->GetMaterialTag(*itr);
		}

		auto endItr = shp_drawInfo->vec_exCBuffer.end();
		for (auto itr = shp_drawInfo->vec_exCBuffer.begin(); itr != endItr; itr++) {
			(*itr)->Initialize();
		}
	}

	CreateData();

	Regist();
}

void ButiEngine::MeshDrawComponent::OnRemove()
{
	if (index)
	gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetRenderer()->UnRegistDrawObject(index,shp_drawInfo->isAlpha,layer);
}

void ButiEngine::MeshDrawComponent::SetBlendMode(const BlendMode& arg_blendMode)
{
	data->SetBlendMode(arg_blendMode);
}

void ButiEngine::MeshDrawComponent::SetMaterialTag(MaterialTag arg_materialTag)
{
	materialTag.clear();
	materialTag.push_back(arg_materialTag);
}

void ButiEngine::MeshDrawComponent::SetMeshTag(MeshTag arg_meshTag)
{
	meshTag = arg_meshTag;
}

void ButiEngine::MeshDrawComponent::SetModelTag(ModelTag arg_modelTag)
{
	modelTag = arg_modelTag;
}

void ButiEngine::MeshDrawComponent::SetShaderTag(ShaderTag arg_shaderTag)
{
	shaderTag = arg_shaderTag;
}

std::shared_ptr<ButiEngine:: GameComponent> ButiEngine::MeshDrawComponent::Clone()
{
	if (shp_drawInfo) {
		if (modelTag.IsEmpty()) {
			return ObjectFactory::Create<MeshDrawComponent>(meshTag, shaderTag, materialTag, shp_drawInfo->Clone(), layer, nullptr);
		}
		else {
			return ObjectFactory::Create<MeshDrawComponent>(modelTag, shaderTag, shp_drawInfo->Clone(), layer, nullptr);
		}
	}
	else {

		if (modelTag.IsEmpty()) {
			return ObjectFactory::Create<MeshDrawComponent>(meshTag, shaderTag, materialTag, nullptr, layer, nullptr);
		}
		else {
			return ObjectFactory::Create<MeshDrawComponent>(modelTag, shaderTag, nullptr, layer, nullptr);
		}
	}
}

void ButiEngine::MeshDrawComponent::Regist()
{
	if (!index) {

		index = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetRenderer()->RegistDrawObject(data, shp_drawInfo->isAlpha, layer);

		data->GetPrimitive()->Update();
	}}

void ButiEngine::MeshDrawComponent::ReRegist()
{

	UnRegist();
	CreateData();
	Regist();
}

void ButiEngine::MeshDrawComponent::UnRegist()
{
	if (index) {
		gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetRenderer()->UnRegistDrawObject(index, shp_drawInfo->isAlpha, layer);
		index = nullptr;
	}
}

std::shared_ptr<ButiEngine::DrawInformation> ButiEngine::MeshDrawComponent::GetDrawInformation()
{
	return shp_drawInfo;
}

void ButiEngine::MeshDrawComponent::OnShowUI()
{
	if (GUI::Button("Regist")) {
		Regist();
	}
	GUI::SameLine();
	if (GUI::Button("UnRegist")) {
		UnRegist();
	}
	GUI::SameLine();
	if (GUI::Button("ReRegist")) {
		ReRegist();
	}

	{
		GUI::BulletText("ModelTag");
		

		auto tagName = gameObject.lock()->GetResourceContainer()->GetTagNameModel(modelTag);

		if (!modelTag.IsEmpty())
			if (GUI::Button("RemoveModelTag"))
			{
				auto resourceContainer = gameObject.lock()->GetResourceContainer();
				meshTag = resourceContainer->GetModel(modelTag).lock()->GetMeshTag();
				materialTag = gameObject.lock()->GetResourceContainer()->GetModel(modelTag).lock()->GetMaterialTags();
				modelTag = ModelTag();
			}

		GUI::BeginChild("ModelTagWin", Vector2(GUI::GetFontSize() * (tagName.size() + 2), GUI::GetFontSize() * 2), true);
			GUI::Text(Util::ToUTF8(tagName).c_str());

			if (GUI::IsWindowHovered()) {
				auto tag = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->GetApplication().lock()->GetGUIController()->GetModelTag();
				if (!tag.IsEmpty()) {
					modelTag = tag;
					ReRegist();
				}
			}
		

		GUI::EndChild();
	}
	{
		GUI::BulletText("MeshTag");
		auto tagName = gameObject.lock()->GetResourceContainer()->GetTagNameMesh(meshTag);
		(GUI::BeginChild("MeshTagWin", Vector2(GUI::GetFontSize() * (tagName.size() + 2), GUI::GetFontSize() * 2), true));
			GUI::Text(Util::ToUTF8(tagName).c_str());

			if (GUI::IsWindowHovered()) {
				auto tag = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->GetApplication().lock()->GetGUIController()->GetMeshTag();
				if (!tag.IsEmpty()) {
					meshTag = tag;
					ReRegist();
				}
			}
		
		
		GUI::EndChild();
	}
	{
		GUI::BulletText("MaterialTag");
		for (int i = 0; i < materialTag.size(); i++) {
			auto tagName = gameObject.lock()->GetResourceContainer()->GetTagNameMaterial(materialTag[i]);
			(GUI::BeginChild("MaterialTagWin" + i, Vector2(GUI::GetFontSize() * (tagName.size()), GUI::GetFontSize() * 2), true));
				GUI::Text(Util::ToUTF8(tagName).c_str());

				if (GUI::IsWindowHovered()) {
					auto tag = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->GetApplication().lock()->GetGUIController()->GetMaterialTag();
					if (!tag.IsEmpty()) {
						materialTag[i] = tag;
						ReRegist();
					}
				}
				GUI::EndChild();
			
		}
	}
	{
		GUI::BulletText("ShaderTag");
		auto tagName = gameObject.lock()->GetResourceContainer()->GetTagNameShader(shaderTag);
		(GUI::BeginChild("ShaderTagWin", Vector2(GUI::GetFontSize() * (tagName.size() + 2), GUI::GetFontSize() * 2), true));
			GUI::Text(Util::ToUTF8(tagName).c_str());

			if (GUI::IsWindowHovered()) {
				auto tag = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->GetApplication().lock()->GetGUIController()->GetShaderTag();
				if (!tag.IsEmpty()) {
					shaderTag = tag;
					ReRegist();
				}
			}
			GUI::EndChild();
		

	}

	if (GUI::ArrowButton("##layerUp",GUI::GuiDir_Left)) {

		UnRegist();
		layer--;
		if (layer < 0) {
			layer = 0;
		}
		Regist();
	}
	GUI::SameLine();
	GUI::Text(std::to_string(layer));
	GUI::SameLine();
	if (GUI::ArrowButton("##layerDown", GUI::GuiDir_Right)) {
		UnRegist();
		layer++;
		Regist();
	}


	if (GUI::TreeNode("DrawSettings")) {

		GUI::BulletText("IsAlphaObject");
		if (GUI::Checkbox("##isAlpha", &shp_drawInfo->isAlpha)) {
			shp_drawInfo->isAlpha = !shp_drawInfo->isAlpha;
			UnRegist();
			shp_drawInfo->isAlpha = !shp_drawInfo->isAlpha;

			Regist();
		}

		GUI::BulletText("TopologyMode");

		if (GUI::Button("Triangle")) {
			shp_drawInfo->drawSettings.topologyType = TopologyType::triangle;
			ReRegist();
		}
		GUI::SameLine();
		if (GUI::Button("TriangleList")) {
			shp_drawInfo->drawSettings.topologyType = TopologyType::triangleList;
			ReRegist();
		}
		GUI::SameLine();
		if (GUI::Button("Line")) {
			shp_drawInfo->drawSettings.topologyType = TopologyType::line;
			ReRegist();
		}
		GUI::SameLine();
		if (GUI::Button("Point")) {
			shp_drawInfo->drawSettings.topologyType = TopologyType::point;
			ReRegist();
		}

		GUI::BulletText("BlendMode");

		if (GUI::Button("Addition")) {
			shp_drawInfo->drawSettings.blendMode = BlendMode::Addition;
			ReRegist();
		}
		GUI::SameLine();
		if (GUI::Button("Blend")) {
			shp_drawInfo->drawSettings.blendMode = BlendMode::AlphaBlend;
			ReRegist();
		}
		GUI::SameLine();
		if (GUI::Button("None")) {
			shp_drawInfo->drawSettings.blendMode = BlendMode::NoBlend;
			ReRegist();
		}
		GUI::SameLine();
		if (GUI::Button("Reverse")) {
			shp_drawInfo->drawSettings.blendMode = BlendMode::Reverse;
			ReRegist();
		}
		GUI::BulletText("Culling");
		if (GUI::Button("None##Culling")) {
			shp_drawInfo->drawSettings.cullMode = CullMode::none;
			ReRegist();
		}
		GUI::SameLine();
		if (GUI::Button("Front##Culling")) {
			shp_drawInfo->drawSettings.cullMode = CullMode::front;
			ReRegist();
		}
		GUI::SameLine();
		if (GUI::Button("Back##Culling")) {
			shp_drawInfo->drawSettings.cullMode = CullMode::back;
			ReRegist();
		}

		GUI::BulletText("BillBoardMode");
		if (GUI::Button("None")) {
			shp_drawInfo->drawSettings.billboardMode = BillBoardMode::none;
			ReRegist();
		}
		GUI::SameLine();
		if (GUI::Button("BillBoard")) {
			shp_drawInfo->drawSettings.billboardMode = BillBoardMode::full;
			ReRegist();
		}
		GUI::SameLine();
		if (GUI::Button("BillBoard_X")) {
			shp_drawInfo->drawSettings.billboardMode = BillBoardMode::x;
			ReRegist();
		}
		GUI::SameLine();
		if (GUI::Button("BillBoard_Y")) {
			shp_drawInfo->drawSettings.billboardMode = BillBoardMode::y;
			ReRegist();
		}
		GUI::SameLine();
		if (GUI::Button("BillBoard_Z")) {
			shp_drawInfo->drawSettings.billboardMode = BillBoardMode::z;
			ReRegist();
		}
		if (GUI::Checkbox("Use Deppth", &shp_drawInfo->isDepth)) {
		}

		GUI::TreePop();
	}
	if (GUI::TreeNode("ExCBuffer")) {
		auto endItr = shp_drawInfo->vec_exCBuffer.end();
		for (auto itr = shp_drawInfo->vec_exCBuffer.begin(); itr != endItr; itr++) {
			(*itr)->ShowUI();
		}

		static int slot=0;

		if (GUI::Button("Add BlurParam")) {
			auto shp_gausBuffer = ObjectFactory::Create<CBuffer_Dx12<GausVariable>>(4);
			shp_gausBuffer->SetExName("GausParam");
			auto size = gameObject.lock()->GetApplication().lock()->GetWindow()->GetSize();
			shp_gausBuffer->Get().CalcGaus(size.x,size.y,Vector2(),2.0);

			shp_drawInfo->vec_exCBuffer.push_back(shp_gausBuffer);
			ReRegist();
		}


		GUI::TreePop();
	}


}

void ButiEngine::MeshDrawComponent::CreateData()
{
	auto renderer = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetRenderer();
	if(!shp_drawInfo->IsContainExCBuffer("FogParameter"))
	shp_drawInfo->vec_exCBuffer.push_back(renderer->GetFogCBuffer());
	if (!shp_transform) {
		shp_transform = gameObject.lock()->transform;
	}
	if (!modelTag.IsEmpty()) {
		meshTag = MeshTag ();
		materialTag.clear();
		auto graphicDevice = gameObject.lock()->GetGraphicDevice()->GetThis<GraphicDevice_Dx12>();
		data = ObjectFactory::Create<MeshDrawData_Dx12>(modelTag, shaderTag, renderer, graphicDevice, shp_drawInfo, shp_transform);

	}
	else {
		if(!materialTag.size())
		materialTag .push_back( MaterialTag());
		data = ObjectFactory::Create<MeshDrawData_Dx12>(meshTag, shaderTag, materialTag, renderer, gameObject.lock()->GetGraphicDevice()->GetThis<GraphicDevice_Dx12>(), shp_drawInfo, shp_transform);


	}
	shp_drawInfo->RemoveExCBuffer("FogParameter");

}
