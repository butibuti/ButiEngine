#include"stdafx.h"

#include"Header/GameParts/GraphicDevice_Dx12.h"
#include "..\..\Header\Common\Camera_Dx12.h"

ButiEngine::Camera_Dx12::Camera_Dx12(const std::string& arg_cameraName, std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice)
{
	wkp_graphicDevice = arg_wkp_graphicDevice;
	cameraViewProp.top = 0;
	cameraViewProp.left = 0;
	cameraViewProp.width = wkp_graphicDevice.lock()->GetApplication().lock()->GetWindow()->GetSize().x;
	cameraViewProp.height = wkp_graphicDevice.lock()->GetApplication().lock()->GetWindow()->GetSize().x;
	SetName(arg_cameraName);
	cameraViewProp = (CameraProjProperty());
	shp_renderer = arg_shp_renderer;
}

ButiEngine::Camera_Dx12::Camera_Dx12(const CameraProjProperty& arg_cameraViewProp, const std::string& arg_cameraName, std::shared_ptr<IRenderer> arg_shp_renderer, std::weak_ptr<GraphicDevice_Dx12> arg_wkp_graphicDevice)
{
	cameraViewProp = arg_cameraViewProp;
	SetName(arg_cameraName);
	shp_renderer = arg_shp_renderer;
	wkp_graphicDevice = arg_wkp_graphicDevice;
}

ButiEngine::Camera_Dx12::~Camera_Dx12()
{
}

void ButiEngine::Camera_Dx12::Initialize()
{



	if (cameraViewProp.isPararell) {

		projectionMatrix = DirectX::XMMatrixOrthographicOffCenterLH(
			-(float)cameraViewProp.width / 2, (float)cameraViewProp.width / 2,
			-(float)cameraViewProp.height / 2, (float)cameraViewProp.height / 2,
			-50.0f,
			200.0f
		);
	}
	else {
		projectionMatrix =
			DirectX::XMMatrixPerspectiveFovLH(
				DirectX::XMConvertToRadians(60.0f),
				(float)cameraViewProp.width / (float)cameraViewProp.height,
				cameraViewProp.nearClip,
				cameraViewProp.farClip
			);
	}

	projectionMatrix.Transpose();

	viewport.TopLeftX = (float)cameraViewProp.left;
	viewport.TopLeftY = (float)cameraViewProp.top;
	viewport.Width = static_cast<FLOAT>(cameraViewProp.width);
	viewport.Height = static_cast<FLOAT>(cameraViewProp.height);
	viewport.MinDepth = cameraViewProp.front;
	viewport.MaxDepth = cameraViewProp.depth;
}

void ButiEngine::Camera_Dx12::Start()
{
	wkp_graphicDevice.lock()->DrawStart();
	wkp_graphicDevice.lock()->ClearDepthStancil(cameraViewProp.clearDepth);
	viewMatrix = shp_transform->GetMatrix().GetInverse();

	wkp_graphicDevice.lock()->SetCameraPos(shp_transform->GetWorldPosition());
	wkp_graphicDevice.lock()->SetProjectionMatrix(projectionMatrix);
	wkp_graphicDevice.lock()->SetRawViewMatrix(viewMatrix);

	auto transposed = viewMatrix.Transpose();

	wkp_graphicDevice.lock()->SetViewMatrix(transposed);

	auto billboard= transposed;
	billboard._14 = 0.0f;
	billboard._24 = 0.0f;
	billboard._34 = 0.0f;

	(XMMATRIX)billboard.Inverse();

	wkp_graphicDevice.lock()->SetViewMatrix_billBoard(billboard);
	wkp_graphicDevice.lock()->SetViewMatrix_billBoardX(billboard.GetInValidYZ());
	wkp_graphicDevice.lock()->SetViewMatrix_billBoardY(billboard.GetInValidXZ());
	wkp_graphicDevice.lock()->SetViewMatrix_billBoardZ(billboard.GetInValidXY());

	wkp_graphicDevice.lock()->GetCommandList().RSSetViewports(1, &viewport);

}

void ButiEngine::Camera_Dx12::Stop() const
{
	wkp_graphicDevice.lock()->Set();
}

void ButiEngine::Camera_Dx12::ChangeMode(const BlendMode& arg_blendMode)
{
}

void ButiEngine::Camera_Dx12::ShowUI()
{


		//Transform
		if(GUI::TreeNode("Transform"))
		{

			shp_transform->ShowUI();
			std::string target = "BaseTransform:";
			if (shp_transform->GetBaseTransform()) {
				target += "Existence";
			}
			else {
				target += "nullptr";
			}


			GUI::BeginChild("##BaseTransform", Vector2((GUI::GetFontSize()) * (target.size() + 2), GUI::GetFontSize() * 2), true);
			GUI::BulletText((target).c_str());
			if (shp_transform->GetBaseTransform()) {
				GUI::SameLine();
				if (GUI::Button("Detach")) {
					shp_transform->SetBaseTransform(nullptr);
				}
			}
			GUI::SameLine();
			if (GUI::Button("Attach New")) {
				if (!shp_transform->GetBaseTransform())
					shp_transform->SetBaseTransform(ObjectFactory::Create<Transform>(), true);
				else {
					shp_transform->SetBaseTransform(shp_transform->GetBaseTransform()->Clone(), true);
				}
			}
			if (GUI::IsWindowHovered())
			{
				auto obj = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetDraggingObject();

				if (obj && obj->IsThis<GameObject>()) {
					auto trans = obj->GetThis<GameObject>()->transform;
					shp_transform->SetBaseTransform(trans, true);
				}

			}
			GUI::EndChild();
			GUI::TreePop();
		}

		if(GetName()!="main"&&GetName()!="edit")
		GUI::Checkbox("isActive", &isActive);

		bool isEdit = false;


		if (GUI::DragInt2("##projectionSize", &cameraViewProp.width, 1, 0, 100000)) {
			isEdit = true;
		}
		if (
			GUI::DragInt2("##projctionPos", &cameraViewProp.left, 1, 0, 100000)) {
			isEdit = true;
		}
		if (GUI::DragFloat2("##FarClipAndNearClip", &cameraViewProp.farClip, 0.01f, 0.01f, 100000000)) {
			isEdit = true;
		}
		if (GUI::Checkbox("isPararell", &cameraViewProp.isPararell)) {
			isEdit = true;
		}


		GUI::BulletText("ProjectionTexture");
		auto tagName =wkp_graphicDevice.lock()->GetApplication().lock()-> GetResourceContainer()->GetTagNameTexture(cameraViewProp.projectionTexture);
		(GUI::BeginChild("MeshTagWin", Vector2(GUI::GetFontSize() * (tagName.size() + 2), GUI::GetFontSize() * 2), true));
		GUI::Text(Util::ToUTF8(tagName).c_str());

		if (GUI::IsWindowHovered()) {
			auto tag = wkp_graphicDevice.lock()->GetApplication().lock()->GetGUIController()->GetTextureTag();
			if (!tag.IsEmpty()) {
				cameraViewProp.projectionTexture = tag;
			}
		}


		GUI::EndChild();

		if (isEdit) {
			Initialize();
		}


}
