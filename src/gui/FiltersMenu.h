#pragma once
#include "core/ImageProcessor.h"
#include "FilterDefs.h"
#include "FilterParamsPanel.h"

void filtersMenu(ImageProcessor &processor, bool &textureNeedsUpdate, FilterType &selectedFilter) {
    if (ImGui::CollapsingHeader("Filters", ImGuiTreeNodeFlags_DefaultOpen)) {
        int count = 0;
        auto addButton = [&](const char* label, FilterType type) {
            ImGui::Indent(20.0f);
            if (ImGui::Selectable(label, selectedFilter == type, 0, ImVec2(150, 0))) {
                selectedFilter = type;
            }
            ImGui::Unindent(20.0f);
            count++;
            if (count % 1 != 0) ImGui::SameLine();
        };

        if (ImGui::CollapsingHeader("Basic Adjustments", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent(20.0f);
            count = 0;
            addButton("Grayscale",       FilterType::Grayscale);
            addButton("Invert",          FilterType::Invert);
            addButton("Black & White",   FilterType::BlackAndWhite);
            addButton("Brightness",      FilterType::Brightness);
            addButton("Contrast",        FilterType::Contrast);
            addButton("Saturation",      FilterType::Saturation);
            ImGui::Unindent(20.0f);
            ImGui::NewLine();
        }

        if (ImGui::CollapsingHeader("Transformations", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent(20.0f);
            count = 0;
            addButton("Crop",             FilterType::Crop);
            addButton("Resize",           FilterType::Resize);
            addButton("Horizontal Flip",  FilterType::HorizontalFlip);
            addButton("Vertical Flip",    FilterType::VerticalFlip);
            addButton("Rotate",           FilterType::Rotate);
            addButton("Skew",             FilterType::Skew);
            addButton("Merge",            FilterType::Merge);
            ImGui::Unindent(20.0f);
            ImGui::NewLine();
        }

        if (ImGui::CollapsingHeader("Effects", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent(20.0f);
            count = 0;
            addButton("Blur",            FilterType::Blur);
            addButton("Frame",           FilterType::Frame);
            addButton("Outline",         FilterType::Outline);
            addButton("Purple",          FilterType::Purple);
            addButton("Infrared",        FilterType::Infrared);
            addButton("Wave",            FilterType::Wave);
            addButton("Oil Painting",    FilterType::OilPainting);
            addButton("Retro",           FilterType::Retro);
            addButton("Vignette",        FilterType::Vignette);
            addButton("Warmth",          FilterType::Warmth);
            ImGui::Unindent(20.0f);
            ImGui::NewLine();
        }

        ImGui::Separator();
        ImGui::Text("Filter Parameters");
        renderFilterParamsPanel(processor, selectedFilter, textureNeedsUpdate);
    }
}
