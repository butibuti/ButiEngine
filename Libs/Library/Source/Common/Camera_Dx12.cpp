#include"stdafx.h"

#include"Header/Common/Camera_Dx12.h"

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
