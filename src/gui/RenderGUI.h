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
        if(is_dark_theme) ImGui::StyleColorsDark();
        else ImGui::StyleColorsLight();
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
            ImVec2 image_pos = ImGui::GetCursorScreenPos();
            ImGui::SetCursorScreenPos(ImVec2(image_pos.x + pan_offset.x, image_pos.y + pan_offset.y));
            
            ImGui::Image((void*)(intptr_t)currentTextureID, zoomed_size);
            ImGui::SetCursorScreenPos(image_pos);
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