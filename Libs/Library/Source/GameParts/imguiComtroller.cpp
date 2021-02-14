#pragma once
#include "stdafx.h"
#include"../../Header/GameParts/imguiComtroller.h"


//imgui

#include "imgui.h"
#include "./backends/imgui_impl_win32.h"
#include "./backends/imgui_impl_dx12.h"

#include "Header/Device/DescriptorHeapManager.h"
#include"Header/Device/GraphicResourceUtil_Dx12.h"
#include "..\..\Header\GameParts\imguiComtroller.h"

using namespace ButiEngine;


ImguiController::ImguiController(std::unique_ptr<IWindow>& unq_window, std::shared_ptr<GraphicDevice_Dx12> shp_graphicDevice)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
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

    unq_instance = std::make_unique<ImguiController::ImguiControllerInstance>();

    unq_instance->wkp_graphicDevice = shp_graphicDevice;
}

void ImguiController::Start()
{

    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    unq_instance->befIo = unq_instance->io;
    unq_instance->io = GUI::GetIO();

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags =  ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_NoResize |  ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    
    ImGui::SetNextWindowBgAlpha(0.0f);
    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar(3);
    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
    
    ImGuiDockNodeFlags dockspace_flags = GUI::GuiDockNodeFlags_NoSplit;

    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    ImGui::End();

}

void ImguiController::Update()
{
    auto endItr = vec_shp_GUIObjects.end();
    int size = vec_shp_GUIObjects.size();
    for (auto itr = vec_shp_GUIObjects.begin(); itr != endItr; itr++) {
        (*itr)->ShowGUI();
    }

    vec_shp_GUIObjects.clear();
    vec_shp_GUIObjects.reserve(size);
}

void ImguiController::Draw()
{

    ImGui::Render();


    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), &unq_instance->wkp_graphicDevice.lock()->GetCommandList());

}

void ImguiController::Release()
{
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImguiController::SetDraggingObject(std::shared_ptr<IObject> arg_shp_draggingObject)
{
    if (unq_instance->befIo.MouseDown[0]) {
        return;
    }
    unq_instance->shp_draggingObject = arg_shp_draggingObject;
}

void ImguiController::SetResourceTag(MeshTag arg_tag)
{
    if (unq_instance->befIo.MouseDown[0]) {
        return;
    }
    unq_instance->currentMeshTag = arg_tag;
}

void ImguiController::SetResourceTag(SoundTag arg_tag)
{
    if (unq_instance->befIo.MouseDown[0]) {
        return;
    }
    unq_instance->currentSoundTag = arg_tag;
}

void ImguiController::SetResourceTag(MotionTag arg_tag)
{
    if (unq_instance->befIo.MouseDown[0]) {
        return;
    }
    unq_instance->currentMotionTag = arg_tag;
}

void ImguiController::SetResourceTag(MaterialTag arg_tag)
{
    if (unq_instance->befIo.MouseDown[0]) {
        return;
    }
    unq_instance->currentMaterialTag = arg_tag;
}

void ImguiController::SetResourceTag(ModelTag arg_tag)
{
    if (unq_instance->befIo.MouseDown[0]) {
        return;
    }
    unq_instance->currentModelTag = arg_tag;
}

void ImguiController::SetResourceTag(ShaderTag arg_tag)
{
    if (unq_instance->befIo.MouseDown[0]) {
        return;
    }
    unq_instance->currentShaderTag = arg_tag;
}

void ImguiController::SetResourceTag(VertexShaderTag arg_tag)
{
    if (unq_instance->befIo.MouseDown[0]) {
        return;
    }
    unq_instance->currentVertexShaderTag = arg_tag;
}

void ImguiController::SetResourceTag(PixelShaderTag arg_tag)
{
    if (unq_instance->befIo.MouseDown[0]) {
        return;
    }
    unq_instance->currentPixelShaderTag = arg_tag;
}

void ImguiController::SetResourceTag(GeometryShaderTag arg_tag)
{
    if (unq_instance->befIo.MouseDown[0]) {
        return;
    }
    unq_instance->currentGeometryShaderTag = arg_tag;
}

void ImguiController::SetResourceTag(TextureTag arg_tag)
{
    if (unq_instance->befIo.MouseDown[0]) {
        return;
    }
    unq_instance->currentTextureTag = arg_tag;
}

void ButiEngine::ImguiController::SetGUIObject(std::shared_ptr<IObject> arg_obj)
{
    vec_shp_GUIObjects.push_back(arg_obj);
}

std::shared_ptr<IObject> ImguiController::GetDraggingObject()
{
    return unq_instance->shp_draggingObject;
}

MeshTag ImguiController::GetMeshTag()
{
    return unq_instance->currentMeshTag;
}

SoundTag ImguiController::GetSoundTag()
{
    return unq_instance->currentSoundTag;
}

MotionTag ImguiController::GetMotionTag()
{
    return unq_instance->currentMotionTag;
}

MaterialTag ImguiController::GetMaterialTag()
{
    return unq_instance->currentMaterialTag;
}

ModelTag ImguiController::GetModelTag()
{
    return unq_instance->currentModelTag;
}

ShaderTag ImguiController::GetShaderTag()
{
    return unq_instance->currentShaderTag;
}

VertexShaderTag ImguiController::GetVertexShaderTag()
{
    return unq_instance->currentVertexShaderTag;
}

PixelShaderTag ImguiController::GetPixelShaderTag()
{
    return unq_instance->currentPixelShaderTag;
}

GeometryShaderTag ImguiController::GetGeometryShaderTag()
{
    return unq_instance->currentGeometryShaderTag;
}

TextureTag ImguiController::GetTextureTag()
{
    return unq_instance->currentTextureTag;
}

GUI::GuiIO& ImguiController::GetGUIIO()
{
    return unq_instance->io;
}
