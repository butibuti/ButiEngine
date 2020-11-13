#pragma once
#include "stdafx.h"
#include "Header/Device/DescriptorHeapManager.h"
#include"Header/Device/GraphicResourceUtil_Dx12.h"
ButiEngine::ButiimguiController::ButiimguiController(std::unique_ptr<Window>& unq_window, std::shared_ptr<GraphicDevice_Dx12> shp_graphicDevice)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(unq_window->GetHandle());
    auto handle = shp_graphicDevice->GetDescriptorHeapManager().lock()->GetNowHandle();
    ImGui_ImplDX12_Init(&shp_graphicDevice->GetDevice(), shp_graphicDevice->GetFrameCount(),
        DXGI_FORMAT_R8G8B8A8_UNORM, shp_graphicDevice->GetDescriptorHeapManager().lock()->GetDescriptorHeap().Get(),
        handle.CPUHandle,
        handle.GPUHandle);

    io.Fonts->AddFontFromFileTTF((GlobalSettings::GetResourceDirectory()+ "Font\\meiryo.ttc").c_str(), 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

    wkp_graphicDevice = shp_graphicDevice;
}

void ButiEngine::ButiimguiController::Start()
{

    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    befIo = io;
    io = ImGui::GetIO();
}

void ButiEngine::ButiimguiController::Update()
{
}

void ButiEngine::ButiimguiController::Draw()
{

    ImGui::Render();


    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), &wkp_graphicDevice.lock()->GetCommandList());

}

void ButiEngine::ButiimguiController::Release()
{
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ButiEngine::ButiimguiController::SetDraggingObject(std::shared_ptr<IObject> arg_shp_draggingObject)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    shp_draggingObject = arg_shp_draggingObject;
}

void ButiEngine::ButiimguiController::SetResourceTag(MeshTag arg_tag)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    currentMeshTag = arg_tag;
}

void ButiEngine::ButiimguiController::SetResourceTag(SoundTag arg_tag)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    currentSoundTag = arg_tag;
}

void ButiEngine::ButiimguiController::SetResourceTag(MotionTag arg_tag)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    currentMotionTag = arg_tag;
}

void ButiEngine::ButiimguiController::SetResourceTag(MaterialTag arg_tag)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    currentMaterialTag = arg_tag;
}

void ButiEngine::ButiimguiController::SetResourceTag(ModelTag arg_tag)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    currentModelTag = arg_tag;
}

void ButiEngine::ButiimguiController::SetResourceTag(ShaderTag arg_tag)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    currentShaderTag = arg_tag;
}

void ButiEngine::ButiimguiController::SetResourceTag(VertexShaderTag arg_tag)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    currentVertexShaderTag = arg_tag;
}

void ButiEngine::ButiimguiController::SetResourceTag(PixelShaderTag arg_tag)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    currentPixelShaderTag = arg_tag;
}

void ButiEngine::ButiimguiController::SetResourceTag(GeometryShaderTag arg_tag)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    currentGeometryShaderTag = arg_tag;
}
