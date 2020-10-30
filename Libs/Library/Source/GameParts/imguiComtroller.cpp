#pragma once
#include "stdafx.h"
#include "Header/Device/DescriptorHeapManager.h"
//#include "..\..\Header\GameParts\imguiComtroller.h"
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

    wkp_graphicDevice = shp_graphicDevice;
}

void ButiEngine::ButiimguiController::Start()
{

    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
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
