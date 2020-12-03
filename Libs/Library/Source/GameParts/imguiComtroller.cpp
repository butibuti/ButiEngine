#pragma once
#include "stdafx.h"


//imgui

#include "imgui.h"
#include "./backends/imgui_impl_win32.h"
#include "./backends/imgui_impl_dx12.h"

#include "Header/Device/DescriptorHeapManager.h"
#include"Header/Device/GraphicResourceUtil_Dx12.h"
ButiEngine::ImguiController::ImguiController(std::unique_ptr<IWindow>& unq_window, std::shared_ptr<GraphicDevice_Dx12> shp_graphicDevice)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //GUI::StyleColorsClassic();

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

void ButiEngine::ImguiController::Start()
{

    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    befIo = io;
    io = GUI::GetIO();
}

void ButiEngine::ImguiController::Update()
{
}

void ButiEngine::ImguiController::Draw()
{

    ImGui::Render();


    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), &wkp_graphicDevice.lock()->GetCommandList());

}

void ButiEngine::ImguiController::Release()
{
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ButiEngine::ImguiController::SetDraggingObject(std::shared_ptr<IObject> arg_shp_draggingObject)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    shp_draggingObject = arg_shp_draggingObject;
}

void ButiEngine::ImguiController::SetResourceTag(MeshTag arg_tag)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    currentMeshTag = arg_tag;
}

void ButiEngine::ImguiController::SetResourceTag(SoundTag arg_tag)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    currentSoundTag = arg_tag;
}

void ButiEngine::ImguiController::SetResourceTag(MotionTag arg_tag)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    currentMotionTag = arg_tag;
}

void ButiEngine::ImguiController::SetResourceTag(MaterialTag arg_tag)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    currentMaterialTag = arg_tag;
}

void ButiEngine::ImguiController::SetResourceTag(ModelTag arg_tag)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    currentModelTag = arg_tag;
}

void ButiEngine::ImguiController::SetResourceTag(ShaderTag arg_tag)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    currentShaderTag = arg_tag;
}

void ButiEngine::ImguiController::SetResourceTag(VertexShaderTag arg_tag)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    currentVertexShaderTag = arg_tag;
}

void ButiEngine::ImguiController::SetResourceTag(PixelShaderTag arg_tag)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    currentPixelShaderTag = arg_tag;
}

void ButiEngine::ImguiController::SetResourceTag(GeometryShaderTag arg_tag)
{
    if (befIo.MouseDown[0]) {
        return;
    }
    currentGeometryShaderTag = arg_tag;
}
