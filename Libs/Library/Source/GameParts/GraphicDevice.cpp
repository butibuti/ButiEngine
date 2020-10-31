#include"stdafx.h"
#include "..\..\Header\GameParts\GraphicDevice.h"


IWICImagingFactory & ButiEngine::GraphicDevice::GetImageFactory()
{
	return *imageFactory.Get();
}


ButiEngine::Matrix4x4 ButiEngine::GraphicDevice::GetProjectionMatrix()
{
	return projectionMatrix;
}


ButiEngine::Matrix4x4 ButiEngine::GraphicDevice::GetCameraViewMatrix()
{
	return viewMatrix;
}

ButiEngine::Matrix4x4 ButiEngine::GraphicDevice::GetRawViewMatrix()
{
	return rawViewMatrix;
}

std::weak_ptr<ButiEngine::Application> ButiEngine::GraphicDevice::GetApplication()
{
	return wkp_application;
}

void ButiEngine::GraphicDevice::SetProjectionMatrix(const Matrix4x4 & arg_projectionMatrix)
{
	projectionMatrix = arg_projectionMatrix;
}


void ButiEngine::GraphicDevice::SetViewMatrix(const Matrix4x4 & arg_viewMatrix)
{
	viewMatrix = arg_viewMatrix;
}

void ButiEngine::GraphicDevice::SetRawViewMatrix(const Matrix4x4& arg_viewMatrix)
{
	rawViewMatrix = arg_viewMatrix;
}

void ButiEngine::GraphicDevice::SetViewMatrix_billBoard(const Matrix4x4& arg_viewMatrix)
{
	viewMatrix_billBoard = arg_viewMatrix;
}

void ButiEngine::GraphicDevice::SetViewMatrix_billBoardX(const Matrix4x4& arg_viewMatrix)
{
	viewMatrix_billBoardX = arg_viewMatrix;
}

void ButiEngine::GraphicDevice::SetViewMatrix_billBoardY(const Matrix4x4& arg_viewMatrix)
{
	viewMatrix_billBoardY = arg_viewMatrix;
}

void ButiEngine::GraphicDevice::SetViewMatrix_billBoardZ(const Matrix4x4& arg_viewMatrix)
{
	viewMatrix_billBoardZ = arg_viewMatrix;
}

void ButiEngine::GraphicDevice::SetClearColor(const Vector4& arg_clearColor)
{
	color = arg_clearColor;
	color.GetData(clearColor);
}

ButiEngine::Vector4 ButiEngine::GraphicDevice::GetClearColor()
{
	return color;
}
