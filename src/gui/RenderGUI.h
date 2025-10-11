#pragma once
#include "FiltersMenu.h"
#include "LoadTexture.h"

void renderGUI(ImageProcessor &processor) {
    ImGui::Begin("Image Processor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if(ImGui::Button("Load Image")) {
        // #TODO: Open a file dialog here
        // For now we'll load a hardcoded image.
        processor.loadImage("assets/SampleImages/luffy.jpg");
        std::cout << "Image loaded." << std::endl;
    }

    ImGui::SameLine();
    if(ImGui::Button("Save Image")) {
        if(processor.saveImage("output.jpg")) std::cout << "Image saved to output.jpg" << std::endl;
        else std::cerr << "Failed to save image." << std::endl;
    }
    
    ImGui::Separator();

    filtersMenu(processor);

    ImGui::Separator();

    ImGui::Text("History:");
    if(ImGui::Button("Undo")) {
        if(processor.undo()) std::cout << "Undo successful." << std::endl;
        else std::cout << "Nothing to undo." << std::endl;
    }

    ImGui::SameLine();
    if(ImGui::Button("Redo")) {
        if(processor.redo()) std::cout << "Redo successful." << std::endl;
        else std::cout << "Nothing to redo." << std::endl;
    }

    ImGui::End();

    ImGui::Begin("Image View");
    const Image& currentImage = processor.getCurrentImage();

    GLuint currentTextureID = 0;
    if(currentImage.width > 0 && currentImage.height > 0) {
        if(!currentTextureID) currentTextureID = loadTexture(currentImage);
        ImGui::Image((void*)(intptr_t)currentTextureID, ImVec2(currentImage.width, currentImage.height));
    }

    ImGui::End();
}