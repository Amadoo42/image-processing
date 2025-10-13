#pragma once
#include "FiltersMenu.h"
#include "LoadTexture.h"
#include "MemoryOperation.h"
#include "CompareView.h"

static bool is_dark_theme = true;
static float zoom_level = 1.0f;
static ImVec2 pan_offset = ImVec2(0, 0);
static bool compareMode = false;
static GLuint currentTextureID = 0;
bool textureNeedsUpdate = false;

void setModernStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    style.WindowRounding = 5.0f;
    style.FrameRounding = 5.0f;
    style.PopupRounding = 5.0f;
    style.ScrollbarRounding = 9.0f;
    style.GrabRounding = 5.0f;
    style.TabRounding = 5.0f;

    style.Colors[ImGuiCol_Text]                  = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_ChildBg]               = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.18f, 0.18f, 0.18f, 0.94f);
    style.Colors[ImGuiCol_Border]                = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.25f, 0.26f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.30f, 0.31f, 0.32f, 1.00f);
    style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.04f, 0.04f, 0.04f, 0.70f);
    style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.18f, 0.18f, 0.18f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.30f, 0.31f, 0.32f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.40f, 0.41f, 0.42f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.50f, 0.51f, 0.52f, 1.00f);
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Button]                = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Header]                = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Separator]             = style.Colors[ImGuiCol_Border];
    style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive]       = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_Tab]                   = ImLerp(style.Colors[ImGuiCol_Header],       style.Colors[ImGuiCol_TitleBgActive], 0.80f);
    style.Colors[ImGuiCol_TabHovered]            = style.Colors[ImGuiCol_HeaderHovered];
    style.Colors[ImGuiCol_TabActive]             = ImLerp(style.Colors[ImGuiCol_HeaderActive], style.Colors[ImGuiCol_TitleBgActive], 0.60f);
    style.Colors[ImGuiCol_TabUnfocused]          = ImLerp(style.Colors[ImGuiCol_Tab],          style.Colors[ImGuiCol_TitleBg], 0.80f);
    style.Colors[ImGuiCol_TabUnfocusedActive]    = ImLerp(style.Colors[ImGuiCol_TabActive],    style.Colors[ImGuiCol_TitleBg], 0.40f);
    style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    style.Colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style.Colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void renderGUI(ImageProcessor &processor) {
    ImGuiIO& io = ImGui::GetIO();
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(main_viewport->WorkPos);
    ImGui::SetNextWindowSize(main_viewport->WorkSize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Main Window", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);
    ImGui::PopStyleVar(3);

    if(ImGui::BeginMainMenuBar()) {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("Load Image...")) {
                std::string selected = openFileDialog_Linux();
                if(!selected.empty()) {
                    std::cout << "Image loaded successfully!\n";
                    processor.loadImage(selected);
                    textureNeedsUpdate = true;
                }
            }
            if(ImGui::MenuItem("Save Image As...")) {
                std::string selected = saveFileDialog_Linux();
                if (!selected.empty()) {
                    if (processor.saveImage(selected)) std::cout << "Image saved to " << selected << std::endl;
                    else std::cerr << "Failed to save image." << std::endl;
                }
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Help")) {
            ImGui::MenuItem("About");
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::Columns(3, "main_layout", false);
    ImGui::SetColumnWidth(0, 200.0f);
    ImGui::SetColumnWidth(1, ImGui::GetWindowWidth() - 400.0f);
    ImGui::SetColumnWidth(2, 200.0f);

    ImGui::BeginChild("Tool Panel", ImVec2(0, 0), true);
    ImGui::Text("Tool Panel:");
    ImGui::Separator();
    if(ImGui::Button("Load Image")) {
        std::string selected = openFileDialog_Linux();
        if(!selected.empty()) {
            std::cout << "Image loaded successfully!\n";
            processor.loadImage(selected);
            textureNeedsUpdate = true;
        }
    }
    if(ImGui::Button("Save Image")) {
        std::string selected = saveFileDialog_Linux();
        if (!selected.empty()) {
            if (processor.saveImage(selected)) std::cout << "Image saved to " << selected << std::endl;
            else std::cerr << "Failed to save image." << std::endl;
        }
    }
     if (ImGui::Button(compareMode ? "Compare: ON" : "Compare: OFF")) {
        compareMode = !compareMode;
    }
    ImGui::Text("Theme:");
    if(ImGui::Button(is_dark_theme ? "Switch to Light Theme" : "Switch to Dark Theme")) {
        is_dark_theme = !is_dark_theme;
        if(is_dark_theme) setModernStyle();
        else ImGui::StyleColorsLight();
    }
    ImGui::Separator();
    if(ImGui::Button("Reset Zoom & Pan", ImVec2(-1, 0))) {
        zoom_level = 1.0f;
        pan_offset = ImVec2(0, 0);
    }

    ImGui::Separator();
    ImGui::Text("History:");
    if(ImGui::Button("Undo", ImVec2(-1, 0))) {
        if(processor.undo()) {
            std::cout << "Undo successful." << std::endl;
            textureNeedsUpdate = true;
        }
        else std::cout << "Nothing to undo." << std::endl;
    }
    if(ImGui::Button("Redo", ImVec2(-1, 0))) {
        if(processor.redo()) {
            std::cout << "Redo successful." << std::endl;
            textureNeedsUpdate = true;
        }
        else std::cout << "Nothing to redo." << std::endl;
    }
    ImGui::EndChild();
    ImGui::NextColumn();

    ImGui::BeginChild("Image View", ImVec2(0, 0), true);
    const Image& currentImage = processor.getCurrentImage();
    if(currentImage.width > 0 && currentImage.height > 0) {
        if(textureNeedsUpdate) {
            if(currentTextureID != 0) glDeleteTextures(1, &currentTextureID);
            currentTextureID = loadTexture(currentImage);
            textureNeedsUpdate = false;
        }
        if(!compareMode) {
            if(ImGui::IsWindowHovered() && io.MouseWheel != 0.0f) {
                zoom_level += io.MouseWheel * 0.1f;
                zoom_level = std::clamp(zoom_level, 0.1f, 10.0f);
            }
            if(ImGui::IsWindowHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
                pan_offset.x += io.MouseDelta.x;
                pan_offset.y += io.MouseDelta.y;
            }
            ImVec2 zoomed_size = ImVec2(currentImage.width * zoom_level, currentImage.height * zoom_level);
            ImVec2 window_size = ImGui::GetContentRegionAvail();
            ImVec2 image_pos = ImVec2(
                (window_size.x - zoomed_size.x) * 0.5f,
                (window_size.y - zoomed_size.y) * 0.5f  
            );

            ImGui::SetCursorPos(image_pos);
            
            ImGui::Image((void*)(intptr_t)currentTextureID, zoomed_size);
        } else {
            renderCompareView(processor, zoom_level, pan_offset);
        }
    }
    ImGui::EndChild();
    ImGui::NextColumn();

    ImGui::BeginChild("Filters Panel", ImVec2(0, 0), true);
    ImGui::Text("Filters Panel");
    ImGui::Separator();
    filtersMenu(processor, textureNeedsUpdate);
    ImGui::EndChild();

    ImGui::Columns(1);
    ImGui::End();
}