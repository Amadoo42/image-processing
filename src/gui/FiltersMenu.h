#pragma once
#include "core/ImageProcessor.h"
#include "FilterDefs.h"
#include "FilterParamsPanel.h"
#include "FilterPreview.h"

void filtersMenu(ImageProcessor &processor, bool &textureNeedsUpdate, FilterType &selectedFilter) {
    if (ImGui::CollapsingHeader("Filters", ImGuiTreeNodeFlags_DefaultOpen)) {
        static FilterPreviewCache previewCache;
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
            // Grid with image previews
            std::vector<FilterType> basics = {
                FilterType::Grayscale,
                FilterType::Invert,
                FilterType::BlackAndWhite,
                FilterType::Brightness,
                FilterType::Contrast,
                FilterType::Saturation
            };
            // Only invalidate cache when a new image is loaded/applied, not every frame.
            bool invalidate = textureNeedsUpdate;
            renderFilterPreviewGrid(previewCache, processor, basics, selectedFilter, invalidate, "basic", 3, ImVec2(96, 72));
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
            std::vector<FilterType> effects = {
                FilterType::Blur,
                FilterType::Outline,
                FilterType::Purple,
                FilterType::Infrared,
                FilterType::Wave,
                FilterType::OilPainting,
                FilterType::Retro,
                FilterType::Vignette,
                FilterType::Warmth
            };
            bool invalidate = textureNeedsUpdate;
            renderFilterPreviewGrid(previewCache, processor, effects, selectedFilter, invalidate, "effects", 3, ImVec2(96, 72));
            // Non-previewable 'Frame' is kept as simple button
            addButton("Frame",           FilterType::Frame);
            ImGui::Unindent(20.0f);
            ImGui::NewLine();
        }

        ImGui::Separator();
        ImGui::Text("Filter Parameters");
        renderFilterParamsPanel(processor, selectedFilter, textureNeedsUpdate);
    }
}
