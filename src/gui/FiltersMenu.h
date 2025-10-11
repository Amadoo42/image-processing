#pragma once
#include "core/ImageProcessor.h"
void filtersMenu(){
        if (ImGui::CollapsingHeader("Filters", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::Button("Grayscale")) {
            GrayscaleFilter filter;
            processor.applyFilter(filter);
            std::cout << "Grayscale filter applied." << std::endl;
        }

        if (ImGui::Button("Invert")) {
            InvertFilter filter;
            processor.applyFilter(filter);
            std::cout << "Invert filter applied." << std::endl;
        }

        if (ImGui::Button("Blur")) {
            BlurFilter filter;
            processor.applyFilter(filter);
            std::cout << "Blur filter applied." << std::endl;
        }

        if (ImGui::Button("Wave")) {
            WaveFilter filter;
            processor.applyFilter(filter);
            std::cout << "Wave filter applied." << std::endl;
        }

        // You can add more here
    }

}