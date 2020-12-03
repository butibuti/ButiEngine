#include "stdafx.h"
#include"Header/Resources/DrawData/ModelDrawData_Dx12.h"
#include"Header/Resources/ModelAnimation.h"
#include "..\..\..\Header\GameObjects\DefaultGameComponent\ModelDrawComponent.h"

#include "..\..\Header\Common\CerealUtill.h"
BUTI_REGIST_GAMECOMPONENT(ButiEngine::ModelDrawComponent);

ButiEngine::ModelDrawComponent::ModelDrawComponent(const ModelTag& arg_modelTag, const ShaderTag& arg_shaderTag, std::shared_ptr< DrawInformation >arg_shp_drawInfo, const UINT arg_layer,  std::shared_ptr<Transform> arg_shp_transform)
{
	layer = arg_layer;
	modelTag = arg_modelTag;
	shaderTag = arg_shaderTag;
	shp_drawInfo= arg_shp_drawInfo;
	shp_transform = arg_shp_transform;
	
	isCereal = false;

}

void ButiEngine::ModelDrawComponent::OnUpdate()
{
#ifdef DEBUG
	if (GameDevice::GetInput()->TriggerKey(Keys::One)) {
		SwitchFillMode(false);
	}
#endif
}
void ButiEngine::ModelDrawComponent::OnSet()
{

	if (!shp_drawInfo) {
		shp_drawInfo = ObjectFactory::Create<DrawInformation>();
		
	}
	if (!isCereal) {
		{
			auto lightBuffer_Dx12 = ObjectFactory::Create<CBuffer_Dx12<LightVariable>>(3);

			shp_drawInfo->vec_exCBuffer.push_back(lightBuffer_Dx12);

			auto light = LightVariable();
			light.lightDir = Vector4(Vector3(-1.0f, -1.0f, 0.0f), 1);

			lightBuffer_Dx12->Get() = light;
		}

	}
	else {

		auto endItr = shp_drawInfo->vec_exCBuffer.end();
		for (auto itr = shp_drawInfo->vec_exCBuffer.begin(); itr != endItr; itr++) {
			(*itr)->Initialize();
		}
	}
	CreateData();

	Regist();
}

std::shared_ptr<ButiEngine::ModelDrawData> ButiEngine::ModelDrawComponent::GetModelData()
{
	return data->GetThis<ModelDrawData>();
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ModelDrawComponent::Clone()
{
	if (shp_drawInfo)
		return ObjectFactory::Create<ModelDrawComponent>(modelTag, shaderTag, shp_drawInfo->Clone(), layer, nullptr);
	else
		return ObjectFactory::Create<ModelDrawComponent>(modelTag, shaderTag, nullptr, layer, nullptr);
}

void ButiEngine::ModelDrawComponent::OnShowUI()
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


		(GUI::BeginChild("ModelTagWin", Vector2(GUI::GetFontSize() * (tagName.size() + 2), GUI::GetFontSize() * 2), true));
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


	if (GUI::TreeNode("DrawSettings")) {

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


		GUI::BulletText("BillBoarsMode");
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
		GUI::TreePop();
	}

}

void ButiEngine::ModelDrawComponent::CreateData()
{
	//auto bone=shp_drawInfo->GetExCBuffer("bone");


	shp_drawInfo->RemoveExCBuffer("bone");

	auto cb_bone_Dx12 = ObjectFactory::Create<CArrayBuffer_Dx12<Matrix4x4>>(4, gameObject.lock()->GetResourceContainer()->GetModel(modelTag).lock()->GetBone().size());
	cb_bone_Dx12->SetExName("bone");
	shp_drawInfo->vec_exCBuffer.push_back(cb_bone_Dx12);
	auto renderer = gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetRenderer();

	if (!shp_drawInfo->IsContainExCBuffer("FogParameter"))
		shp_drawInfo->vec_exCBuffer.push_back(renderer->GetFogCBuffer());
	if (!shp_transform) {
		shp_transform = gameObject.lock()->transform;
	}

	auto modelData = ObjectFactory::Create<ModelDrawData_Dx12>(modelTag, shaderTag, renderer, gameObject.lock()->GetGraphicDevice()->GetThis<GraphicDevice_Dx12>(), shp_drawInfo, shp_transform);

	data = modelData;

	for (auto itr = modelData->vec_bone.begin(); itr != modelData->vec_bone.end(); itr++) {
		if (!((*itr)->parentBone)) {

			(*itr)->transform->SetBaseTransform(shp_transform, true);
		}

	}
	shp_drawInfo->RemoveExCBuffer("FogParameter");
}
