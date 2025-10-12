#pragma once
#include "FiltersMenu.h"
#include "LoadTexture.h"
#include "MemoryOperation.h"

void renderGUI(ImageProcessor &processor) {
    ImGuiIO& io = ImGui::GetIO();
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(main_viewport->WorkPos);
    ImGui::SetNextWindowSize(main_viewport->WorkSize);
    // ImGui::SetNextWindowViewport(main_viewport->ID);
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
        }
    }
    if(ImGui::Button("Save Image")) {
        std::string selected = saveFileDialog_Linux();
        if (!selected.empty()) {
            if (processor.saveImage(selected)) std::cout << "Image saved to " << selected << std::endl;
            else std::cerr << "Failed to save image." << std::endl;
        }
    }

    ImGui::Separator();
    ImGui::Text("History:");
    if(ImGui::Button("Undo", ImVec2(-1, 0))) {
        if(processor.undo()) std::cout << "Undo successful." << std::endl;
        else std::cout << "Nothing to undo." << std::endl;
    }
    if(ImGui::Button("Redo", ImVec2(-1, 0))) {
        if(processor.redo()) std::cout << "Redo successful." << std::endl;
        else std::cout << "Nothing to redo." << std::endl;
    }
    ImGui::EndChild();
    ImGui::NextColumn();

    ImGui::BeginChild("Image View", ImVec2(0, 0), true);
    const Image& currentImage = processor.getCurrentImage();
    GLuint currentTextureID;
    if(currentImage.width > 0 && currentImage.height > 0) {
        currentTextureID = loadTexture(currentImage);
        ImGui::Image((void*)(intptr_t)currentTextureID, ImVec2(currentImage.width, currentImage.height));
    }
    ImGui::EndChild();
    ImGui::NextColumn();

    ImGui::BeginChild("Filters Panel", ImVec2(0, 0), true);
    ImGui::Text("Filters Panel");
    ImGui::Separator();
    filtersMenu(processor);
    ImGui::EndChild();

    ImGui::Columns(1);
    ImGui::End();
}